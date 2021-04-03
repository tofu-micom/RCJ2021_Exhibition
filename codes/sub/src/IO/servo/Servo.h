#ifndef SERVO_H
#define SERVO_H
#include "mbed.h"

class Servo : public PwmOut {
 public:
  /* コンストラクタ */
  Servo(PinName port) : PwmOut(port) {}
  /* 初期化処理 */
  void init();
  /* 回転処理 */
  int write(unsigned int angle);
  /* 無限サーボ回転 */
  int roll_write(unsigned int speed);
};

#endif