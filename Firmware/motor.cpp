#include "mbed.h"
#include "motor.h"
#include "pinout.h"
#include "MicroBit.h"
#include "MicroBitPin.h"

// NOTE:
// 1)   Depending on the wiring of your motors, you may have to shuffle the
//      contents of the forward() reverse() and misc turn functions around
// 2)   The left and right throttle multiplier will have to be calibrated
//      for the bot to be able to move in a straight line over longer 
//      distances. 
// 3)   There are issues with PWM motor control in the current version. Using PWM 
//      seems to cause random crashes after a while (typically one to two minutes)
//      It is currently unclear if this is an issue with the nrf51 PWM driver for
//      the arduino framework, or if it is a result of a layout issue in the PCB


MicroBitPin left(1, LEFT_SPEED, PIN_CAPABILITY_AD);
MicroBitPin right(2, RIGHT_SPEED, PIN_CAPABILITY_AD);

Motor::Motor(IOExpander * io) :
    _io(io),
    left_throttle_multiplier(1), 
    right_throttle_multiplier(1)
{
    left.setAnalogPeriodUs(10);
    left.setAnalogValue(500);
    right.setAnalogPeriodUs(10);
    right.setAnalogValue(500);
}

void Motor::test()
{
    left.setAnalogPeriod(1);
    left.setAnalogValue(500);
    right.setAnalogPeriod(1);
    right.setAnalogValue(500);
}

void Motor::forward()
{
    _io->set_value(LEFT_MOTOR_INPUT1, LOW);
    _io->set_value(LEFT_MOTOR_INPUT2, HIGH);
    _io->set_value(RIGHT_MOTOR_INPUT1, HIGH);
    _io->set_value(RIGHT_MOTOR_INPUT2, LOW);
}

void Motor::reverse() 
{
  _io->set_value(LEFT_MOTOR_INPUT1, HIGH);
  _io->set_value(LEFT_MOTOR_INPUT2, LOW);
  _io->set_value(RIGHT_MOTOR_INPUT1, LOW);
  _io->set_value(RIGHT_MOTOR_INPUT2, HIGH);
}

void Motor::turn_right()
{
  _io->set_value(LEFT_MOTOR_INPUT1, HIGH);
  _io->set_value(LEFT_MOTOR_INPUT2, LOW);
  _io->set_value(RIGHT_MOTOR_INPUT1, HIGH);
  _io->set_value(RIGHT_MOTOR_INPUT2, LOW);
}

void Motor::turn_left()
{
  _io->set_value(LEFT_MOTOR_INPUT1, LOW);
  _io->set_value(LEFT_MOTOR_INPUT2, HIGH);
  _io->set_value(RIGHT_MOTOR_INPUT1, LOW);
  _io->set_value(RIGHT_MOTOR_INPUT2, HIGH);
}

// SetSpeed changes the PWM duty cycle.
// Range 0-1023
void Motor::setSpeed(int speed)
{
    if (speed < 0)
        speed = 0;
    if (speed > 1023)
        speed = 1023;


    left.setAnalogValue((int)(speed*left_throttle_multiplier));
    right.setAnalogValue((int)(speed*right_throttle_multiplier));
}

