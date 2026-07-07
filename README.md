# J1939

A C library for parsing and constructing SAE J1939 CAN bus messages. Decomposes the 29-bit CAN identifier into semantic fields (priority, PGN, PDU format/specific, source address) and manages the 8-byte data payload.

Written in [C-Next](https://github.com/jlaustill/c-next), transpiled to portable C. Works with any framework and platform.

## Installation

### PlatformIO

```ini
lib_deps = jlaustill/J1939
```

### Manual

Copy `include/J1939Message.h` and `src/J1939Message.c` into your project.

## Usage

### Parsing a received CAN message

```cnx
#include "J1939Message.cnx"

J1939Message msg;
J1939.init(msg);

// Received CAN ID from bus
u32 canId <- 0x18EA00F8; // PGN 59904, Priority 6, SA 248
J1939.setCanId(msg, canId);

// All fields are now decomposed
// msg.priority       = 6
// msg.pgn            = 59904
// msg.pduFormat      = 234
// msg.pduSpecific    = 0
// msg.sourceAddress  = 248
```

### Constructing a message to transmit

```cnx
J1939Message msg;
J1939.init(msg);

u16 pgn <- 59904;
u8 pri <- 6;
u8 sa <- 248;
J1939.setPgn(msg, pgn);
J1939.setPriority(msg, pri);
J1939.setSourceAddress(msg, sa);

// msg.canId is now ready to send
u32 canId <- msg.canId; // 0x18EA00F8

// Set the data payload
u8[8] data <- [0x00, 0xEE, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF];
J1939.setData(msg, data);
```

### Stateless field extraction

Getters can extract fields from any CAN ID without a message struct:

```cnx
u32 canId <- 0x18EA00F8;

u8 sa       <- J1939.getSourceAddress(canId); // 248
u16 pgn     <- J1939.getPgn(canId);           // 59904
u8 pf       <- J1939.getPduFormat(canId);     // 234
u8 ps       <- J1939.getPduSpecific(canId);   // 0
u8 pri      <- J1939.getPriority(canId);      // 6
```

### Using with C++

The generated header includes `extern "C"` guards, so it works directly in C++:

```cpp
#include <J1939Message.h>

J1939Message msg;
J1939_init(&msg);
J1939_setCanId(&msg, 0x18EA00F8);
```

## API Reference

### Struct

```cnx
struct J1939Message {
    u32 canId;
    u8 sourceAddress;
    u16 pgn;
    u8 pduFormat;
    u8 pduSpecific;
    u8 priority;
    u8[8] data;
}
```

### Functions

| Function | Description |
|----------|-------------|
| `J1939.init(msg)` | Initialize message (canId = 0x80000000, data = 0xFF) |
| `J1939.setCanId(msg, canId)` | Set CAN ID and decompose all fields |
| `J1939.setData(msg, data)` | Copy 8-byte payload into message |
| `J1939.getSourceAddress(canId)` | Extract source address (bits 0-7) |
| `J1939.setSourceAddress(msg, sa)` | Set source address, returns updated canId |
| `J1939.getPgn(canId)` | Extract PGN (bits 8-23) |
| `J1939.setPgn(msg, pgn)` | Set PGN (also updates pduFormat/pduSpecific), returns updated canId |
| `J1939.getPduFormat(canId)` | Extract PDU Format (bits 16-23) |
| `J1939.setPduFormat(msg, pf)` | Set PDU Format, returns updated canId |
| `J1939.getPduSpecific(canId)` | Extract PDU Specific (bits 8-15) |
| `J1939.setPduSpecific(msg, ps)` | Set PDU Specific, returns updated canId |
| `J1939.getPriority(canId)` | Extract priority (bits 26-28) |
| `J1939.setPriority(msg, pri)` | Set priority, returns updated canId |

### CAN ID Bit Layout (29-bit)

```
Bits 26-28: Priority (3 bits)
Bits 24-25: Reserved
Bits 16-23: PDU Format (PF, 8 bits)  ─┐
Bits  8-15: PDU Specific (PS, 8 bits) ─┘── PGN (16 bits)
Bits  0-7:  Source Address (SA, 8 bits)
```

## MCU Health Module

`J1939McuHealth` is a small companion codec for a **fleet-wide MCU health frame** — a
Proprietary B PGN (`0xFFDC` / 65500, broadcast) that every microcontroller on the bus
transmits with its own source address, so a listener can collect one health record per
SA. It carries MCU die temperature, reset cause, boot count, and uptime.

### Payload layout (8 bytes, little-endian)

| Bytes | Field | Encoding | N/A sentinel |
|-------|-------|----------|--------------|
| 0–1 | MCU die temp | 0.03125 °C/bit, offset −273 | `0xFFFF` |
| 2 | Reset cause | `J1939McuHealth_RESET_*` code | `0xFF` (not implemented) |
| 3–4 | Boot count | `uint16` | `0xFFFF` |
| 5–6 | Uptime | `uint16` minutes | `0xFFFF` |
| 7 | Reserved | `0xFF` | — |

Each field carries its own N/A sentinel, so a controller that only knows some fields
still emits a valid frame. Set the matching `*Valid` flag to `false` (or `resetCause` to
`J1939McuHealth_RESET_NOT_IMPLEMENTED`) and `encode` writes the sentinel.

### Encoding on a transmitter

```cpp
#include <J1939McuHealth.h>

McuHealth h;
h.temperatureC = 62.5f;   h.temperatureValid = true;
h.resetCause   = J1939McuHealth_RESET_WDOG;
h.bootCount    = 12;      h.bootCountValid   = true;
h.uptimeMinutes = 340;    h.uptimeValid      = true;

uint8_t data[8];
J1939McuHealth_encode(&h, data);
// send data on PGN J1939McuHealth_PGN, every J1939McuHealth_TRANSMIT_INTERVAL_MS ms
```

### Decoding on a listener

```cpp
uint8_t data[8] = { /* received payload for PGN 0xFFDC */ };
McuHealth h;
J1939McuHealth_decode(data, &h);

if (h.temperatureValid) { /* h.temperatureC is the MCU die temp */ }
// h.resetCause is passed through verbatim (0xFF = sender doesn't implement it)
```

### Reset-cause codes

`RESET_UNKNOWN`=0, `RESET_POR`=1, `RESET_LOCKUP`=2, `RESET_PIN`=3, `RESET_WDOG`=4,
`RESET_WDOG3`=5, `RESET_TEMPSENSE`=6, `RESET_JTAG`=7, `RESET_CSU`=8,
`RESET_NOT_IMPLEMENTED`=`0xFF` (matches the Teensy i.MX RT1062 `SRSR` decode).

## License

MIT
