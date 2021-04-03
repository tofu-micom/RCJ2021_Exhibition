//
// Created by 平地浩一 on 2021/03/22.
//

#ifndef MAIN_COMMS_H
#define MAIN_COMMS_H

#include <mbed.h>

#define SPI_SSEL PA_11

enum ServoMode { ArmUp, ArmDown, StopperUp, StopperDown };

void comms_init();

void move_motor(int r_speed, int l_speed);
void servo(enum ServoMode mode);

#endif  // MAIN_COMMS_H
