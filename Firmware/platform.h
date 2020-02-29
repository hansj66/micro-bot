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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <map>
#include <string>
#include "IOExpander.h"

enum SELFTEST_EVENT {
    EXIT_TEST,
    NEXT_TEST
} ;



class Platform
{
    public:
        Platform(IOExpander * io);
        bool SelfCheck();

    private:
        bool VerifyI2CBus();
        bool VerifyIOExpanderRegisters();
        void RunSelfTestSequence();
        void RunSensorTest();
        SELFTEST_EVENT LineSensorTest();
        SELFTEST_EVENT RangeSensorTest();
        SELFTEST_EVENT MotorTest();
        void OnButtonA();
        void OnButtonB();

        IOExpander * _io;

    std::map<uint8_t, std::pair<bool, std::string> > _address_map;
    std::map<uint8_t, std::pair<uint8_t, std::string> > _io_expander_port_map;
};



#endif // _PLATFORM_H_