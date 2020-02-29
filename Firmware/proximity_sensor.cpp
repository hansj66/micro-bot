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
#include "proximity_sensor.h"
#include "i2c_address_map.h"
#include "IOExpander.h"

bool REFRESH_PROXIMITY_SENSOR = true;

extern ProximitySensorArray front_sensor;

ProximitySensorArray::ProximitySensorArray(IOExpander * io) : 
    _io(io),
    _leftRange(0),
    _centerRange(0),
    _rightRange(0)
{
  xshutdown();

  assignSensorI2CAddress(_left, XSHUT_LEFT, VL53L0X_I2C_ADDRESS_LEFT);
  assignSensorI2CAddress(_center, XSHUT_CENTER, VL53L0X_I2C_ADDRESS_CENTER);
  assignSensorI2CAddress(_right, XSHUT_RIGHT, VL53L0X_I2C_ADDRESS_RIGHT);
  
  
}

// xshutdown pulls all the XSHUT signals low and thereby 
// also causes all VL53L0X sensors to enter hardware reset state.
void ProximitySensorArray::xshutdown()
{
  _io->set_value(XSHUT_CENTER, LOW);
  _io->set_value(XSHUT_LEFT, LOW);
  _io->set_value(XSHUT_RIGHT, LOW);
   wait_ms(10);
}

// assignSensorI2CAddress brings a sensor out of reset state, initializes
// the sensor and assigns a new I2C address to it (default is 0x29 (7 bit))
void ProximitySensorArray::assignSensorI2CAddress(VL53L0X & sensor, uint8_t xshutPin, uint8_t address)
{
  _io->set_value(xshutPin, HIGH);
  wait_ms(1);
  if (!sensor.init()) 
  {
    printf("ProximitySensorArray::assignSensorI2CAddress - FAILED\r\n");
  }
  sensor.setAddress(address);
  wait_ms(150);
}

uint16_t ProximitySensorArray::leftRange()
{
  return _leftRange;
}

uint16_t ProximitySensorArray::centerRange()
{
  return _centerRange;
}

uint16_t ProximitySensorArray::rightRange()
{
  return _rightRange;
}

void ProximitySensorArray::update()
{
  _leftRange = _left.readRangeSingleMillimeters();
  _centerRange = _center.readRangeSingleMillimeters();
  _rightRange = _right.readRangeSingleMillimeters();
}



