/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "DigitalOut.h"
#include "IO/motor/Motor.h"
#include "ThisThread.h"
#include "mbed.h"
#include "IO/servo/Servo.h"

Motor R(PA_12, PB_0);
Motor L(PB_6, PB_3);

SPISlave device(PA_7, PA_6, PA_5, PA_4); // mosi, miso, sclk, ssel

Servo servoStopper(PB_1);
Servo servoFront(PA_8);
DigitalIn test(PA_11);
DigitalIn upSwitch(PB_5);

int main() {
  device.reply(0x00);
  servoStopper.write(30);
  servoFront.write(90);
test.mode(PullUp);

  while (1) {
      if(false){
          R.move(-1);
          L.move(-1);
          ThisThread::sleep_for(1s);
          R.move(1);
          L.move(-1);
          ThisThread::sleep_for(1s);
          R.move(1);
          L.move(1);
          ThisThread::sleep_for(1s);
          R.move(-1);
          L.move(1);
          ThisThread::sleep_for(1s);
           R.move(1);
          L.move(1);
          ThisThread::sleep_for(1s);
          R.move(-1);
          L.move(1);
          ThisThread::sleep_for(1s);
           R.move(1);
          L.move(1);
          ThisThread::sleep_for(1s);
          R.move(1);
          L.move(-1);
          ThisThread::sleep_for(1s);
          R.move(0);
          L.move(0);
      }  
    if (device.receive()) {
      int v = device.read();
      printf("%x\n", v);
      switch (v) {
        /*
          0xf1 :  0  0
          0xf2 :  0  1
          0xf3 :  1  0
          0xf4 :  1  1
          0xf5 : -1  0
          0xf6 :  0 -1
          0xf7 :  1 -1
          0xf8 : -1  1
          0xf9 : -1 -1
        */
      case 0xf1:
        R.move(0);
        L.move(0);
        break;
      case 0xf2:
        R.move(0);
        L.move(1);
        break;
      case 0xf3:
        R.move(1);
        L.move(0);
        break;
      case 0xf4:
        R.move(1);
        L.move(1);
        break;
      case 0xf5:
        R.move(-1);
        L.move(0);
        break;
      case 0xf6:
        R.move(0);
        L.move(-1);
        break;
      case 0xf7:
        R.move(1);
        L.move(-1);
        break;
      case 0xf8:
        R.move(-1);
        L.move(1);
        break;
      case 0xf9:
        R.move(-1);
        L.move(-1);
        break;
      case 0xfa:
        R.move(2);
        L.move(2);
        break;
      case 0xfb:
           servoFront.write(0);
      ThisThread::sleep_for(2500ms);
      servoFront.write(90);
        break;
      case 0xfc:
      servoFront.write(0);
      ThisThread::sleep_for(2500ms);
      servoFront.write(90);
        break;
      case 0xfd:
        servoStopper.write(90);
        break;
      case 0xfe:
        servoStopper.write(0);
        break;
      }
    }
  }
}