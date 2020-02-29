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