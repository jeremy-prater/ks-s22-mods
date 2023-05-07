use anyhow::Result;
use bluer::{gatt::remote::Service, Adapter, Device};
use futures::{pin_mut, StreamExt};
use log::{info, warn};
use s22_library::packet::KingSongPacket;
use std::collections::VecDeque;
use std::sync::Arc;
use tokio::sync::Mutex;

#[tokio::main]
async fn main() -> Result<()> {
    s22_library::logger::init_logging()?;

    let args: Vec<String> = std::env::args().collect();

    // C2:C5:C3:01:E3:39 - My S22 mac

    if args.len() != 2 {
        warn!("Usage : s22-client <mac of s22>");
        return Ok(());
    }
    let mac = args[1].to_string();

    let session = bluer::Session::new().await?;
    let adapter = session.default_adapter().await?;
    adapter.set_powered(true).await?;

    info!("s22-client : searching for mac {}", mac);

    let (device, service) = find_kingsong_s22(adapter, mac).await.unwrap();
    info!("Found KS device : {:?} {:?}", device, service);

    let (command_tx, command_rx) = flume::unbounded();
    let (notify_tx, notify_rx) = flume::unbounded();

    for characteristic in service.characteristics().await? {
        match characteristic.uuid().await {
            Ok(s22_library::ble::KS_CHARACTERISTIC_READ_UUID) => {
                info!("Found KS Read characteristic");

                for descriptor in characteristic.descriptors().await? {
                    match descriptor.uuid().await.unwrap() {
                        s22_library::ble::KS_CHARACTERISTIC_CONFIG_UUID => {
                            info!("Found KS client config characteristic");
                        }
                        _ => {}
                    }

                    let characteristic = characteristic.clone();
                    let notify_tx = notify_tx.clone();
                    tokio::task::spawn(async move {
                        info!("Starting KS notify read loop");
                        let notify_stream = characteristic.notify().await.unwrap();
                        pin_mut!(notify_stream);
                        while let Some(event) = notify_stream.next().await {
                            info!("Notify from S22 : {:?}", event);
                            notify_tx.send_async(event.clone()).await.unwrap();
                        }
                    });
                }
            }
            Ok(s22_library::ble::KS_CHARACTERISTIC_1_UUID) => {
                info!("Found KS ??? characteristic");
            }
            _ => {
                warn!("Unknown characteristic : {:?}", characteristic);
            }
        };
    }

    // Follow command sequence from darkness bot...? Ehh ok
    let mut command_sequence: Arc<Mutex<VecDeque<s22_library::packet::KingSongPacket>>> =
        Arc::new(Mutex::new(VecDeque::new()));
    {
        let mut lock = command_sequence.lock().await;
        // lock.push_back(0x44); // Not sure what this does...
        command_tx
            .send_async(KingSongPacket {
                command: 0x63,
                ..Default::default()
            })
            .await?;
        command_tx
            .send_async(KingSongPacket {
                command: 0x9b,
                ..Default::default()
            })
            .await?;
        command_tx
            .send_async(KingSongPacket {
                command: 0x6d,
                ..Default::default()
            })
            .await?;
        command_tx
            .send_async(KingSongPacket {
                command: 0x54,
                ..Default::default()
            })
            .await?;
        command_tx
            .send_async(KingSongPacket {
                command: 0x5e,
                ..Default::default()
            })
            .await?;
        command_tx
            .send_async(KingSongPacket {
                command: 0x98,
                ..Default::default()
            })
            .await?;
    }

    loop {
        // Send next command
        let command = command_rx.recv_async().await.unwrap();
        info!(" Sending command  : {:?}", command);
        let response = notify_rx.recv_async().await.unwrap();
        info!(" Sending response : {:?}", response);
    }

    // Ok(())
}

async fn find_kingsong_s22(adapter: Adapter, mac: String) -> Option<(Device, Service)> {
    let mut discover = adapter.discover_devices().await.unwrap();
    while let Some(event) = discover.next().await {
        if let bluer::AdapterEvent::DeviceAdded(address) = event {
            if address.to_string() == mac {
                let device = adapter.device(address).unwrap();
                device.connect().await.unwrap();
                let services = device.services().await.unwrap();
                for service in services {
                    if service.uuid().await.unwrap_or_default() == s22_library::ble::KS_SERVICE_UUID
                    {
                        return Some((device, service));
                    }
                }
            }
        }
    }
    None
}
