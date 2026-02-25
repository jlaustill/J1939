# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

C library (written in C-Next, transpiled to C) for parsing and constructing SAE J1939 CAN bus messages. Provides a `J1939Message` struct and `J1939` scope functions that abstract the 29-bit J1939 CAN identifier into semantic fields (priority, PGN, PDU format/specific, source address) and manage the 8-byte data payload. Published to the PlatformIO registry as `jlaustill/J1939`.

## Build & Test Commands

```bash
cnext src/J1939Message.cnx    # Transpile C-Next → C (uses cnext.config.json)
pio test                       # Run all tests (GoogleTest)
pio run                        # Build only (no tests)
./publish.sh                   # Transpile, test, then publish to PlatformIO registry
```

There is no single-test runner configured; all 23 tests run together via `pio test`.

## Architecture

**Struct + Scope design** — the library is:
- `src/J1939Message.cnx` — C-Next source (edit this)
- `src/J1939Message.c` — generated C implementation (do not edit directly)
- `include/J1939Message.h` — generated C header (do not edit directly)

**C-Next transpilation:** Run `cnext src/J1939Message.cnx` to regenerate `.c` and `.h` files. The `cnext.config.json` configures `basePath: "src"` and `headerOut: "include"` so headers land in `include/` without an extra `src/` prefix.

**J1939 CAN ID bit layout (29-bit):**
```
Bits 26-28: Priority (3 bits)
Bits 24-25: Reserved
Bits 16-23: PDU Format (PF, 8 bits)  ─┐
Bits  8-15: PDU Specific (PS, 8 bits) ─┘── PGN (16 bits at offset 8)
Bits  0-7:  Source Address (SA, 8 bits)
```

**Bidirectional sync pattern:** Setters update both the individual field AND reconstruct the composite `canId`. `setCanId()` decomposes the CAN ID into all individual fields. `setPgn()` also splits into `pduFormat` and `pduSpecific`.

**Default state:** `canId` initializes to `0x80000000` (bit 31 set), data bytes default to `0xFF`.

**API pattern:** `J1939Message` is a plain C struct. Functions live in the `J1939` scope:
- `J1939_init(J1939Message* msg)` — initialize struct (replaces constructor)
- `J1939_setCanId(J1939Message* msg, uint32_t canId)` — set CAN ID and decompose fields
- `J1939_getSourceAddress(uint32_t canId)` — stateless getter (takes raw canId)
- `J1939_setSourceAddress(J1939Message* msg, uint8_t sa)` — setter returns updated canId

## Dependencies

- **GoogleTest** — test framework, configured via `test_framework = googletest` in platformio.ini.
- No runtime dependencies. C-Next's built-in bit indexing (`canId[0, 8]`) replaces the former SeaDash dependency.

## Testing

Tests are in `test/J1939_test.cpp`. Tests are C++ (GoogleTest) calling the generated C API. The canonical test CAN ID is `2364145912` which decomposes to:
- PGN: 59904, PF: 234, PS: 0, Priority: 3, SA: 248

Tests cover: field extraction, field setting with canId sync, edge cases (0x00000000, 0x1FFFFFFF, 0xFFFFFFFF), struct copy, assignment, and field modification integrity.

## Conventions

- Edit `.cnx` source, then transpile — never edit generated `.c`/`.h` directly
- Getters take a raw `uint32_t canId` parameter (stateless extraction)
- Setters modify the struct and return the updated `canId`
- Library is framework/platform agnostic (`frameworks: "*"`, `platforms: "*"` in library.json)
- Tests build with `-D unitTesting` flag available for conditional compilation
