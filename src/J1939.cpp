//
// Created by jlaustill on 11/23/23.
//

#include "J1939.h"
#include <bit-utils.h>

void J1939::setCanId(uint32_t _canId) {
    this->canId = _canId;
    this->sourceAddress = getSourceAddress(_canId);
    this->pgn = getPgn(_canId);
    this->pduSpecific = getPduSpecific(_canId);
    this->pduFormat = getPduFormat(_canId);
    this->priority = getPriority(_canId);
}

uint8_t J1939::getSourceAddress(uint32_t canId) {
    return getNBits(canId, 0, 8);
}

uint16_t J1939::getPgn(uint32_t canId) {
    return getNBits(canId, 8, 16);
}

uint8_t J1939::getPduSpecific(uint32_t canId) {
    return getNBits(canId, 8, 8);
}

uint8_t J1939::getPduFormat(uint32_t canId) {
    return getNBits(canId, 16, 8);
}

uint8_t J1939::getPriority(uint32_t canId) {
    return getNBits(canId, 26, 3);
}