#include "Servo.h"

#define SG_PERIOD 20
#define SG_NEUTRAL 1450  //初期角度:90度

void Servo::init() {
  period_ms(SG_PERIOD);
  pulsewidth_us(SG_NEUTRAL);
}

int Servo::write(unsigned int angle) {
  int pulseWidth = 0;
  if (angle < 90) {
    pulseWidth = (90 - angle) * 10;
    pulseWidth = SG_NEUTRAL - pulseWidth;
  } else if (angle <= 180) {
    pulseWidth = (angle - 90) * 10;
    pulseWidth = SG_NEUTRAL + pulseWidth;
  } else {
    return -1;
  }
  pulsewidth_us(pulseWidth);
  return 0;
}

int Servo::roll_write(unsigned int speed){
int pulseWidth = 0;
if(speed == 90){
    write(0);
}else if (speed< 90) {
    pulseWidth = (90 - speed) * 10;
    pulseWidth = SG_NEUTRAL - pulseWidth;
  } else if (speed <= 180) {
    pulseWidth = (speed - 90) * 10;
    pulseWidth = SG_NEUTRAL + pulseWidth;
  } else {
    return -1;
  }
  pulsewidth_us(pulseWidth);
  return 0;
}