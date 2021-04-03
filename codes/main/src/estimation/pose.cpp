#include "pose.h"

pose::pose(BMX055 *nine, float correction) : correction(correction) {
    this->nine = nine;
}
float pose::period() { return period_; }
void pose::setPeriod(float per) { period_ = per; }
void pose::init() { nine->init(); }
void pose::calibrate(int numOfSampling) { nine->calibrate(numOfSampling); }
int pose::degree() { return int(degree_); }
void pose::setDegree(int deg) { degree_ = deg; }
int pose::xPosition() { return xPosition_; }
int pose::yPosition() { return yPosition_; }
void pose::setPosition(int x, int y) {
    xPosition_ = x;
    yPosition_ = y;
}

void pose::update() {
    nine->update();
    updatePosition();
    updateDegree();
}

void pose::updatePosition() {}
void pose::updateDegree() {
    // xMag_ = nine->xMag();
    // yMag_ = nine->yMag();
    // degree_ = atan(float(xMag_) / float(yMag_)) * 180 / 3.1415;
    gyr_ = nine->zGyr();
    degree_ += (gyr_ + pGyr_) * period_ / 2.0;
    pGyr_ = gyr_;
}
