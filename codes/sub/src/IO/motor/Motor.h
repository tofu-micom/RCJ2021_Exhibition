#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"

class Motor {
 private:
  PinName pin[2];
  DigitalOut out0;
  DigitalOut out1;

 public:
  Motor(PinName, PinName);
  int move(int);
  void stop();
};

class Drive {
 private:
  PinName pinL[2];
  PinName pinR[2];
  Motor* motorL;
  Motor* motorR;

 public:
  Drive(PinName, PinName, PinName, PinName);
  void run(int, int);
  void stop();
};

#endif /* end of include guard */
