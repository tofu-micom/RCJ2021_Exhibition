#pragma once

#include <mbed.h>

#include "../sensor/BMX055.h"

/*
1. BMX055のインスタンスを作り，そこへのポインタを得る
2. それと補正係数をposeのコンストラクタに渡す
3. init()する
4. setPeriod(処理時間秒)する
5. 初期位置についたら，setDegree(0)する
---くりかえす---
6. update()する
7. (degree()で角度を取得できる)
---
*/

class pose {
    public:
    pose(BMX055 *nine, float correction);
    float period();
    void setPeriod(float per);  // second
    void setCorrection(float cor);
    void init();
    void update();
    void calibrate(int numOfSampling);
    int degree();
    void setDegree(int deg);
    int xPosition();
    int yPosition();
    void setPosition(int x, int y);

    private:
    BMX055 *nine;
    const float correction = 1;

    float period_ = 0.01;
    float degree_ = 0;
    int xPosition_ = 0;
    int yPosition_ = 0;

    float gyr_ = 0;
    float pGyr_ = 0;
    int xMag_ = 1;
    int yMag_ = 1;

    void updateDegree();
    void updatePosition();
};
