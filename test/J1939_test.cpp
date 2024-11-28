#include <J1939Message.h>
#include <gtest/gtest.h>

J1939Message message;
uint32_t canId = 2364145912;
uint16_t pgn = 59904;
uint8_t pduSpecific = 0;
uint8_t pduFormat = 234;
uint8_t priority = 3;
uint8_t sourceAddress = 248;

TEST(J1939Message, setCanId) {
  message = J1939Message();
  message.setCanId(canId);
  
  EXPECT_EQ(message.canId, canId);
  EXPECT_EQ(message.sourceAddress, sourceAddress);
  EXPECT_EQ(message.pgn, pgn);
  EXPECT_EQ(message.pduSpecific, pduSpecific);
  EXPECT_EQ(message.pduFormat, pduFormat);
  EXPECT_EQ(message.priority, priority);
}

TEST(J1939Message, setAllGetCanId) {
  message = J1939Message();

  message.setPgn(pgn);
  message.setPriority(priority);
  message.setSourceAddress(sourceAddress);

  EXPECT_EQ(message.canId, canId);
}

TEST(J1939Message, getSourceAddress) {
  message = J1939Message();
  uint8_t result = message.getSourceAddress(0b100000000000010101010);

  EXPECT_EQ(result, 0b10101010);
}

TEST(J1939Message, getGgn) {
  message = J1939Message();
  uint16_t result = message.getPgn(canId);

  EXPECT_EQ(result, pgn);
}

TEST(J1939Message, getPduSpecific) {
  message = J1939Message();
  uint8_t result = message.getPduSpecific(canId);

  EXPECT_EQ(result, pduSpecific);
}

TEST(J1939Message, getPduFormat) {
  message = J1939Message();
  uint8_t result = message.getPduFormat(canId);

  EXPECT_EQ(result, pduFormat);
}

TEST(J1939Message, getPriority) {
  message = J1939Message();
  uint8_t result = message.getPriority(canId);

  EXPECT_EQ(result, priority);
}

TEST(J1939Message, setData) {
  message = J1939Message();
  uint8_t const data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  message.setData(data);

  for (int i = 0; i < 8; i++) {
    EXPECT_EQ(message.data[i], data[i]);
  }
}

TEST(J1939Message, setPgn) {
  message = J1939Message();
  message.setCanId(0);
  EXPECT_EQ(message.canId, 0);
  message.setPgn(59905);
  EXPECT_EQ(message.pgn, 59905);
  EXPECT_EQ(message.pduSpecific, 1);
  EXPECT_EQ(message.pduFormat, 234);
  message.setPriority(6);
  EXPECT_EQ(message.priority, 6);
  message.setSourceAddress(254);
  EXPECT_EQ(message.sourceAddress, 254);
  EXPECT_EQ(message.canId, 417989118);
}

TEST(J1939Message, setCanId_MaxValue) {
  message = J1939Message();
  uint32_t maxCanId = 0x1FFFFFFF;  // Max 29-bit CAN ID
  message.setCanId(maxCanId);
  EXPECT_EQ(message.priority, 7);
  EXPECT_EQ(message.pgn, 65535);
  EXPECT_EQ(message.pduFormat, 255);
  EXPECT_EQ(message.pduSpecific, 255);
  EXPECT_EQ(message.sourceAddress, 255);
}

TEST(J1939Message, setCanId_MinValue) {
  message = J1939Message();
  uint32_t minCanId = 0;  // Min CAN ID
  message.setCanId(minCanId);

  EXPECT_EQ(message.canId, minCanId);
  EXPECT_EQ(message.sourceAddress, 0);
  EXPECT_EQ(message.pgn, 0);
  EXPECT_EQ(message.pduSpecific, 0);
  EXPECT_EQ(message.pduFormat, 0);
  EXPECT_EQ(message.priority, 0);
}

TEST(J1939Message, getSourceAddress_EdgeCase) {
  message = J1939Message();
  uint32_t canId = 0xFFFFFFFF;  // All bits set
  uint8_t result = message.getSourceAddress(canId);

  EXPECT_EQ(result, 0xFF);  // Expecting the last 8 bits
}

TEST(J1939Message, getPgn_EdgeCase) {
  message = J1939Message();
  uint32_t canId = 0xFFFFFFFF;  // All bits set
  uint16_t result = message.getPgn(canId);

  EXPECT_EQ(result, 0xFFFF);  // Expecting the middle 16 bits
}

TEST(J1939Message, getPduSpecific_EdgeCase) {
  message = J1939Message();
  uint32_t canId = 0xFFFFFFFF;  // All bits set
  uint8_t result = message.getPduSpecific(canId);

  EXPECT_EQ(result, 0xFF);  // Expecting the 8 bits starting from bit 8
}

TEST(J1939Message, getPduFormat_EdgeCase) {
  message = J1939Message();
  uint32_t canId = 0xFFFFFFFF;  // All bits set
  uint8_t result = message.getPduFormat(canId);

  EXPECT_EQ(result, 0xFF);  // Expecting the 8 bits starting from bit 16
}

TEST(J1939Message, getPriority_EdgeCase) {
  message = J1939Message();
  uint32_t canId = 0xFFFFFFFF;  // All bits set
  uint8_t result = message.getPriority(canId);

  EXPECT_EQ(result, 0x07);  // Expecting the 3 bits starting from bit 26
}

TEST(J1939Message, setSourceAddress_UpdateCanId) {
  message = J1939Message();
  uint32_t initialCanId = 0x1FFFFF00;
  message.setCanId(initialCanId);
  message.setSourceAddress(0xAA);

  EXPECT_EQ(message.sourceAddress, 0xAA);
  EXPECT_EQ(message.canId & 0xFF, 0xAA);
}

TEST(J1939Message, setPduSpecific_UpdateCanId) {
  message = J1939Message();
  uint32_t initialCanId = 0x1FF000FF;
  message.setCanId(initialCanId);
  message.setPduSpecific(0xAA);

  EXPECT_EQ(message.pduSpecific, 0xAA);
  EXPECT_EQ((message.canId >> 8) & 0xFF, 0xAA);
}

TEST(J1939Message, setPduFormat_UpdateCanId) {
  message = J1939Message();
  uint32_t initialCanId = 0x1F00FFFF;
  message.setCanId(initialCanId);
  message.setPduFormat(0xAA);

  EXPECT_EQ(message.pduFormat, 0xAA);
  EXPECT_EQ(message.getPduFormat(message.canId), 0xAA);
}

TEST(J1939Message, setPriority_UpdateCanId) {
  message = J1939Message();
  uint32_t initialCanId = 0xE000000;
  message.setCanId(initialCanId);
  message.setPriority(0x7);

  EXPECT_EQ(message.priority, 0x7);
  EXPECT_EQ((message.canId >> 26) & 0x7, 0x7);
}

TEST(J1939Message, CopyConstructor) {
  message = J1939Message();
  message.setCanId(canId);
  J1939Message copiedMessage = message;

  EXPECT_EQ(copiedMessage.canId, message.canId);
  EXPECT_EQ(copiedMessage.sourceAddress, message.sourceAddress);
  EXPECT_EQ(copiedMessage.pgn, message.pgn);
  EXPECT_EQ(copiedMessage.pduSpecific, message.pduSpecific);
  EXPECT_EQ(copiedMessage.pduFormat, message.pduFormat);
  EXPECT_EQ(copiedMessage.priority, message.priority);
}

TEST(J1939Message, AssignmentOperator) {
  message = J1939Message();
  message.setCanId(canId);
  J1939Message assignedMessage;
  assignedMessage = message;

  EXPECT_EQ(assignedMessage.canId, message.canId);
  EXPECT_EQ(assignedMessage.sourceAddress, message.sourceAddress);
  EXPECT_EQ(assignedMessage.pgn, message.pgn);
  EXPECT_EQ(assignedMessage.pduSpecific, message.pduSpecific);
  EXPECT_EQ(assignedMessage.pduFormat, message.pduFormat);
  EXPECT_EQ(assignedMessage.priority, message.priority);
}

TEST(J1939Message, ModifyFieldIntegrity) {
  message = J1939Message();
  message.setCanId(canId);

  uint8_t originalPduSpecific = message.pduSpecific;
  message.setPriority(6);

  EXPECT_EQ(message.pduSpecific,
               originalPduSpecific);  // Ensure other fields remain unchanged
}
