use anyhow::Result;
use bluer::{
    adv::Advertisement,
    gatt::local::{
        Application, Characteristic, CharacteristicNotify, CharacteristicNotifyMethod,
        CharacteristicWrite, CharacteristicWriteMethod, Service,
    },
};
use futures::FutureExt;
use log::{info, warn};
use s22_library::{ble::*, packet::KingSongPacket};
use std::{collections::BTreeMap, time::Duration};
use tokio::io::{AsyncBufReadExt, BufReader};
use tokio::time::sleep;

#[tokio::main]
async fn main() -> Result<()> {
    s22_library::logger::init_logging()?;

    let session = bluer::Session::new().await?;
    let adapter = session.default_adapter().await?;
    adapter.set_powered(true).await?;

    info!(
        "Advertising on Bluetooth adapter {} with address {}",
        adapter.name(),
        adapter.address().await?
    );
    let mut manufacturer_data = BTreeMap::new();
    manufacturer_data.insert(0x4542, vec![0x53, 0x54, 0x20, 0x43, 0x4f, 0x52, 0x45]);
    manufacturer_data.insert(0xE339, vec![0x01, 0xc3, 0xc5, 0xc2]);
    let le_advertisement = Advertisement {
        service_uuids: Default::default(), //vec![KS_SERVICE_UUID].into_iter().collect(),
        manufacturer_data,
        discoverable: Some(true),
        local_name: Some("KSN-S22-TEST22".to_string()),
        appearance: Some(0x0000),
        ..Default::default()
    };
    let adv_handle = adapter.advertise(le_advertisement).await?;

    info!(
        "Serving GATT service on Bluetooth adapter {}",
        adapter.name()
    );
    let (ks_notify_tx, ks_notify_rx) = flume::unbounded();
    let (ks_write_tx, ks_write_rx) = flume::unbounded();

    let app = Application {
        services: vec![Service {
            uuid: KS_SERVICE_UUID,
            primary: true,
            characteristics: vec![
                Characteristic {
                    uuid: KS_CHARACTERISTIC_1_UUID,
                    write: Some(CharacteristicWrite {
                        write: true,
                        write_without_response: true,
                        method: CharacteristicWriteMethod::Fun(Box::new(move |new_value, req| {
                            async move {
                                info!(
                                    "KS_1 : Write request {:?} with value {:x?}",
                                    &req, &new_value
                                );

                                Ok(())
                            }
                            .boxed()
                        })),
                        ..Default::default()
                    }),
                    ..Default::default()
                },
                Characteristic {
                    uuid: KS_CHARACTERISTIC_READ_UUID,
                    write: Some(CharacteristicWrite {
                        write: false,
                        write_without_response: true,
                        method: CharacteristicWriteMethod::Fun(Box::new(move |message, _req| {
                            let ks_write_tx = ks_write_tx.clone();
                            async move {
                                ks_write_tx.send_async(message).await.unwrap();

                                Ok(())
                            }
                            .boxed()
                        })),
                        ..Default::default()
                    }),
                    notify: Some(CharacteristicNotify {
                        notify: true,
                        method: CharacteristicNotifyMethod::Fun(Box::new(move |mut notifier| {
                            let ks_notify_rx = ks_notify_rx.clone();
                            async move {
                                tokio::spawn(async move {
                                    info!(
                                        "Notification session start with confirming={:?}",
                                        notifier.confirming()
                                    );
                                    loop {
                                        let message = ks_notify_rx.recv_async().await;
                                        match message {
                                            Ok(message) => {
                                                if let Err(err) = notifier.notify(message).await {
                                                    warn!("Notification error: {}", &err);
                                                    break;
                                                }
                                            }
                                            Err(err) => {
                                                warn!("Notification error: {}", &err);
                                                break;
                                            }
                                        }
                                    }
                                    info!("Notification session stop");
                                });
                            }
                            .boxed()
                        })),
                        ..Default::default()
                    }),
                    // descriptors: vec![Descriptor {
                    //     uuid: KS_CHARACTERISTIC_CONFIG_UUID,
                    //     ..Default::default()
                    // }],
                    ..Default::default()
                },
            ],
            ..Default::default()
        }],
        ..Default::default()
    };
    let app_handle = adapter.serve_gatt_application(app).await?;
    let mut notification_thread: Option<tokio::task::JoinHandle<()>> = None;
    loop {
        match ks_write_rx.recv_async().await {
            Ok(command) => {
                if let Ok(packet) = s22_library::packet::KingSongPacket::from_raw(command) {
                    info!("Got packet : {}", packet);
                    if let Some(event) = packet.event() {
                        match event {
                            BleEvent::EnableNotifications => {
                                info!("Starting KS notification thread");
                                let mut packet = KingSongPacket::new(BleEvent::ModelName);
                                packet.set_model_name("KS-S22-0221");
                                info!("KS Notify : {}", packet);
                                ks_notify_tx
                                    .send_async(packet.generate_command())
                                    .await
                                    .unwrap();

                                if notification_thread.is_none() {
                                    let ks_notify_tx = ks_notify_tx.clone();
                                    notification_thread = Some(tokio::spawn(async move {
                                        loop {
                                            let mut packet =
                                                KingSongPacket::new(BleEvent::VSOCTempRide);
                                            packet.set_vsoc(126.0, 15.0, 2000.0, 5.0, 30.0, 0);
                                            info!("KS Notify : {}", packet);
                                            ks_notify_tx
                                                .send_async(packet.generate_command())
                                                .await
                                                .unwrap();
                                            tokio::time::sleep(Duration::from_millis(250)).await;
                                        }
                                    }));
                                }
                            }
                            BleEvent::RequestSerialNumber => {
                                let mut packet = KingSongPacket::new(BleEvent::SerialNumber);
                                packet.set_serial_number(
                                    &[
                                        0x4b, 0x53, 0x53, 0x41, 0x30, 0x31, 0x42, 0x32, 0x32, 0x30,
                                        0x35, 0x31, 0x31, 0x46,
                                    ],
                                    &[0x30, 0x35, 0x36],
                                );
                                info!("KS Notify : {}", packet);
                                ks_notify_tx
                                    .send_async(packet.generate_command())
                                    .await
                                    .unwrap();
                            }
                            BleEvent::Some44Req => {}
                            BleEvent::Some54Req => {}
                            BleEvent::Some5EReq => {}
                            BleEvent::Some6D => {}
                            _ => {}
                        }
                    }
                }
            }
            Err(err) => {
                warn!("KS Write : Error : {err}");
                break;
            }
        }
    }

    info!("Service ready. Press enter to quit.");
    let stdin = BufReader::new(tokio::io::stdin());
    let mut lines = stdin.lines();
    let _ = lines.next_line().await;

    info!("Removing service and advertisement");
    drop(app_handle);
    drop(adv_handle);
    sleep(Duration::from_secs(1)).await;

    Ok(())
}
