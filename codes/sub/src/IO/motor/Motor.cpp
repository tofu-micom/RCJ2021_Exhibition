#include "Motor.h"
/*-------------------*/
/*    class Motor    */
/*-------------------*/

Motor::Motor(PinName pin0, PinName pin1) : out0(pin0), out1(pin1) {
  pin[0] = pin0;
  pin[1] = pin1;
  out0 = 0;
  out1 = 0;
}

int Motor::move(int power) {
  if (power == 2) {
    out0 = 1;
    out1 = 1;
  } else if (power == 0) {
    out0 = 0;
    out1 = 0;
  } else if (power > 0) {
    out0 = 1;
    out1 = 0;
  } else {
    out0 = 0;
    out1 = 1;
  }
  return 0;
}

void Motor::stop() {
  out0 = 1;
  out1 = 1;
}

/*--------------------*/
/*     class Drive    */
/*--------------------*/
Drive::Drive(PinName pinL0, PinName pinL1, PinName pinR0, PinName pinR1) {
  motorL = new Motor(pinL0, pinL1);
  motorL->move(0);
  motorR = new Motor(pinR0, pinR1);
  motorR->move(0);
  pinL[0] = pinL0;
  pinL[1] = pinL1;
  pinR[0] = pinR0;
  pinR[1] = pinR1;
}

void Drive::run(int powerL, int powerR) {
  motorL->move(powerL);
  motorR->move(powerR);
}
void Drive::stop() {
  motorL->stop();
  motorR->stop();
}
