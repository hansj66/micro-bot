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

#ifndef _PINOUT_H_
#define _PINOUT_H

// Micro:bit native pinout
#define LEFT_SPEED P8
#define RIGHT_SPEED P16 

// In addition to the above mentioned pins, 5 GPIO pins are
// available via the expansion port. These can be configured as
// input or output pins. The pins output pins by default and
// can only be accessed via the IOExpander class. The pin names
// are :
// PORT0 : GPIO_1
// PORT1 : GPIO_2, GPIO_3, GPIO_4, GPIO_5
// Configuring these as inputs is done by reading the corresponding
// port register and then doing a bitwise OR between pinMap[<gpio pin name>]
// and the register. Write the result back to the correct configuration
// register.
//
// Unregulated battery voltage, regulated +3.3V 
// and GND are also available from the expansion header. 

#endif