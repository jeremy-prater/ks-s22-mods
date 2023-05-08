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

#[derive(FromPrimitive, Debug)]
pub enum BleEvent {
    EnableNotifications = 0x9B,
    RequestSerialNumber = 0x63,

    // Vehicle status
    VSOCTempRide = 0xA9, // Voltage, Speed, Odometer, Current (DC Amps), Temp?, Ride Mode
    SomeTemp = 0xF5, // Unknown temp? Maybe mobo, maybe motor?

    // Trip status
    DistTimeSpeedFan = 0xB9, // Trip Distance, Trip Time, Max Speed, Fan status

    // Alarm setpoints
    AlarmTiltback = 0xB5, // Speed Alarm's 1, 2, 3. Tilback Speed

    // Device Information
    SerialNumber = 0xB3,
    ModelName = 0xBB,
}
