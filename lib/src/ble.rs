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
