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

#ifndef _TCA9539_H_
#define _TCA9539_H_

#include "wire.h"

const int TCA9539_ADDRESS = 0x74;

const int XSHUT_RIGHT           = 0;    // C0.0
const int XSHUT_LEFT            = 1;    // C0.1
const int XSHUT_CENTER          = 2;    // C0.2
const int NW_SENSE_PIN          = 3;    // C0.3
const int NE_SENSE_PIN          = 4;    // C0.4
const int SW_SENSE_PIN          = 5;    // C0.5
const int SE_SENSE_PIN          = 6;    // C0.6
const int GPIO_1                = 7;    // C0.7
const int GPIO_2                = 8;    // C1.0
const int GPIO_3                = 9;    // C1.1
const int GPIO_4                = 10;   // C1.2
const int GPIO_5                = 11;   // C1.3
const int LEFT_MOTOR_INPUT1     = 12;   // C1.4
const int LEFT_MOTOR_INPUT2     = 13;   // C1.5
const int RIGHT_MOTOR_INPUT1    = 14;   // C1.6
const int RIGHT_MOTOR_INPUT2    = 15;   // C1.7

const uint8_t pinMap[16] = 
{
1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 
1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 
};

// Command registers
const int INPUT_PORT0   = 0x00;
const int INPUT_PORT1   = 0x01;
const int OUTPUT_PORT0  = 0x02;
const int OUTPUT_PORT1  = 0x03;
const int POLARITY_INVERSION_PORT0  = 0x04;
const int POLARITY_INVERSION_PORT1  = 0x05;
const int CONFIGURATION_PORT0  = 0x06;
const int CONFIGURATION_PORT1  = 0x07;

class IOExpander
{
    public:
        IOExpander();
        void begin();
        void scan();
        uint8_t get_value(uint8_t pin);
        void set_value(uint8_t pin, uint8_t value);
        bool line_sensor_hit();


    private:
        void configure_port0();
        void configure_port1();
        void write_register(uint8_t reg, uint8_t value);
        uint8_t read_register(uint8_t reg);

        uint8_t _PIN0;  // Input state PORT0
        uint8_t _PIN1;  // Input state PORT1
};

#endif