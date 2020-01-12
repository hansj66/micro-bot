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

#include "proximity_sensor.h"
#include "IOExpander.h"

Adafruit_VL53L0X lox_center = Adafruit_VL53L0X();
Adafruit_VL53L0X lox_left = Adafruit_VL53L0X();
Adafruit_VL53L0X lox_right = Adafruit_VL53L0X();


ProximitySensorArray::ProximitySensorArray(IOExpander * io) : 
  _io(io),
  _left(MAX_RANGE),
  _center(MAX_RANGE),
  _right(MAX_RANGE)
{
}

void ProximitySensorArray::begin()
{
  Serial.println("Initializing time of flight sensors");

  reset();
  activate(LEFT, lox_left, 0x70);
  activate(RIGHT, lox_right, 0x71);
  activate(CENTER, lox_center, 0x72);
}

void ProximitySensorArray::reset() 
{
  _io->set_value(XSHUT_CENTER, LOW);
  _io->set_value(XSHUT_LEFT, LOW);
  _io->set_value(XSHUT_RIGHT, LOW);
  delay(10);

  _io->set_value(XSHUT_CENTER, HIGH);
  _io->set_value(XSHUT_LEFT, HIGH);
  _io->set_value(XSHUT_RIGHT, HIGH);
  delay(10);

  _io->set_value(XSHUT_CENTER, LOW);
  _io->set_value(XSHUT_LEFT, LOW);
  _io->set_value(XSHUT_RIGHT, LOW);
}


void ProximitySensorArray::activate(RANGINING_SENSOR_ID id, Adafruit_VL53L0X & sensor, int i2c_address) 
{
  char msg[128];

  switch (id) 
  {
    case LEFT:    _io->set_value(XSHUT_LEFT, HIGH);
                  break;
    case CENTER:  _io->set_value(XSHUT_CENTER, HIGH);
                  break;
    case RIGHT:   _io->set_value(XSHUT_RIGHT, HIGH);
                  break;
  }
  delay(10);
 
  while (!sensor.begin(i2c_address)) 
  {
    sprintf(msg, "Failed to boot VL53L0X at address :0x%02X", i2c_address);
    Serial.println(msg);
    delay(100);
  }
  sprintf(msg, "VL53L0X booted at address :0x%02X", i2c_address);
  Serial.println(msg);
}

void ProximitySensorArray::scan()
{
  _left = MAX_RANGE;
  _center = MAX_RANGE;
  _right = MAX_RANGE;
  VL53L0X_RangingMeasurementData_t center_measure;
  VL53L0X_RangingMeasurementData_t left_measure;
  VL53L0X_RangingMeasurementData_t right_measure;

  lox_center.rangingTest(&center_measure, NO_DEBUG);
  if (center_measure.RangeStatus != OUT_OF_RANGE) 
  {  
    _center = center_measure.RangeMilliMeter;
  }
  delay(50);  
  lox_left.rangingTest(&left_measure, NO_DEBUG);
  if (left_measure.RangeStatus != OUT_OF_RANGE) 
  {  
    _left = left_measure.RangeMilliMeter;
  }
  delay(50);
  lox_right.rangingTest(&right_measure, NO_DEBUG);
  if (right_measure.RangeStatus != OUT_OF_RANGE) 
  {  
    _right = right_measure.RangeMilliMeter;
  }
  delay(50);
}
