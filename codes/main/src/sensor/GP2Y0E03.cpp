#include "GP2Y0E03.h"

const int GP2Y0E03::REG_DIST = 0x5E;

GP2Y0E03::GP2Y0E03(PinName sda, PinName scl, int addr)
    : i2c(sda, scl), ADDR(addr) {}

float GP2Y0E03::read() {
    char data[3];
    int err = 0;
    i2c.start();
    err += i2c.write(ADDR, (char*)&REG_DIST, 1, true);
    ThisThread::sleep_for(1ms);
    err += i2c.read(ADDR, data, 2);
    i2c.stop();
    return (err ? -err : calcDist(data));
}

int GP2Y0E03::calcDist(char* data) {
    return ((data[0] * 16 + data[1]) / 16) / (2 * 2);
}
