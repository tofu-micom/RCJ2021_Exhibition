#pragma once
#include <mbed.h>

#include "../comms/comms.h"
#include "../line/lineTrace.h"

const int time_move_1cm = 0;
const int time_turn_45deg = 0;
const int time_arm_up = 0;
const int time_arm_down = 2500;
const int time_stopper_up = 2500;
const int time_stopper_down = 0;

void turn_left(int deg);
void turn_left();

void turn_right(int deg);
void turn_right();

void move_ncm(int cm);

void arm_up();
void arm_down();
void stopper_up();
void stopper_down();
