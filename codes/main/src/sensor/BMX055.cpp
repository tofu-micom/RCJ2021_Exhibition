#include "BMX055.h"

BMX055::BMX055(PinName sda, PinName scl) : i2c(sda, scl){};
int BMX055::init() {
    int err = 0;
    err += write(ADDR_ACC, 0x0F, 0x03);  // Range = +/- 2g
    err += write(ADDR_ACC, 0x10, 0x08);  // Bandwidth = 7.81 Hz
    err += write(ADDR_ACC, 0x11, 0x00);  // Normal mode, Sleep duration = 0.5ms

    err += write(ADDR_GYR, 0x0F, 0x04);  // Full scale = +/- 125 degree/s
    err += write(ADDR_GYR, 0x10, 0x07);  // ODR = 100 Hz
    err += write(ADDR_GYR, 0x11, 0x00);  // Normal mode, Sleep duration = 2ms

    /*err +=*/write(ADDR_MAG, 0x4B, 0x83);  // Soft reset
    /*err +=*/write(ADDR_MAG, 0x4B, 0x01);  // Soft reset
    err += write(ADDR_MAG, 0x4C, 0x00);     // Normal Mode, ODR = 10 Hz
    err += write(ADDR_MAG, 0x4E, 0x84);     // X, Y, Z-Axis enabled
    err += write(ADDR_MAG, 0x51, 0x04);  // No. of Repetitions for X-Y Axis = 9
    err += write(ADDR_MAG, 0x52, 0x16);  // No. of Repetitions for Z-Axis = 15
    return err;
}

void BMX055::update() {
    getAcc();
    getGyr();
    getMag();
}

void BMX055::calibrate(int numOfSampling) {
    float residual, dx, dy;
    for (int i = 0; i < numOfSampling; i++) {
        update();
        dx = xMag_ - bias[0];
        dy = yMag_ - bias[1];
        residual = dx * dx + dy * dy - bias[2] * bias[2];
        bias[0] += 4 * leaningRate * residual * dx;
        bias[1] += 4 * leaningRate * residual * dy;
        bias[2] += 4 * leaningRate * residual * bias[2];
        ThisThread::sleep_for(5ms);
    }
    printf("calibrated. bx=%d by=%d r=%d\n", int(bias[0]), int(bias[1]),
           int(bias[2]));
}

float BMX055::xAcc() { return xAcc_; }
float BMX055::yAcc() { return yAcc_; }
float BMX055::zAcc() { return zAcc_; }

float BMX055::xGyr() { return xGyr_; }
float BMX055::yGyr() { return yGyr_; }
float BMX055::zGyr() { return zGyr_; }

int BMX055::xMag() { return xMag_; }
int BMX055::yMag() { return yMag_; }
int BMX055::zMag() { return zMag_; }

void BMX055::getAcc() {
    char data[6];
    read(ADDR_ACC, REG_ACC, data, 6);
    // Convert the data to 12-bits
    xAcc_ = ((data[1] * 256) + (data[0] & 0xF0)) / 16;
    if (xAcc_ > 2047) xAcc_ -= 4096;
    yAcc_ = ((data[3] * 256) + (data[2] & 0xF0)) / 16;
    if (yAcc_ > 2047) yAcc_ -= 4096;
    zAcc_ = ((data[5] * 256) + (data[4] & 0xF0)) / 16;
    if (zAcc_ > 2047) zAcc_ -= 4096;
    xAcc_ = xAcc_ * 0.0098;  // renge +-2g
    yAcc_ = yAcc_ * 0.0098;  // renge +-2g
    zAcc_ = zAcc_ * 0.0098;  // renge +-2g
}

void BMX055::getGyr() {
    char data[6];
    read(ADDR_GYR, REG_GYR, data, 6);
    // Convert the data
    xGyr_ = (data[1] * 256) + data[0];
    if (xGyr_ > 32767) xGyr_ -= 65536;
    yGyr_ = (data[3] * 256) + data[2];
    if (yGyr_ > 32767) yGyr_ -= 65536;
    zGyr_ = (data[5] * 256) + data[4];
    if (zGyr_ > 32767) zGyr_ -= 65536;

    xGyr_ = xGyr_ * 0.0038;  //  Full scale = +/- 125 degree/s
    yGyr_ = yGyr_ * 0.0038;  //  Full scale = +/- 125 degree/s
    zGyr_ = zGyr_ * 0.0038;  //  Full scale = +/- 125 degree/s
}

void BMX055::getMag() {
    char data[8];
    read(ADDR_MAG, REG_MAG, data, 8);
    // Convert the data
    xMag_ = ((data[1] << 5) | (data[0] >> 3));
    if (xMag_ > 4095) xMag_ -= 8192;
    yMag_ = ((data[3] << 5) | (data[2] >> 3));
    if (yMag_ > 4095) yMag_ -= 8192;
    zMag_ = ((data[5] << 7) | (data[4] >> 1));
    if (zMag_ > 16383) zMag_ -= 32768;
    xMag_ -= bias[0];
    yMag_ -= bias[1];
}

bool BMX055::read(int addr, uint8_t reg, char *data, int len) {
    int err = 0;
    err += i2c.write(addr, (char *)&reg, 1, true);
    err += i2c.read(addr, data, len, false);
    if (err) printf("error: failed to read in addr 0x%x reg 0x%x\n", addr, reg);

    return err != 0;
}

bool BMX055::write(int addr, uint8_t reg, uint8_t data) {
    int err = 0;
    char cmd[2];
    cmd[0] = reg;
    cmd[1] = data;
    err += i2c.write(addr, cmd, 2, 0);
    if (err)
        printf("error: failed to write in addr 0x%x reg 0x%x data 0x%x\n", addr,
               reg, data);

    return err != 0;
}
