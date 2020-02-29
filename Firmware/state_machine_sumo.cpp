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
#include "state_machine_sumo.h"
#include "line_sensor.h"
#include "motor.h"
#include "proximity_sensor.h"

extern MicroBitDisplay display;
extern ProximitySensorArray front_sensor;
extern LineSensorArray bottom_sensor;
extern Motor motor;

namespace Sumo {
    
#define TARGET_CUTOFF 400
    
enum MESSAGE
{ 
    SCAN,
    TARGET_DETECT_FRONT,
    TARGET_DETECT_LEFT,
    TARGET_DETECT_RIGHT,
    NO_TARGET,
    FRONT_LINE_SENSOR_TRIGGERED,
    ERROR 
};

enum MESSAGE start_state(void)
{
    wait_ms(2000);
    return SCAN;
}

enum MESSAGE scan_state(void)
{
    printf("scan state\r\n");
    motor.setSpeed(200);
    motor.turn_right();
    while (true)
    {
        front_sensor.update();
        printf("Front sensor range: %d\r\n", front_sensor.centerRange());
        wait_ms(2);
        if (front_sensor.centerRange() < TARGET_CUTOFF)
        {
            return TARGET_DETECT_FRONT;
        }
    }

}
    
enum MESSAGE chase_state(void)
{
    printf("chase state\r\n");
    motor.forward();
    motor.setSpeed(700);
    while (true)
    {
        front_sensor.update();
        bottom_sensor.update();
        if ((LINE == bottom_sensor.NE) || (LINE == bottom_sensor.NW))
            return FRONT_LINE_SENSOR_TRIGGERED;

        if (front_sensor.centerRange() > TARGET_CUTOFF)
        {
            if (front_sensor.leftRange() < TARGET_CUTOFF)
                return TARGET_DETECT_LEFT;
            if (front_sensor.rightRange() < TARGET_CUTOFF)
                return TARGET_DETECT_RIGHT;
        }
    }
}

enum MESSAGE reverse_state(void)
{
    printf("reverse state\r\n");
    motor.reverse();
    motor.setSpeed(300);
    while (true) {
        if ((LINE == bottom_sensor.NE) || (LINE == bottom_sensor.NW))
        {
            wait_ms(1500);
            return SCAN;
        }

    }
}


enum MESSAGE align_right_state(void)
{
    printf("align right state\r\n");
    motor.setSpeed(200);
    motor.turn_right();
    while (true)
    {
        front_sensor.update();
        if (front_sensor.centerRange() < TARGET_CUTOFF)
        {
            return TARGET_DETECT_FRONT;
        }
    }

}

enum MESSAGE align_left_state(void)
{
    printf("align left state\r\n");
    motor.setSpeed(150);
    motor.turn_left();
    while (true)
    {
        front_sensor.update();
        if (front_sensor.centerRange() < TARGET_CUTOFF)
        {
            return TARGET_DETECT_FRONT;
        }
    }
}



enum STATE_ID 
{ 
    START_STATE = 0,
    SCAN_STATE, 
    CHASE_STATE,
    ALIGN_RIGHT_STATE,
    ALIGN_LEFT_STATE,
    REVERSE_STATE,
};

enum MESSAGE (*handler[])() = 
{
    start_state,
    scan_state,
    chase_state,
    align_right_state,
    align_left_state,
    reverse_state,
};


struct transition
{
    STATE_ID   current_state;
    MESSAGE message;
    STATE_ID    next_state;
};

#define TRANSITIONS 9

struct transition transitions[TRANSITIONS] = 
{
    {START_STATE, SCAN, SCAN_STATE},
    {SCAN_STATE, TARGET_DETECT_FRONT, CHASE_STATE},    
    {CHASE_STATE, NO_TARGET, SCAN_STATE},    
    {CHASE_STATE, TARGET_DETECT_RIGHT, ALIGN_RIGHT_STATE},    
    {ALIGN_RIGHT_STATE, TARGET_DETECT_FRONT, CHASE_STATE},    
    {CHASE_STATE, TARGET_DETECT_LEFT, ALIGN_LEFT_STATE},    
    {ALIGN_LEFT_STATE, TARGET_DETECT_FRONT, CHASE_STATE},    
    {CHASE_STATE, FRONT_LINE_SENSOR_TRIGGERED, REVERSE_STATE},    
    {REVERSE_STATE, SCAN, SCAN_STATE}
};




void Run()
{
    printf("Running state machine\r\n");
    
    display.print('H');

     enum STATE_ID current_state = START_STATE;
    enum MESSAGE message = SCAN;
    
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