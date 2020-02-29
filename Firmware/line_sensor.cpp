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

#include "line_sensor.h"

bool REFRESH_LINE_SENSOR = true;

LineSensorArray::LineSensorArray(IOExpander * io) :
    _io(io)
{
}

void LineSensorArray::update()
{
    NW = _io->get_value(NW_SENSE_PIN);
    NE = _io->get_value(NE_SENSE_PIN);
    SW = _io->get_value(SW_SENSE_PIN);
    SE = _io->get_value(SE_SENSE_PIN);
}
