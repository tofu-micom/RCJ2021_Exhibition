#include "lineTrace.h"

#include "PinNames.h"
#include "ThisThread.h"
#include "comms/comms.h"
#include "estimation/pose.h"
#include "mbed_wait_api.h"
#include "sensor/BMX055.h"
#include "sensor/GP2Y0E03.h"

AnalogIn lineL(PA_1);
AnalogIn lineR(PA_4);
AnalogIn cornerL(PA_0);
AnalogIn cornerR(PA_5);
AnalogIn colorL(PA_7);
AnalogIn colorR(PA_6);
AnalogIn center(PA_3);
DigitalOut greenLED(PF_0);
DigitalOut redLED(PB_1);

GP2Y0E03 disFront(I2C_SDA, I2C_SCL, 0x50 << 1);
GP2Y0E03 disBack(I2C_SDA, I2C_SCL, 0x40 << 1);
GP2Y0E03 disRight(I2C_SDA, I2C_SCL, 0x48 << 1);

// BMX055* nine = new BMX055(I2C_SDA, I2C_SCL);
// pose Pose(nine, 1.5);

const float g_silver = 0.80;
const float g_white = 0.63;
const float g_black = 0.10;
const float g_sw = (g_silver + g_white) / 2;          //銀・白の閾値
const float g_wb = (1 * g_white + 2 * g_black) / 3;       //白・黒の閾値
const float gc_wb = (2 * g_white + 3 * g_black) / 5;  //直角検知の閾値

const float r_silver = 1.0;
const float r_white = 0.18;
const float r_black = 0.02;
const float r_sw = (r_silver + r_white) / 2;     //銀・白の閾値
const float r_wb = (2 * r_white + r_black) / 3;  //白・黒の閾値


void lineTest() {
  greenLED = 1;
  redLED = 0;
  float photo_colorL = colorL.read();
  float photo_colorR = colorR.read();
  printf("%d,%d\n", (int)(photo_colorL * 100.0f), (int)(photo_colorR * 100.0f));
  wait_us(1 * 1000);
}

void lineInit() {  //ライントレース初期化用の関数
  // nine->init();
  // Pose.setPeriod(0.01);
  // while (true) {
  //   float disFronstInput = disFront.read();  // Back
  //   float disBackInput = disBack.read();     // Front
  //   float disRightInput = disRight.read();   // Right
  //   printf("%d, %d, %d\n", (int)disFronstInput, (int)disBackInput,
  //          (int)disRightInput);
  // }
  move_motor(1, 1);
}

void lineTrace() {
  // Pose.update();
  // printf("%d\n", Pose.degree());

  greenLED = 1;
  redLED = 0;
  wait_us(1 * 1000);
  float photo_center = center.read();
  float photo_lineL = lineL.read();
  float photo_lineR = lineR.read();
  float photo_cornerL = cornerL.read();
  float photo_cornerR = cornerR.read();
  float photo_colorL = colorL.read();
  float photo_colorR = colorR.read();

  printf("%d,%d,%d,%d,%d \n", (int)(photo_cornerL * 100.0f),
         (int)(photo_lineL * 100.0f), (int)(photo_lineR * 100.0f),
         (int)(photo_cornerR * 100.0f), (int)(gc_wb * 100));

  float disFronstInput = disFront.read();
  float disBackInput = disFront.read();
  float disRightInput = disFront.read();
  printf("%d, %d, %d\n", (int)disFronstInput, (int)disBackInput,
         (int)disRightInput);

  if (false){//disRightInput <= 4.0 &&
//      !(disRightInput == -63 || disRightInput == -1.0)) {  //障害物検知
    const int forwardTime = 3000;
    move_motor(2, 2);
    move_motor(-1, 1);
    wait_us(2000 * 1000);
    move_motor(2, 2);
    move_motor(1, 1);
    wait_us(forwardTime * 1000);
    move_motor(2, 2);
    move_motor(1, -1);
    wait_us(2000 * 1000);
    move_motor(2, 2);
    move_motor(1, 1);
    wait_us(forwardTime * 1000);
    move_motor(2, 2);
    move_motor(1, -1);
    wait_us(2000 * 1000);
    move_motor(2, 2);
    move_motor(1, 1);
    wait_us(forwardTime * 1000);
    move_motor(2, 2);
    move_motor(-1, 1);
    wait_us(2000 * 1000);
    //障害物よけ
  } else if (photo_cornerL <= gc_wb || photo_cornerR <= gc_wb) {  //直角検知
    move_motor(2, 2);                                             // stop
    wait_us(1000 * 1000);
    //マーカー検知開始
    bool isGreen[2] = {0};
    bool isRed[2] = {0};  // 0:L, 1:R
    greenLED = 1;
    redLED = 0;
    wait_us(100 * 1000);
    float g_colorL = colorL.read();
    float g_colorR = colorR.read();
    greenLED = 0;
    redLED = 1;
    wait_us(100 * 1000);
    float r_colorL = colorL.read();
    float r_colorR = colorR.read();
    greenLED = 1;
    redLED = 0;
    // wait_us(100 * 1000);

    if (g_colorL > g_wb) {
      if (r_colorL >= r_wb) {
        // white
      } else {
        // green
        isGreen[0] = true;
      }
    } else {
      if (r_colorL >= r_wb) {
        // red
        isRed[0] = true;
      } else {
        // black
      }
    }

    if (g_colorR > g_wb) {
      if (r_colorR > r_wb) {
        // white
      } else {
        // green
        isGreen[1] = true;
      }
    } else {
      if (r_colorR > r_wb) {
        // red
        isRed[1] = true;
      } else {
        // black
      }
    }
    /*printf("%d,%d,%d,%d\n",(int)(g_colorL*100),(int)(g_colorR*100),
    (int)(r_colorL*100),(int)(r_colorR*100));
    printf("%d,%d\n",(int)(g_wb*100),(int)(r_wb*100));
    printf("%d,%d,%d,%d\n",(int)isGreen[0],(int)isGreen[1],(int)isRed[0],(int)isRed[1]);
    while(true);*/
    //マーカー検知終了
    if (isRed[0] || isRed[1]) {
      // the end of line. stop
		printf("red line\n");
		//while (true);
		move_motor(1, 1);
    } else if (isGreen[0] && isGreen[1]) {
      // go back
      move_motor(1, 1); wait_us(500 *1000); move_motor(2, 2);
	  move_motor(1, -1); wait_us(3000 *1000); move_motor(2, 2);
      while (center.read() > 0.50) {
        printf("%d\n",(int)(center.read() * 100));
	    move_motor(1, -1);
	  }
	  move_motor(1, 1); wait_us(250 *1000); move_motor(2, 2);
      move_motor(2, 2);
    } else if (isGreen[0]) {
      // turn left
      move_motor(1, 1); wait_us(350 *1000); move_motor(2, 2);
	  move_motor(1, -1); wait_us(750 *1000); move_motor(2, 2);
	  while (center.read() > 0.50) {
		//printf("%d\n",(int)(center.read() * 100));
		printf("left\n");
		move_motor(1,-1);
	  }
	  move_motor(2, 2);

    } else if (isGreen[1]) {
      // turn right
      move_motor(1, 1); wait_us(350 *1000); move_motor(2, 2);
	  move_motor(-1, 1); wait_us(750 *1000); move_motor(2, 2);
	  while (center.read() > 0.50) {
	    //printf("%d\n",(int)(center.read() * 100));
	    printf("right\n");
	    move_motor(-1,1);
	  }
	  move_motor(2, 2);
    } else {
      //マークなし直角
      float g_center = photo_center;    // center.read();
      float g_cornerL = photo_cornerL;  // cornerL.read();
      float g_cornerR = photo_cornerR;  // cornerR.read();
      /*while(true){
                      move_motor(2, 2);
                      printf("%d,%d,%d,%d\n",(int)(g_cornerL*100),(int)(g_center
      * 100),(int)(g_cornerR*100),(int)(gc_wb*100)); wait_us(10*1000);
      }*/
      if (g_center <= 0.60) {
        //マークなし十字→直進
        /*while(true){
                move_motor(2, 2);
                printf("%d,%d,%d,%d\n",(int)(g_cornerL*100),(int)(g_center *
        100),(int)(g_cornerR*100),(int)(gc_wb*100)); wait_us(10*1000);
        }*/
        move_motor(1, 1);
        wait_us(500 * 1000);
        move_motor(2, 2);
      } else {  //丁字or通常直角
        if (g_cornerL <= gc_wb && g_cornerR > gc_wb) {
          // turn right
		  move_motor(1, 1); wait_us(350 *1000); move_motor(2, 2);
		  move_motor(1, -1); wait_us(750 *1000); move_motor(2, 2);
		  while (center.read() > 0.50) {
		    printf("%d\n",(int)(center.read() * 100));
		    move_motor(1,-1);
		  }
          move_motor(2, 2);
        } else if (g_cornerL > gc_wb && g_cornerR <= gc_wb) {
          // turn left
		  move_motor(1, 1); wait_us(350 *1000); move_motor(2, 2);
		  move_motor(-1, 1); wait_us(750 *1000); move_motor(2, 2);
		  while (center.read() > 0.50) {
		    printf("%d\n",(int)(center.read() * 100));
			move_motor(-1,1);
		  }
          move_motor(2, 2);
        } else {
          // stay (cannot move)
          move_motor(2, 2);
          printf("Cannot move\n");
        }
      }
    }
  } else if (false) {  //ギャップ検知
    move_motor(0, 0);
    wait_us(1000000);
    //ギャップ対処 or 銀検知
    while (lineL.read() > g_wb && lineR.read() > g_wb && center.read() > g_wb) {
      move_motor(1, 1);
    }
  } else {
    //通常ライントレース
    if (photo_lineL > g_wb && photo_lineR > g_wb) {
      move_motor(1, 1);
    } else if (photo_lineL <= g_wb && photo_lineR > g_wb) {
      move_motor(1, 1);
      wait_us(5 * 1000);
      move_motor(1, -1);
    } else if (photo_lineL > g_wb && photo_lineR <= g_wb) {
      move_motor(1, 1);
      wait_us(5 * 1000);
      move_motor(-1, 1);
    } else {
      move_motor(1, 1);
      wait_us(5 * 1000);
      move_motor(1, 1);
    }
    // wait_us(1*1000);
    // printf("%d,%d\n",(int)(photo_lineL*100), (int)(photo_lineR*100));
  }
}

bool is_green_line() {
    greenLED = 0;
    redLED = 1;
    wait_us(1000);
    float g_colorL = colorL.read();
    float g_colorR = colorR.read();
    greenLED = 0;
    redLED = 0;
    return (g_colorL < g_wb || g_colorR < g_wb);
}
