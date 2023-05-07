use anyhow::{bail, Result};

#[derive(Debug, Default)]
pub struct KingSongPacket {
    pub data: [u8; 14],
    pub command: u8,
}

impl KingSongPacket {
    pub fn from(data: [u8; 14], command: u8) -> Result<Self> {
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
            bail!("Incorrect command length {:x?}", data);
        }
        Ok(KingSongPacket { data, command })
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
