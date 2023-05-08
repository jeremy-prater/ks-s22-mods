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

## 0x9B - Subscribe to notification events

```
Payload : None
```


### Return the following data

```
0:00:00.192877364 ::: s22_client [INFO] > S22 update : bb -> [4b, 53, 2d, 53, 32, 32, 2d, 30, 32, 32, 37, 0, 0, 0] [14, 85, fd]
```

### Start notifying on the following responses


- Known

```
0:00:00.652879995 ::: s22_client [INFO] > S22 update : a9 -> [3d, 2e, 0, 0, c, 0, 8d, 16, 0, 0, 40, 6, 0, e0] [14, 5a, 5a]
0:00:00.252868416 ::: s22_client [INFO] > S22 update : b9 -> [0, 0, 0, 0, 42, 0, 0, 0, 14, 1, 0, 0, 78, 5] [14, 5a, 5a]
```

- Unknown

```
0:00:00.332831626 ::: s22_client [INFO] > S22 update : f5 -> [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1e, 0] [14, 5a, 5a]
0:00:00.572859715 ::: s22_client [INFO] > S22 update : f1 -> [eb, 2d, 0, 0, d0, 2, e8, 3, 0, 0, e8, 3, 48, f] [0, 5a, 5a]
0:00:00.412916114 ::: s22_client [INFO] > S22 update : f6 -> [d0, 7, 0, 0, 0, 0, 79, 47, 0, 0, 51, 0, 0, 0] [14, 5a, 5a]
0:00:00.492860137 ::: s22_client [INFO] > S22 update : c9 -> [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] [14, 5a, 5a]
```

---
## Command 0x9B is required to subscribe to notify events. This is required to get responses for the following commands.
---

## 0x63 - Request Serial Number


```
Payload : None
```

- Returns 0xB3 - Serial Number

```
0:00:00.278928166 ::: s22_client [INFO] > S22 update : b3 -> [4b, 53,  --- redacted ---, 31, 46] [30, 35, 36]
```



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

## f-series responses

- These have a unique pattern

```
0:00:07.267815517 ::: s22_client [INFO] > S22 update : Unknown (f1) [e4, 2d, 0, 0, cd, 2, e8, 3, 0, 0, e8, 3, 45, f] [0, 5a, 5a]
0:00:07.744693955 ::: s22_client [INFO] > S22 update : Unknown (f1) [8e, b, 90, b, 8d, b, 94, b, b0, b, 97, b, 9a, b] [1, 5a, 5a]
0:00:08.244780675 ::: s22_client [INFO] > S22 update : Unknown (f1) [47, f, 45, f, 45, f, 47, f, 45, f, 45, f, 45, f] [2, 5a, 5a]
0:00:08.724778386 ::: s22_client [INFO] > S22 update : Unknown (f1) [47, f, 47, f, 47, f, 47, f, 47, f, 48, f, 47, f] [3, 5a, 5a]
0:00:09.204794472 ::: s22_client [INFO] > S22 update : Unknown (f1) [47, f, 52, f, 52, f, 52, f, 50, f, 52, f, 52, f] [4, 5a, 5a]
0:00:09.684490307 ::: s22_client [INFO] > S22 update : Unknown (f1) [52, f, 52, f, 53, f, 52, f, 52, f, 53, f, 53, f] [5, 5a, 5a]
0:00:10.168133585 ::: s22_client [INFO] > S22 update : Unknown (f1) [53, f, 50, f, 0, 0, 0, 0, 9b, b, 0, 0, 0, 0] [6, 5a, 5a]
0:00:10.644826364 ::: s22_client [INFO] > S22 update : Unknown (f2) [cc, 2d, 0, 0, c4, 2, e8, 3, 0, 0, e8, 3, 4a, f] [0, 5a, 5a]
0:00:11.148073652 ::: s22_client [INFO] > S22 update : Unknown (f2) [8a, b, 8e, b, 8c, b, 94, b, ad, b, 8f, b, 9b, b] [1, 5a, 5a]
0:00:11.624728758 ::: s22_client [INFO] > S22 update : Unknown (f2) [48, f, 4a, f, 4a, f, 4a, f, 4a, f, 4a, f, 4a, f] [2, 5a, 5a]
0:00:12.124710364 ::: s22_client [INFO] > S22 update : Unknown (f2) [4a, f, 4b, f, 48, f, 4a, f, 4b, f, 4a, f, 4a, f] [3, 5a, 5a]
0:00:12.584865181 ::: s22_client [INFO] > S22 update : Unknown (f2) [4a, f, 3f, f, 3f, f, 3f, f, 3f, f, 3d, f, 3f, f] [4, 5a, 5a]
0:00:13.064852313 ::: s22_client [INFO] > S22 update : Unknown (f2) [3f, f, 3f, f, 3d, f, 3f, f, 3d, f, 3f, f, 40, f] [5, 5a, 5a]
0:00:13.548055900 ::: s22_client [INFO] > S22 update : Unknown (f2) [40, f, 3f, f, 0, 0, 0, 0, 9d, b, 0, 0, 0, 0] [6, 5a, 5a]
0:00:14.044362414 ::: s22_client [INFO] > S22 update : Unknown (f3) [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] [0, 5a, 5a]
0:00:14.524643606 ::: s22_client [INFO] > S22 update : Unknown (f3) [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] [1, 5a, 5a]
0:00:15.004676490 ::: s22_client [INFO] > S22 update : Unknown (f3) [0, 0, 0, 0, 0, 0, 46, 10, 0, 0, 0, 0, 0, 0] [2, 5a, 5a]
0:00:15.484759040 ::: s22_client [INFO] > S22 update : Unknown (f3) [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] [3, 5a, 5a]
0:00:15.964602234 ::: s22_client [INFO] > S22 update : Unknown (f4) [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] [0, 5a, 5a]
0:00:16.444690880 ::: s22_client [INFO] > S22 update : Unknown (f4) [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] [1, 5a, 5a]
0:00:16.944739642 ::: s22_client [INFO] > S22 update : Unknown (f4) [0, 0, 0, 0, 0, 0, 46, 10, 0, 0, 0, 0, 0, 0] [2, 5a, 5a]
0:00:17.424829675 ::: s22_client [INFO] > S22 update : Unknown (f4) [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] [3, 5a, 5a]
0:00:17.904602938 ::: s22_client [INFO] > S22 update : Unknown (f1) [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] [b0, 5a, 5a]
0:00:18.384418965 ::: s22_client [INFO] > S22 update : Unknown (f2) [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] [b0, 5a, 5a]
```

### f1 and f2 are full of data

- f1 : iterates through [0..6]
- f2 : iterates through [0..6]

### f3 and f4 are identical with only a 0x46 and a 0x10 

- f3 : iterates through [0..3]
- f4 : iterates through [0..3] 

### f5 appears to just have one entry which might be a temp

```
0:00:04.054097613 ::: s22_client [INFO] > S22 update : Unknown (f5) [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 0] [14, 5a, 5a]
```

### f1 and f2 send a b0 series

- f1 : re-iterates through [b0] : All 0's
- f2 : re-iterates through [b0] : All 0's

### Firmware Download

wget https://www.kingsong.vip/upload/firmware/2023/02/01/2023-02-01-03-20-46.bin
