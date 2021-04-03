//
// Created by 平地浩一 on 2021/03/22.
//
#include "comms.h"

SPI spi(PB_5, PB_4, PB_3);  // mosi, miso, sclk
DigitalOut cs(PA_11);

void comms_init() {
  // 初期化処理
}

void move_motor(int r, int l) {
  int command = 0xf0;
  if (r == 0 && l == 0) {
    command += 0x1;
  } else if (r == 0 && l == 1) {
    command += 0x2;
  } else if (r == 1 && l == 0) {
    command += 0x3;
  } else if (r == 1 && l == 1) {
    command += 0x4;
  } else if (r == -1 && l == 0) {
    command += 0x5;
  } else if (r == 0 && l == -1) {
    command += 0x6;
  } else if (r == 1 && l == -1) {
    command += 0x7;
  } else if (r == -1 && l == 1) {
    command += 0x8;
  } else if (r == -1 && l == -1) {
    command += 0x9;
  } else if (r == 2 && l == 2) {
    command += 0xa;
  }
  cs = 0;
  spi.write(command);
  cs = 1;
}

void servo(enum ServoMode mode) {
    cs = 0;
    spi.write(0xfb + mode);
    cs = 1;
}
