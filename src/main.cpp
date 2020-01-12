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
#include <Wire.h>
#include <Adafruit_Microbit.h>
#include "pinout.h"
#include "proximity_sensor.h"
#include "IOExpander.h"
#include "motor.h"

Adafruit_Microbit_Matrix display;
IOExpander io;
ProximitySensorArray front_sensor(&io);
Motor motor(&io);

#ifndef UNIT_TEST 
void setup() 
{
  Serial.begin(115200);
  delay(2000);

  display.begin();
  Wire.setClock(100000);
  Wire.begin();
  io.begin();
  front_sensor.begin();
  motor.begin();

  Serial.println("Sumo robot demo is ready!");
}

void display_triggered_line_sensors()
{
  int nw = io.get_value(NW_SENSE_PIN);
  int ne = io.get_value(NE_SENSE_PIN);
  int sw = io.get_value(SW_SENSE_PIN);
  int se = io.get_value(SE_SENSE_PIN);

  display.drawPixel(4,0,!nw);
  display.drawPixel(0,0,!ne);
  display.drawPixel(4,4,!sw);
  display.drawPixel(0,4,!se);
}

void display_proximity_bar_graphs()
{
  // Ignoring stuff that is farther away than 50cm
  int c = front_sensor.get_center_range();
  if (c > 250)
    c = 250;
  int l = front_sensor.get_left_range();
  if (l > 250)
    l = 250;
  int r = front_sensor.get_right_range();
  if (r > 250)
    r = 250;

  c = (250-c)/50;
  l = (250-l)/50;
  r = (250-r)/50;
  
  display.drawLine(1,0,1,4,0);
  display.drawLine(2,0,2,4,0);
  display.drawLine(3,0,3,4,0);

  if (r > 0)
    display.drawLine(1,r,1,0,1);
  if (c > 0)
    display.drawLine(2,c,2,0,1);
  if (l > 0)
    display.drawLine(3,l,3,0,1);

}

void loop() 
{
  // Sensor demo
  // 1) A triggered line sensor will light up a LED in the corresponding corner of the display
  // 2) Objects in proximity to the range sensors will 
 
    // Scan inputs (GPIO on expansion header and hard wired line sensors)
    io.scan();  
    // Scan for collisions (front proximity sensors)
    front_sensor.scan(); 

    // Simple demo. Just reversing direction and turning left
    // if the front sensor detects an object that is closer than 15 cm
    if (front_sensor.get_center_range() < 150) 
    {
      motor.reverse();
      delay(150);
      motor.turn_left();
      delay(250);
    }
    else {
      motor.forward();
    }

    // We'll display a pixel in the corresponding corner
    // of the matrix for each line sensor that is triggered
    // and a "bargraph" (representing the proximity of objects 
    // in front of the three proximity sensors) for each of the
    // three center columns in the matrix
    display_triggered_line_sensors();
    display_proximity_bar_graphs();

    // And finally, let's just blink the center pixel to indicate 
    // that the code is running.
    display.drawPixel(2,2,1);
    delay(100);
    display.drawPixel(2,2,0);
    delay(100);
}
#endif