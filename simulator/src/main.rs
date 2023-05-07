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
use s22_library::ble::*;
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
        service_uuids: vec![KS_SERVICE_UUID].into_iter().collect(),
        manufacturer_data,
        discoverable: Some(true),
        local_name: Some("KSN-S22-TEST22".to_string()),
        ..Default::default()
    };
    let adv_handle = adapter.advertise(le_advertisement).await?;

    info!(
        "Serving GATT service on Bluetooth adapter {}",
        adapter.name()
    );
    let (ks_read_tx, ks_read_rx) = flume::unbounded();

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
                            let ks_read_tx = ks_read_tx.clone();
                            async move {
                                if let Some(response) =
                                    s22_library::process_command::process_command(message)
                                {
                                    ks_read_tx.send_async(response).await.unwrap();
                                }

                                Ok(())
                            }
                            .boxed()
                        })),
                        ..Default::default()
                    }),
                    notify: Some(CharacteristicNotify {
                        notify: true,
                        method: CharacteristicNotifyMethod::Fun(Box::new(move |mut notifier| {
                            let ks_read_rx = ks_read_rx.clone();
                            async move {
                                tokio::spawn(async move {
                                    info!(
                                        "Notification session start with confirming={:?}",
                                        notifier.confirming()
                                    );
                                    loop {
                                        let message = ks_read_rx.recv_async().await;
                                        match message {
                                            Ok(message) => {
                                                info!("KS Read : Response {:x?}", &message);
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
