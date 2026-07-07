#include <J1939McuHealth.h>
#include <gtest/gtest.h>

// A fully-populated, all-valid health record.
static McuHealth makeFull() {
  McuHealth h;
  h.temperatureC = 60.0f;      // (60 + 273) * 32 = 10656 = 0x29A0
  h.resetCause = J1939McuHealth_RESET_WDOG;   // 4
  h.bootCount = 7;
  h.uptimeMinutes = 1000;
  h.temperatureValid = true;
  h.bootCountValid = true;
  h.uptimeValid = true;
  return h;
}

TEST(J1939McuHealth, pgnConstant) {
  EXPECT_EQ(J1939McuHealth_PGN, 0xFFDC);
  EXPECT_EQ(J1939McuHealth_TRANSMIT_INTERVAL_MS, 1000);
}

TEST(J1939McuHealth, encodeFullFrame) {
  McuHealth h = makeFull();
  uint8_t out[8] = {0};
  J1939McuHealth_encode(&h, out);

  // Temp 60C -> raw 10656 (0x29A0), little-endian.
  EXPECT_EQ(out[0], 0xA0);
  EXPECT_EQ(out[1], 0x29);
  // Reset cause wdog = 4.
  EXPECT_EQ(out[2], 4);
  // Boot count 7, little-endian.
  EXPECT_EQ(out[3], 0x07);
  EXPECT_EQ(out[4], 0x00);
  // Uptime 1000 min = 0x03E8, little-endian.
  EXPECT_EQ(out[5], 0xE8);
  EXPECT_EQ(out[6], 0x03);
  // Reserved.
  EXPECT_EQ(out[7], 0xFF);
}

TEST(J1939McuHealth, encodeSentinelsForInvalidFields) {
  McuHealth h = makeFull();
  h.temperatureValid = false;
  h.bootCountValid = false;
  h.uptimeValid = false;
  h.resetCause = J1939McuHealth_RESET_NOT_IMPLEMENTED;  // 0xFF
  uint8_t out[8] = {0};
  J1939McuHealth_encode(&h, out);

  EXPECT_EQ(out[0], 0xFF);
  EXPECT_EQ(out[1], 0xFF);
  EXPECT_EQ(out[2], 0xFF);
  EXPECT_EQ(out[3], 0xFF);
  EXPECT_EQ(out[4], 0xFF);
  EXPECT_EQ(out[5], 0xFF);
  EXPECT_EQ(out[6], 0xFF);
  EXPECT_EQ(out[7], 0xFF);
}
