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
#include "MicroBitDisplay.h"
#include "pinout.h"
#include "IOExpander.h"
#include "motor.h"
#include "proximity_sensor.h"
#include "platform.h"
#include "graphics.h"

#include "MicroBitPin.h"

I2C i2c(I2C_SDA0, I2C_SCL0); 

MicroBitDisplay display;
IOExpander io;
ProximitySensorArray front_sensor(&io);
Platform p(&io);
Motor motor(&io);


DigitalIn buttonA(BUTTON_A);
DigitalIn buttonB(BUTTON_B);

void RunProgram()
{
    // TODO : Makecode integration + demo
    while (true)
    {
        display.print("- RUNNING PROGRAM -");
    }
}


int main() 
{
    wait_ms(1000);
    printf("-----------------------\n\r");
    printf("------ Micro:bot ------\n\r");
    printf("-----------------------\n\r");
    i2c.frequency(400000);

    wait_ms(2000);

    if (!p.SelfCheck()) {
        while (true)
        {
            DisplayFail();
        }
    }

    while (true)
    {
        DisplayReady();
        if ((0 == buttonB.read()) || (0 == buttonA.read()))
            break;
    }

    RunProgram();
}
