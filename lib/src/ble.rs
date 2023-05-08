use std::fmt;

// Service UUID for GATT example.
pub const KS_SERVICE_UUID: uuid::Uuid = uuid::Uuid::from_u128(0x0000ffe000001000800000805f9b34fb);

// Writeable, Writable with out response
pub const KS_CHARACTERISTIC_1_UUID: uuid::Uuid =
    uuid::Uuid::from_u128(0x0000ffe200001000800000805f9b34fb);

// Writeable with out response, Notify
pub const KS_CHARACTERISTIC_READ_UUID: uuid::Uuid =
    uuid::Uuid::from_u128(0x0000ffe100001000800000805f9b34fb);

// Client Characteristic Configuration
pub const KS_CHARACTERISTIC_CONFIG_UUID: uuid::Uuid =
    uuid::Uuid::from_u128(0x0000290200001000800000805f9b34fb);

#[derive(FromPrimitive, ToPrimitive, Debug)]
pub enum BleEvent {
    // Commands
    Some44Req = 0x44,
    Some54Req = 0x54,
    Some5EReq = 0x5E,

    RequestSerialNumber = 0x63,

    Some6D = 0x6D,

    EnableNotifications = 0x9B,

    // Notifications
    Some55Resp = 0x55,
    Some5FResp = 0x5F,
    Empty6E = 0x6E,
    VSOCTempRide = 0xA9, // Voltage, Speed, Odometer, Current (DC Amps), Temp?, Ride Mode
    DistTimeSpeedFan = 0xB9, // Trip Distance, Trip Time, Max Speed, Fan status
    SerialNumber = 0xB3,
    ModelName = 0xBB,
    AlarmTiltback = 0xB5, // Speed Alarm's 1, 2, 3. Tilback Speed

    ZeroC9 = 0xC9,

    F1 = 0xF1,
    F2 = 0xF2,
    F3 = 0xF3,
    F4 = 0xF4,
    F5Temp = 0xF5, // Unknown temp? Maybe mobo, maybe motor?
    F6Todo = 0xF6,   // Who knows?
}
