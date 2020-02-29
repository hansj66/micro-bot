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
#include "state_machine_line_follower.h"
#include "line_sensor.h"
#include "motor.h"

extern LineSensorArray bottom_sensor;
extern Motor motor;
extern MicroBitDisplay display;

namespace LineFollower 
{

#define FORWARD_SPEED   250
#define TURN_SPEED      250
#define REVERSE_WAIT_MS 100
#define TURN_WAIT_MS    50

enum MESSAGE
{ 
    START, 
    NW_LINE, 
    NW_ARENA, 
    NE_LINE, 
    NE_ARENA,
    ERROR 
};

enum MESSAGE start_state(void)
{
    display.print("LINE");
    return START;
}
    
enum MESSAGE forward_state(void)
{
    motor.forward();
    motor.setSpeed(FORWARD_SPEED);
    
    while (true)
    {
        bottom_sensor.update();
        if (LINE == bottom_sensor.NE) 
            return NE_LINE;
        if (LINE == bottom_sensor.NW)
            return NW_LINE;
    }
}

enum MESSAGE right_turn_state(void)
{
    motor.setSpeed(TURN_SPEED);
    motor.turn_right();

    while (true)
    {
        bottom_sensor.update();
        if (ARENA == bottom_sensor.NE)
        {
            wait_ms(1);
            return NE_ARENA;
        }
    }
}

enum MESSAGE left_turn_state(void)
{
    motor.setSpeed(TURN_SPEED);
    motor.turn_left();
    
    while (true)
    {
        bottom_sensor.update();
        if (ARENA == bottom_sensor.NW)
        {
            wait_ms(1);
            return NW_ARENA;
        }
    }
}

enum STATE_ID 
{ 
    START_STATE = 0,
    FORWARD_STATE, 
    RIGHT_TURN_STATE, 
    LEFT_TURN_STATE
};

enum MESSAGE (*handler[])() = 
{
    start_state, 
    forward_state,
    right_turn_state,
    left_turn_state
};


struct transition
{
    STATE_ID   current_state;
    MESSAGE message;
    STATE_ID    next_state;
};

#define TRANSITIONS 5

struct transition transitions[TRANSITIONS] = {
    {START_STATE, START, FORWARD_STATE},
    {FORWARD_STATE, NE_LINE, RIGHT_TURN_STATE},    
    {FORWARD_STATE, NW_LINE, LEFT_TURN_STATE},    
    {RIGHT_TURN_STATE, NE_ARENA, FORWARD_STATE},    
    {LEFT_TURN_STATE, NW_ARENA, FORWARD_STATE},    
};




void Run()
{
    printf("Running state machine\r\n");

    enum STATE_ID current_state = START_STATE;
    enum MESSAGE message = START;
    
    while (true) 
    {
        message = handler[current_state]();
        for (int i=0; i<TRANSITIONS; i++)
        {
            if ((transitions[i].current_state == current_state) && (transitions[i].message == message))
            {
                current_state = transitions[i].next_state;
                break;
            }
        }
    }
}

}