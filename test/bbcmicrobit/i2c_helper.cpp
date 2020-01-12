#include <Arduino.h>
#include <Wire.h>
#include "IOExpander.h"
#include "i2c_helper.h"

void write_register(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(TCA9539_ADDRESS);
    Wire.write(reg);
    Wire.write(value);  
    Wire.endTransmission();
}
 
uint8_t read_register(uint8_t reg)
{
    Wire.beginTransmission(TCA9539_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(TCA9539_ADDRESS, 1);
    return (uint8_t)Wire.read();
}