#include "rescue.h"

void rescue() {
    //入った直後の処理
    turn_left();    //短辺方向に進むために方向転換
    for (int i = 0; i < 4; i++) {  // ジグザグ移動を4回繰り返す
        arm_down();
        move_ncm(60);  //短辺の長さ-機体長ぶん進む
        arm_up();
        if (i % 2 == 0)  //曲がるたびに右折/左折が入れ替わる
            turn_right();
        else
            turn_left();
        move_ncm(30);  //避難ゾーン側に30cm進む
        if (i % 2 == 0)
            turn_right();
        else
            turn_left();
    }
    //ここで避難ゾーンの前にいるはず
    turn_right(135);  //避難ゾーンに向く
    stopper_up();     //ボール放出
    stopper_down();
    move_motor(1, 1);
    turn_right(45);  // 45+135=180度回りUターン
    while (!is_green_line());  //救助ゾーンから抜ける
    move_motor(1, 1);
    move_motor(0, 0);
}

void turn_left(int deg) {
    move_motor(1, -1);
    wait_us(time_turn_45deg * deg * 1000 / 45);
    move_motor(0, 0);
}
void turn_left() { turn_left(90); }

void turn_right(int deg) {
    move_motor(-1, 1);
    wait_us(time_turn_45deg * deg * 1000 / 45);
    move_motor(0, 0);
}
void turn_right() { turn_right(90); }

void move_ncm(int cm) {
    move_motor(1, 1);
    wait_us(time_move_1cm * cm * 1000);
    move_motor(0,0);
}
void arm_up() {
    servo(ArmUp);
    wait_us(1000*time_arm_up);
}
void arm_down() {
    servo(ArmDown);
    wait_us(1000*time_arm_down);
}
void stopper_up() {
    servo(StopperUp);
    wait_us(1000*time_stopper_up);
}
void stopper_down() {
    servo(StopperDown);
    wait_us(1000*time_stopper_down);
}
