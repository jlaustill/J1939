#include <J1939.h>
#include <gtest/gtest.h>

J1939 message;
uint32_t canId = 217056001;
uint16_t pgn = 61443;
uint8_t pduSpecific = 3;
uint8_t pduFormat = 240;
uint8_t priority = 3;

TEST(J1939, setCanId) {
  message = J1939();
  message.setCanId(canId);

  EXPECT_EQ(message.canId, canId);
  EXPECT_EQ(message.sourceAddress, 1);
  EXPECT_EQ(message.pgn, pgn);
  EXPECT_EQ(message.pduSpecific, pduSpecific);
  EXPECT_EQ(message.pduFormat, pduFormat);
  EXPECT_EQ(message.priority, priority);
}

TEST(J1939, getSourceAddress) {
    message = J1939();
    uint8_t result = message.getSourceAddress(0b100000000000010101010);

    EXPECT_EQ(result, 0b10101010);
}

TEST(J1939, getGgn) {
    message = J1939();
    uint16_t result = message.getPgn(canId);

    EXPECT_EQ(result, pgn);
}

TEST(J1939, getPduSpecific) {
    message = J1939();
    uint8_t result = message.getPduSpecific(canId);

    EXPECT_EQ(result, pduSpecific);
}

TEST(J1939, getPduFormat) {
    message = J1939();
    uint8_t result = message.getPduFormat(canId);

    EXPECT_EQ(result, pduFormat);
}

TEST(J1939, getPriority) {
    message = J1939();
    uint8_t result = message.getPriority(canId);

    EXPECT_EQ(result, priority);
}

TEST(J1939, setData) {
    message = J1939();
    uint8_t const data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    message.setData(data);

    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(message.data[i], data[i]);
    }
}