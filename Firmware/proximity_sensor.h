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

#ifndef _PROXIMITY_SENSOR_H_
#define _PROXIMITY_SENSOR_H_

#include <VL53L0X.h>
#include "IOExpander.h"

class ProximitySensorArray
{
  public:
    ProximitySensorArray(IOExpander * io);
    uint16_t leftRange();
    uint16_t centerRange();
    uint16_t rightRange();
    void update();

  private:
    VL53L0X _center;
    VL53L0X _left;
    VL53L0X _right;
    uint16_t _leftRange;
    uint16_t _centerRange;
    uint16_t _rightRange;

    void xshutdown();
    void assignSensorI2CAddress(VL53L0X & sensor, uint8_t xshutPin, uint8_t address);

    IOExpander * _io;
};

#endif