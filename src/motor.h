#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "mbed.h"
#include "IOExpander.h"


class Motor
{
    public: 
        Motor(IOExpander * io);
        void forward();
        void reverse();
        void setSpeed(int speed);
        void turn_left();
        void turn_right();

    private:
        IOExpander * _io;
        float left_throttle_multiplier;
        float right_throttle_multiplier;
};

#endif