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
#include "i2c_address_map.h"
#include "IOExpander.h"
#include "ErrorNo.h"

extern I2C i2c;


char msgBuf[128];

IOExpander::IOExpander() : 
    _PIN0(0), 
    _PIN1(0) 
{
    configure_port0();
    configure_port1();
}

void IOExpander::write_register(uint8_t reg, uint8_t value)
{
  char buf[2];
  buf[0] = reg;
  buf[1] = value;
  i2c.write(TCA9539_I2C_ADDRESS, buf, 2);
}
 
uint8_t IOExpander::read_register(uint8_t reg)
{
    char value;
    char data = reg;
    i2c.write(TCA9539_I2C_ADDRESS, &data, 1, true);
    i2c.read(TCA9539_I2C_ADDRESS, &value, 1);
    return value;
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
    scan();
    if (pin < 8) 
        return _PIN0 & pinMap[pin];
    return _PIN1 & pinMap[pin];
}

void IOExpander::set_value(uint8_t pin, uint8_t value)
{
    char port = OUTPUT_PORT1;
    if (pin < 8) {
        port = OUTPUT_PORT0;
    }

    char state = read_register(port);

    char newState;
    if (HIGH == value)
    {
        newState = state | pinMap[pin];    // set bit
    }
    else
    {
        newState = state & ~(pinMap[pin]);   // clear bit
    }

    char buf[2];
    buf[0] = port;
    buf[1] = newState;
    if (MICROBIT_OK != i2c.write(TCA9539_I2C_ADDRESS, &buf[0], 2))
         printf("ERROR: IOExpander::set_value\r\n");
}

bool IOExpander::line_sensor_hit()
{
    return (read_register(INPUT_PORT0) & 0b01111000) != 0b01111000;
}

