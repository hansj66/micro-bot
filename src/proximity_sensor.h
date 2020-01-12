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

#include <Adafruit_VL53L0X.h>
#include <BLEPeripheral.h>
#include "IOExpander.h"

// VL53L0X
#define OUT_OF_RANGE 4
#define NO_DEBUG false

const int MAX_RANGE = 2000;

enum RANGINING_SENSOR_ID 
{
  LEFT,
  CENTER,
  RIGHT
};

class ProximitySensorArray
{
  public:
    ProximitySensorArray(IOExpander * io);
    void begin();
    void scan();
    int get_left_range() { return _left; }
    int get_center_range() { return _center; }
    int get_right_range() { return _right; }

  private:
    void reset();
    void activate(RANGINING_SENSOR_ID id, Adafruit_VL53L0X & sensor, int i2c_address);

    IOExpander * _io;
    int _left;
    int _center;
    int _right;

};

#endif