#pragma once
#include <mbed.h>

class BMX055 {
    public:
    BMX055(PinName sda, PinName scl);
    int init();
    void update();
    void calibrate(int numOfSampling);
    float xAcc();
    float yAcc();
    float zAcc();
    float xGyr();
    float yGyr();
    float zGyr();
    int xMag();
    int yMag();
    int zMag();

    private:
    I2C i2c;

    const int ADDR_ACC = 0x19 << 1;
    const int ADDR_GYR = 0x69 << 1;
    const int ADDR_MAG = 0x13 << 1;

    const int REG_ACC = 0x02;
    const int REG_GYR = 0x02;
    const int REG_MAG = 0x42;

    float bias[3] = {0, 0, 1};
    const float leaningRate = 0.00002;

    float xAcc_ = 0.00;
    float yAcc_ = 0.00;
    float zAcc_ = 0.00;
    float xGyr_ = 0.00;
    float yGyr_ = 0.00;
    float zGyr_ = 0.00;
    int xMag_ = 0;
    int yMag_ = 0;
    int zMag_ = 0;

    void getAcc();
    void getGyr();
    void getMag();

    bool read(int addr, uint8_t reg, char *data, int len);
    bool write(int addr, uint8_t reg, uint8_t data);
};
