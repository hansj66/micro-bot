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

#include <Arduino.h>
#include "IOExpander.h"
#include "Wire.h"

char msgBuf[128];

IOExpander::IOExpander() : 
    _PIN0(0), 
    _PIN1(0) 
{
}

void IOExpander::write_register(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(TCA9539_ADDRESS);
    Wire.write(reg);
    Wire.write(value);  
    if (0 != Wire.endTransmission())
    {
        sprintf(msgBuf, "Write %02X to register %02X failed.", value, reg);
        Serial.println(msgBuf);
    }
}
 
uint8_t IOExpander::read_register(uint8_t reg)
{
    Wire.beginTransmission(TCA9539_ADDRESS);
    Wire.write(reg);
    if (0!= Wire.endTransmission())
    {
        Serial.print("Read ");
        Serial.print(reg);
        Serial.println(" register failed");
    }
    Wire.requestFrom(TCA9539_ADDRESS, 1);
    return Wire.read();
}

void IOExpander::configure_port0()
{
    write_register(CONFIGURATION_PORT0, pinMap[NW_SENSE_PIN] | pinMap[NE_SENSE_PIN] | pinMap[SW_SENSE_PIN] | pinMap[SE_SENSE_PIN]);
}

void IOExpander::configure_port1()
{
    write_register(CONFIGURATION_PORT1, 0);
}

void IOExpander::scan()
{
    _PIN0 = read_register(INPUT_PORT0);
    _PIN1 = read_register(INPUT_PORT1);
}

uint8_t IOExpander::get_value(uint8_t pin)
{
    if (pin < 8) 
        return _PIN0 & pinMap[pin];
    return _PIN1 & pinMap[pin];
}

void IOExpander::set_value(uint8_t pin, uint8_t value)
{
    uint8_t port = OUTPUT_PORT1;
    if (pin < 8) {
        port = OUTPUT_PORT0;
    }

    uint8_t state = read_register(port);

    uint8_t newState;
    if (HIGH == value)
    {
        newState = state | pinMap[pin];    // set bit
    }
    else
    {
        newState = state & ~(pinMap[pin]);   // clear bit
    }

    Wire.beginTransmission(TCA9539_ADDRESS);
    Wire.write(port);
    Wire.write(newState);    
    Wire.endTransmission();
}

bool IOExpander::line_sensor_hit()
{
    return (read_register(INPUT_PORT0) & 0b01111000) != 0b01111000;
}

void IOExpander::begin()
{
    configure_port0();
    configure_port1();
}
