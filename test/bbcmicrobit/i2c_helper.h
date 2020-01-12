#ifndef _I2C_HELPER_H_
#define _I2C_HELPER_H_

#include <Arduino.h>

void write_register(uint8_t reg, uint8_t value);
uint8_t read_register(uint8_t reg);

#endif