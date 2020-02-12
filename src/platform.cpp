#include "mbed.h"
#include "i2c_address_map.h"
#include "platform.h"
#include "IOExpander.h"
#include <map>
#include <string>
#include <iterator>
#include "MicroBitPin.h"
#include "MicroBitButton.h"
#include "MicroBitDisplay.h"
#include "graphics.h"
#include "proximity_sensor.h"

extern I2C i2c;

extern MicroBitDisplay display;

// InterruptIn doesn't seem to work in this context...
extern DigitalIn buttonA;
extern DigitalIn buttonB;

extern ProximitySensorArray front_sensor;

Platform::Platform(IOExpander * io) :
    _io(io)
{
    _address_map[VL53L0X_I2C_ADDRESS_LEFT << 1] = std::make_pair(false, "Left facing VL53L0X sensor");
    _address_map[VL53L0X_I2C_ADDRESS_CENTER << 1] = std::make_pair(false, "Center facing VL53L0X sensor");
    _address_map[VL53L0X_I2C_ADDRESS_RIGHT << 1] = std::make_pair(false, "Right facing VL53L0X sensor");
    _address_map[TCA9539_I2C_ADDRESS] = std::make_pair(false, "IO Expander");

    // Checking the port values doesn't really make sense unless every bot is tested on
    // a similar surface. Maybe define a test pattern ?
    // _io_expander_port_map[INPUT_PORT0] = std::make_pair(0b10000111, "PORT 0 register");
    // _io_expander_port_map[INPUT_PORT1] = std::make_pair(0b11111111, "PORT 1 register");
    _io_expander_port_map[OUTPUT_PORT0] = std::make_pair(0b11111111, "Output PORT 0 register");
    _io_expander_port_map[OUTPUT_PORT1] = std::make_pair(0b11111111, "Output PORT 1 register");
    _io_expander_port_map[POLARITY_INVERSION_PORT0] = std::make_pair(0b00000000, "Polarity inversion PORT0 register");
    _io_expander_port_map[POLARITY_INVERSION_PORT1] = std::make_pair(0b00000000, "Polarity inversion PORT1 register");
    _io_expander_port_map[CONFIGURATION_PORT0] = std::make_pair(0b01111000, "Configuration PORT0 register (Expansion pins: PO7/GPIO01).");
    _io_expander_port_map[CONFIGURATION_PORT1] = std::make_pair(0b00000000, "Configuration PORT1 register (Expansion pins: P10/GPIO02, P11/GPIO3, P12/GPIO4, P13/GPIO5).");
}

SELFTEST_EVENT Platform::LineSensorTest()
{
    display.print("- TEST - LINE SENSORS -");
    while (true)
    {
        RenderLineSensorResponse(
            _io->get_value(NW_SENSE_PIN), 
            _io->get_value(NE_SENSE_PIN),
            _io->get_value(SW_SENSE_PIN),
            _io->get_value(SE_SENSE_PIN)
        );
        if (0 == buttonB.read()) 
            return NEXT_TEST;
        if (0 == buttonA.read())
            return EXIT_TEST;
    }
}

SELFTEST_EVENT Platform::RangeSensorTest()
{
    display.print("- TEST - RANGE SENSORS -");
    while (true)
    {
        uint16_t left = front_sensor.leftRange();
        uint16_t center = front_sensor.centerRange();
        uint16_t right = front_sensor.rightRange();

        printf("L: %04d C: %04d: R: %04d\r\n", left, center, right);

        if (0 == buttonB.read()) 
            return NEXT_TEST;
        if (0 == buttonA.read())
            return EXIT_TEST;
    }
}

SELFTEST_EVENT Platform::MotorTest()
{
    return EXIT_TEST;
}

void Platform::RunSensorTest()
{
    if (EXIT_TEST == LineSensorTest())
        return;
    if (EXIT_TEST == RangeSensorTest())
        return;
    if (EXIT_TEST == MotorTest())
        return;
}

void Platform::RunSelfTestSequence()
{
    RightArrowAnimation();
    while (true)
    {
        RightArrowBump();
        if (0 == buttonB.read()) {
            RunSensorTest();
            break;
        }
        if (0 == buttonA.read())
        {
            break;
        }
    }
}



// Performs a platform diagnostic by checking for the presence
// of expected I2C devices and correctly configured sensors
bool Platform::SelfCheck()
{
    bool hardwareOK = true;
    if (!VerifyI2CBus())
        hardwareOK = false;
    if (!VerifyIOExpanderRegisters())
        hardwareOK = false;

    if (!hardwareOK)
    {
        // Connect USB cable and a serial monitor
        // in order to inspect error messages
        DisplayFail();
    }
    else
    {
        RunSelfTestSequence();
    }
    return hardwareOK;   
}

// Checks for correctly configured IO expander registers
// (and implicitly line sensors)
bool Platform::VerifyIOExpanderRegisters()
{
    std::map<uint8_t, std::pair<uint8_t, std::string> >::iterator it = _io_expander_port_map.begin();

    while (it != _io_expander_port_map.end())
    {
        char reg = it->first;
        char data;
        i2c.write(TCA9539_I2C_ADDRESS, &reg, 1, true);
        i2c.read(TCA9539_I2C_ADDRESS, &data, 1);

        if (data != it->second.first)
        {
            printf("WARNING - Unexpected IO Expander register configuration:\r\n");
            printf("%s contains: %02X. Expected: %02X\r\n", it->second.second.c_str(), data, it->second.first);
            return false;
        }
        it++;
    }
    return true;
}


// Checks for the presence of the IOExpander and also for
// correctly configured VL53L0X time of flight sensors.
bool Platform::VerifyI2CBus()
{
    std::map<uint8_t, std::pair<bool, std::string> >::iterator it = _address_map.begin();
    while (it != _address_map.end())
	{
        char data = 0xFF;
        if (0 == i2c.write(it->first,&data,1))
        {
            _address_map[it->first].first = true;
            printf("%s found at address 0x%X - OK\r\n", it->second.second.c_str(), it->first); 
        }
        it++;
        wait_ms(1);
    }

    it = _address_map.begin();
    while (it != _address_map.end())
    {

        if (!(it->second.first))
        {
            printf("HARDWARE ERROR - %s not found at expected 0x%X\r\n", it->second.second.c_str(), it->first); 
            return false;
        }
        it++;
    }
    return true;
}
