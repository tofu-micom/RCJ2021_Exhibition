#pragma once
#include <mbed.h>

class GP2Y0E03 {
    public:
    GP2Y0E03(PinName sda, PinName scl, int addr);  // 8bit address!!
    float read();

    private:
    I2C i2c;
    const int ADDR;
    static const int REG_DIST;
    int calcDist(char* data);
};
