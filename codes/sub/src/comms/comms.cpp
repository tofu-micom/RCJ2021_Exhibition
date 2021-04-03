//
// Created by 平地浩一 on 2021/03/23.
//
#include "comms.h"

DigitalIn a(PA_7);
DigitalIn b(PA_6);

DigitalOut RA(PB_0);
DigitalOut RB(PA_12);
DigitalOut LA(PB_3);
// PB_7 許さん！！！
DigitalOut LB(PB_6);

BufferedSerial pc(USBTX, USBRX);

void comms_init() {
  RA.write(0);
  RB.write(0);
  LA.write(0);
  LB.write(0);
}

void comms_read() {
  bool A = a.read();
  bool B = b.read();
  static bool AA;
  static bool BB;
  if (A != AA || B != BB) {
    if (A && !B) {
      // 左超信地
      RA.write(0);
      RB.write(1);
      LA.write(1);
      LB.write(0);
    } else if (!A && B) {
      // 右超信地
      RA.write(1);
      RB.write(0);
      LA.write(0);
      LB.write(1);
    } else if (!A && !B) {
      // 停止
      RA.write(1);
      RB.write(1);
      LA.write(1);
      LB.write(1);
    } else if (A && B) {
      // 前進
      RA.write(0);
      RB.write(1);
      LA.write(0);
      LB.write(1);
    }
  }
  AA = A;
  BB = B;
}