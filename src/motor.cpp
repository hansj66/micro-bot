#include <Arduino.h>
#include <Wire.h>
#include "motor.h"
#include "pinout.h"

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

// #define PWM_MOTOR_CONTROL

Motor::Motor(IOExpander * io) :
    _io(io),
    left_throttle_multiplier(1), 
    right_throttle_multiplier(1) 
{
}

void Motor::begin()
{
    pinMode(LEFT_SPEED, OUTPUT);
    pinMode(RIGHT_SPEED, OUTPUT);

    forward();
    run(75);
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

void Motor::run(uint8_t speed)
{
#ifdef PWM_MOTOR_CONTROL  
  if (speed < 75)
    speed = 75;
  if (speed > 255)
    speed = 255;
  analogWrite(LEFT_SPEED, speed * left_throttle_multiplier);
  analogWrite(RIGHT_SPEED, speed * right_throttle_multiplier);
#else
  digitalWrite(LEFT_SPEED, speed);
  digitalWrite(RIGHT_SPEED, speed);
#endif
}

void Motor::stop()
{
#ifdef PWM_MOTOR_CONTROL  
  analogWrite(LEFT_SPEED, 0);
  analogWrite(RIGHT_SPEED, 0);
#else
  digitalWrite(LEFT_SPEED, 0);
  digitalWrite(RIGHT_SPEED, 0);
#endif    
}
