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

#ifndef _I2C_ADDRESS_MAP_H_
#define _I2C_ADDRESS_MAP_H_

#define ONBOARD_I2C_DEVICES 6

// #define VL53L0X_I2C_ADDRESS_LEFT    0xC0 // 0x60 7 bit
// #define VL53L0X_I2C_ADDRESS_CENTER  0xC1 // 0x61 7 bit
// #define VL53L0X_I2C_ADDRESS_RIGHT   0xC2 // 0x62 7 bit

#define VL53L0X_I2C_ADDRESS_LEFT    0x31
#define VL53L0X_I2C_ADDRESS_CENTER  0x33
#define VL53L0X_I2C_ADDRESS_RIGHT   0x35

#define TCA9539_I2C_ADDRESS 0xE8

#endif // _I2C_ADDRESS_MAP_H_