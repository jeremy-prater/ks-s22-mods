use log::{info, warn};
use std::vec::Vec;

pub fn process_command(message: Vec<u8>) -> Option<Vec<u8>> {
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
    
    let valid = message.len() == 20
        && message[0] == 0xAA
        && message[1] == 0x55
        && message[17] == 0x14
        && message[18] == 0x5A
        && message[19] == 0x5A;

    if !valid {
        return None;
    }

    let command = message[16];
    info!("KS Read : Command : {:x?}", &message);

    // Create an empty return response
    let mut response: [u8; 20] = [0; 20];
    response[0] = 0xAA;
    response[1] = 0x55;
    response[17] = 0x14;
    response[18] = 0x5A;
    response[19] = 0x5A;

    match command {
        0x98 => {}
        0x6d => {}
        0x63 => {}
        0x54 => {}
        _ => {
            warn!("Command {:x?} is not implemented", command);
        }
    }

    Some(response.to_vec())
}
