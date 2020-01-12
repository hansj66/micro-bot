#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "IOExpander.h"

class Motor
{
    public: 
        Motor(IOExpander * io);
        void set_throttle(int value);
        void begin();
        void forward();
        void reverse();
        void stop();
        void run(uint8_t speed);
        void turn_left();
        void turn_right();

    private:
        IOExpander * _io;
        int left_throttle_multiplier;
        int right_throttle_multiplier;
};

#endif