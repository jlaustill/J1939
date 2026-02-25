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

```c
#include <J1939Message.h>

J1939Message msg;
J1939_init(&msg);

// Received CAN ID from bus
uint32_t canId = 0x18EA00F8; // PGN 59904, Priority 6, SA 248
J1939_setCanId(&msg, canId);

// All fields are now decomposed
printf("Priority: %u\n", msg.priority);       // 6
printf("PGN: %u\n", msg.pgn);                 // 59904
printf("PDU Format: %u\n", msg.pduFormat);     // 234
printf("PDU Specific: %u\n", msg.pduSpecific); // 0
printf("Source Addr: %u\n", msg.sourceAddress); // 248
```

### Constructing a message to transmit

```c
J1939Message msg;
J1939_init(&msg);

J1939_setPgn(&msg, 59904);
J1939_setPriority(&msg, 6);
J1939_setSourceAddress(&msg, 248);

// msg.canId is now ready to send
uint32_t canId = msg.canId; // 0x18EA00F8

// Set the data payload
uint8_t data[8] = {0x00, 0xEE, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
J1939_setData(&msg, data);
```

### Stateless field extraction

Getters can extract fields from any CAN ID without a message struct:

```c
uint32_t canId = 0x18EA00F8;

uint8_t sa      = J1939_getSourceAddress(canId); // 248
uint16_t pgn    = J1939_getPgn(canId);           // 59904
uint8_t pf      = J1939_getPduFormat(canId);     // 234
uint8_t ps      = J1939_getPduSpecific(canId);   // 0
uint8_t pri     = J1939_getPriority(canId);      // 6
```

### Using with C++

The header includes `extern "C"` guards, so it works directly in C++:

```cpp
#include <J1939Message.h>

J1939Message msg;
J1939_init(&msg);
J1939_setCanId(&msg, 0x18EA00F8);
```

## API Reference

### Struct

```c
typedef struct J1939Message {
    uint32_t canId;
    uint8_t  sourceAddress;
    uint16_t pgn;
    uint8_t  pduFormat;
    uint8_t  pduSpecific;
    uint8_t  priority;
    uint8_t  data[8];
} J1939Message;
```

### Functions

| Function | Description |
|----------|-------------|
| `J1939_init(msg)` | Initialize message (canId = 0x80000000, data = 0xFF) |
| `J1939_setCanId(msg, canId)` | Set CAN ID and decompose all fields |
| `J1939_setData(msg, data)` | Copy 8-byte payload into message |
| `J1939_getSourceAddress(canId)` | Extract source address (bits 0-7) |
| `J1939_setSourceAddress(msg, sa)` | Set source address, returns updated canId |
| `J1939_getPgn(canId)` | Extract PGN (bits 8-23) |
| `J1939_setPgn(msg, pgn)` | Set PGN (also updates pduFormat/pduSpecific), returns updated canId |
| `J1939_getPduFormat(canId)` | Extract PDU Format (bits 16-23) |
| `J1939_setPduFormat(msg, pf)` | Set PDU Format, returns updated canId |
| `J1939_getPduSpecific(canId)` | Extract PDU Specific (bits 8-15) |
| `J1939_setPduSpecific(msg, ps)` | Set PDU Specific, returns updated canId |
| `J1939_getPriority(canId)` | Extract priority (bits 26-28) |
| `J1939_setPriority(msg, pri)` | Set priority, returns updated canId |

### CAN ID Bit Layout (29-bit)

```
Bits 26-28: Priority (3 bits)
Bits 24-25: Reserved
Bits 16-23: PDU Format (PF, 8 bits)  ─┐
Bits  8-15: PDU Specific (PS, 8 bits) ─┘── PGN (16 bits)
Bits  0-7:  Source Address (SA, 8 bits)
```

## License

MIT
