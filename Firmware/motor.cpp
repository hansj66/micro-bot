/*
*  Copyright [2020] [Hans Jørgen Grimstad]
*  
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*  
*      http://www.apache.org/licenses/LICENSE-2.0
*  
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

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

