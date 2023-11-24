//
// Created by jlaustill on 11/23/23.
//

#ifndef J1939_J1939_H
#define J1939_J1939_H

#include <cstdint>

class J1939 {
 public:
  J1939() {
    canId = 0;
    sourceAddress = 0;
    pgn = 0;
    pduFormat = 0;
    pduSpecific = 0;
    priority = 0;
  }
  uint32_t canId;
  uint8_t sourceAddress;
  uint16_t pgn;
  uint8_t pduSpecific;
  uint8_t pduFormat;
  uint8_t priority;
  uint8_t data[8];

  void setCanId(uint32_t _canId);
  uint8_t getSourceAddress(uint32_t canId);
  uint16_t getPgn(uint32_t canId);
  uint8_t getPduSpecific(uint32_t canId);
  uint8_t getPduFormat(uint32_t canId);
  uint8_t getPriority(uint32_t canId);
  void setData(uint8_t *data);
};

#endif  // J1939_J1939_H
