extern crate num;
extern crate num_derive;

use anyhow::Result;
use bluer::{gatt::remote::Service, Adapter, Device};
use futures::{pin_mut, StreamExt};
use log::{info, warn};
use s22_library::ble::BleEvent;
use s22_library::packet::KingSongPacket;

use std::collections::{HashMap, VecDeque};
use std::sync::Arc;
use std::time::Duration;
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

    let (command_tx, command_rx) = flume::unbounded::<KingSongPacket>();
    let (notify_tx, notify_rx) = flume::unbounded();

    let s22_state = Arc::new(Mutex::new(HashMap::new()));

    for characteristic in service.characteristics().await? {
        match characteristic.uuid().await {
            Ok(s22_library::ble::KS_CHARACTERISTIC_READ_UUID) => {
                info!("Found KS Read characteristic");

                for descriptor in characteristic.descriptors().await? {
                    if descriptor.uuid().await.unwrap()
                        == s22_library::ble::KS_CHARACTERISTIC_CONFIG_UUID
                    {
                        info!("Found KS client config characteristic");
                    }

                    let notify_characteristic = characteristic.clone();
                    let notify_tx = notify_tx.clone();
                    let s22_state: Arc<Mutex<HashMap<u8, KingSongPacket>>> = s22_state.clone();
                    tokio::task::spawn(async move {
                        info!("Starting KS notify read loop");
                        let notify_stream = notify_characteristic.notify().await.unwrap();
                        pin_mut!(notify_stream);
                        while let Some(event) = notify_stream.next().await {
                            match KingSongPacket::from_raw(event.clone()) {
                                Ok(response) => {
                                    let mut lock = s22_state.lock().await;
                                    let updated = match lock.get(&response.command) {
                                        Some(packet) => {
                                            let different = packet != &response;
                                            if different {
                                                lock.insert(response.command, response.clone());
                                            }
                                            different
                                        }
                                        None => {
                                            lock.insert(response.command, response.clone());
                                            true
                                        }
                                    };
                                    if updated {
                                        info!("S22 update : {}", response);
                                    }
                                    notify_tx.send_async(response).await.unwrap();
                                }
                                Err(err) => {
                                    warn!("KS notify : Packet error : {:?}", err);
                                }
                            }
                        }
                    });
                    let write_characteristic = characteristic.clone();
                    let command_rx = command_rx.clone();
                    tokio::task::spawn(async move {
                        info!("Starting KS write loop");
                        // Send next command
                        while let Ok(command) = command_rx.recv_async().await {
                            info!("S22 cmd    : {}", command);
                            let command = command.generate_command();
                            write_characteristic.write(&command).await.unwrap();
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
    let command_sequence: Arc<Mutex<VecDeque<s22_library::packet::KingSongPacket>>> =
        Arc::new(Mutex::new(VecDeque::new()));
    {
        let mut lock = command_sequence.lock().await;
        lock.push_front(KingSongPacket {
            command: BleEvent::EnableNotifications as u8,
            ..Default::default()
        });
        lock.push_front(KingSongPacket {
            command: BleEvent::RequestSerialNumber as u8,
            ..Default::default()
        });
        lock.push_front(KingSongPacket {
            command: BleEvent::Some6D as u8,
            ..Default::default()
        });
        // lock.push_front(KingSongPacket {
        //     command: BleEvent::Some54Req as u8,
        //     ..Default::default()
        // });
        // lock.push_front(KingSongPacket {
        //     command: BleEvent::Some5EReq as u8,
        //     ..Default::default()
        // });
        // lock.push_front(KingSongPacket {
        //     command: BleEvent::Some44Req as u8,
        //     ..Default::default()
        // });
    }

    loop {
        let next_command = {
            let mut lock = command_sequence.lock().await;
            // lock.push_back(0x44); // Not sure what this does...
            lock.pop_back()
        };
        match next_command {
            Some(command) => {
                command_tx.send_async(command).await.unwrap();
                let _response = notify_rx.recv_async().await.unwrap();
            }
            None => {
                tokio::time::sleep(Duration::from_secs(1)).await;
            }
        }
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
