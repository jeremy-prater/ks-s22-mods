use crate::ble::BleEvent;
use anyhow::{bail, Result};
use log::{info, warn};
use std::fmt;

#[derive(Debug, Default, PartialEq, Eq, Clone)]
pub struct KingSongPacket {
    pub data: [u8; 14],
    pub command: u8,
    pub footer: Option<[u8; 3]>,
}

impl fmt::Display for KingSongPacket {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let payload = match self.footer {
            Some(metadata) => {
                format!("{:x?} {:x?}", self.data, metadata)
            }
            None => {
                format!("{:x?}", self.data)
            }
        };

        let command = match self.event() {
            Some(event) => format!("{:?}", event),
            None => format!("Unknown ({:x?})", self.command),
        };

        write!(f, "{} {}", command, payload)
    }
}

impl KingSongPacket {
    pub fn event(&self) -> Option<BleEvent> {
        num::FromPrimitive::from_u8(self.command)
    }

    pub fn from_raw(data: Vec<u8>) -> Result<Self> {
        if data.len() != 20 {
            warn!("Incorrect command length {:x?}", data);
            bail!("Incorrect command length {:x?}", data);
        }
        if data[0] != 0xAA || data[1] != 0x55
        // || data[17] != 0x14
        // || data[18] != 0x5A
        // || data[19] != 0x5A
        {
            warn!("Invalid packet header {:x?}", data);
            bail!("Invalid packet header {:x?}", data);
        }

        Ok(KingSongPacket {
            data: data[2..16].try_into()?,
            command: data[16],
            footer: Some(data[17..20].try_into()?),
        })
    }

    pub fn from(data: &[u8], command: u8) -> Result<Self> {
        // Protocol format

        //  2 bytes - hdr = header (0xAA55)
        // 14 bytes - payload = command payload
        //  1 byte  - cm = Command
        //  3 bytes - footer (0x145A5A)
        // 20 bytes total

        // | hdr|       Payload               |cm|footer|
        // |----|----------------------------|--|------|
        //  AA55 xxxxxxxxxxxxxxxxxxxxxxxxxxxx yy 145A5A
        //

        if data.len() != 14 {
            warn!("Incorrect command length {:x?}", data);
            bail!("Incorrect command length {:x?}", data);
        }
        Ok(KingSongPacket {
            data: data.try_into()?,
            command,
            footer: None,
        })
    }

    pub fn generate_command(self) -> Vec<u8> {
        // Protocol format

        //  2 bytes - hdr = header (0xAA55)
        // 14 bytes - payload = command payload
        //  1 byte  - cm = Command
        //  3 bytes - footer (0x145A5A)
        // 20 bytes total

        // | hdr|       Payload               |cm|footer|
        // |----|----------------------------|--|------|
        //  AA55 xxxxxxxxxxxxxxxxxxxxxxxxxxxx yy 145A5A
        //

        // Create an empty return response
        let mut result = Vec::new();
        result.push(0xAA);
        result.push(0x55);
        for data in self.data {
            result.push(data);
        }
        result.push(self.command);
        result.push(0x14);
        result.push(0x5A);
        result.push(0x5A);

        result
    }
}
