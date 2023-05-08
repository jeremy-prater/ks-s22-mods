# Kingsong S22 modification tools

# S22 BLE Client

The client code will connect to a real S22 and start querying data. This is used for exploratory purposes to collect data against a real S22

# S22 BLE Server

This represents a `fake` S22 by emulating the protocol and BLE services/characteristics. This allows us to test specific features like firmware update to gain knowledge about various processes between the app and S22

# Protocol format
```
| hdr|       Payload              |cm|footer|
|----|----------------------------|--|------|
 AA55 xxxxxxxxxxxxxxxxxxxxxxxxxxxx yy 145A5A
``` 

-  2 bytes - hdr = header (0xAA55)
- 14 bytes - payload = command payload
-  1 byte  - cm = Command
-  3 bytes - footer (0x145A5A)
- 20 bytes total
 
### Note : Some responses use the 3 bytes in the footer as a payload

# Command codes

- TODO : work in progress

# Response Codes

## 0xA9

```
Payload

 VV SS OOOO CC TT M
|--|--|----|--|--|-|
```

| Sym  | Measurement    | Payload Bytes | Equation |
|------|----------------|---------------|----------|
| VV   | Voltage        | 0 - 1         | x / 100  |
| SS   | Speed          | 2 - 3         | x / 100  |
| OOOO | ODO            | 4 - 7         | x / 1000 |
| CC   | Current        | 8 - 9         | x / 100  |
| TT   | Temp           | 10 - 11       | x / 100  |
| M    | Ride Mode      | 12            |          |

## 0xB9

```
Payload

 TTTT CC SS F
|----|--|--|-|
```

| Sym  | Measurement    | Payload Bytes | Equation |
|------|----------------|---------------|----------|
| TTTT | Trip Distance  | 0 - 3         | x / 1000 |
| CC   | Uptime         | 4 - 5         | x / 100  |
| SS   | Top Speed      | 6 - 7         | x / 100  |
| F    | Fan Mode       | 10            |          |

## 0xB5

```
Payload

 A1 A2 A3 TB
|--|--|--|--|
```

| Sym  | Measurement    | Payload Bytes | Equation |
|------|----------------|---------------|----------|
| A1   | Speed Alarm 1  |  2            |          |
| A2   | Speed Alarm 2  |  3            |          |
| A3   | Speed Alarm 3  |  6            |          |
| TB   | Tilt back      |  8            |          |


## 0xB3

```
Payload

 SN
|--|-->
```

| Sym  | Measurement    | Payload Bytes | Equation |
|------|----------------|---------------|----------|
| SN   | Serial Number  |  0 - 14 + 3   |          |

## 0xBB

```
Payload

 MN
|--|
```

| Sym  | Measurement    | Payload Bytes | Equation |
|------|----------------|---------------|----------|
| MN   | Model Name     |  0 - 14       |          |
