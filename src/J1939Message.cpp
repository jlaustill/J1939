//
// Created by jlaustill on 11/23/23.
// Copyright 2023 Joshua Austill

#include "J1939Message.h"

#include <SeaDash.hpp>

void J1939Message::setCanId(uint32_t _canId) {
  this->canId = _canId;
  this->sourceAddress = getSourceAddress(_canId);
  this->pgn = getPgn(_canId);
  this->pduSpecific = getPduSpecific(_canId);
  this->pduFormat = getPduFormat(_canId);
  this->priority = getPriority(_canId);
}

void J1939Message::setData(uint8_t const *data) {
  for (uint8_t i = 0; i < 8; i++) {
    this->data[i] = data[i];
  }
}

uint8_t J1939Message::getSourceAddress(uint32_t canId) {
  return SeaDash::Bits::getNBits<uint32_t>(canId, 0, 8);
}

uint8_t J1939Message::setSourceAddress(uint8_t sourceAddress) {
  this->sourceAddress = sourceAddress;
  this->canId =
      SeaDash::Bits::setNBitsAt<uint32_t>(this->canId, sourceAddress, 0, 8);
  return this->canId;
}

uint16_t J1939Message::getPgn(uint32_t canId) {
  return SeaDash::Bits::getNBits<uint32_t>(canId, 8, 16);
}

uint32_t J1939Message::setPgn(uint16_t pgn) {
  this->pgn = pgn;
  this->pduSpecific = SeaDash::Bits::getNBits<uint16_t>(pgn, 0, 8);
  this->pduFormat = SeaDash::Bits::getNBits<uint16_t>(pgn, 8, 8);
  this->canId = SeaDash::Bits::setNBitsAt<uint32_t>(this->canId, pgn, 8, 16);
  return this->canId;
}

uint8_t J1939Message::getPduSpecific(uint32_t canId) {
  return SeaDash::Bits::getNBits<uint32_t>(canId, 8, 8);
}

uint32_t J1939Message::setPduSpecific(uint8_t pduSpecific) {
  this->pduSpecific = pduSpecific;
  this->canId =
      SeaDash::Bits::setNBitsAt<uint32_t>(this->canId, pduSpecific, 8, 8);
  return this->canId;
}

uint8_t J1939Message::getPduFormat(uint32_t canId) {
  return SeaDash::Bits::getNBits<uint32_t>(canId, 16, 8);
}

uint32_t J1939Message::setPduFormat(uint8_t pduFormat) {
  this->pduFormat = pduFormat;
  this->canId =
      SeaDash::Bits::setNBitsAt<uint32_t>(this->canId, pduFormat, 16, 8);
  return this->canId;
}

uint8_t J1939Message::getPriority(uint32_t canId) {
  return SeaDash::Bits::getNBits<uint32_t>(canId, 26, 3);
}

uint32_t J1939Message::setPriority(uint8_t priority) {
  this->priority = priority;
  this->canId =
      SeaDash::Bits::setNBitsAt<uint32_t>(this->canId, priority, 26, 3);
  return this->canId;
}
