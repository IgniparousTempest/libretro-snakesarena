#ifndef LR_SUPERFLAPPYBIRD_INPUT_H
#define LR_SUPERFLAPPYBIRD_INPUT_H

struct Input {
    bool accept;
    bool back;
    bool right;
    bool down;
    bool left;
    bool up;
    bool start;

    bool accept_pressed;
    bool back_pressed;
    bool right_pressed;
    bool down_pressed;
    bool left_pressed;
    bool up_pressed;
    bool start_pressed;

    bool accept_released;
    bool back_released;
    bool right_released;
    bool down_released;
    bool left_released;
    bool up_released;
    bool start_released;
};

#endif //LR_SUPERFLAPPYBIRD_INPUT_H
