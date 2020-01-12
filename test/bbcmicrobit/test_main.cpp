#include <Arduino.h>
#include "i2c_helper.h"
#include "IOExpander.h"
#include <unity.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

IOExpander io_test;

void IO_verify_power_up_default_input_port0(void) 
{
    TEST_ASSERT_EQUAL(0b10000111, read_register(INPUT_PORT0));
}

void IO_verify_power_up_default_input_port1(void) 
{
    TEST_ASSERT_EQUAL(0b11111111, read_register(INPUT_PORT1));
}

void IO_verify_power_up_default_output_port0(void) 
{
    TEST_ASSERT_EQUAL(0b11111111, read_register(OUTPUT_PORT0));
}

void IO_verify_power_up_default_output_port1(void) 
{
    TEST_ASSERT_EQUAL(0b11111111, read_register(OUTPUT_PORT1));
}

void IO_verify_power_up_default_polarity_inversion_port0(void) 
{
    TEST_ASSERT_EQUAL(0x00000000, read_register(POLARITY_INVERSION_PORT0));
}

void IO_verify_power_up_default_polarity_inversion_port1(void) 
{
    TEST_ASSERT_EQUAL(0x00000000, read_register(POLARITY_INVERSION_PORT1));
}

void IO_verify_power_up_default_configuration_port0(void) 
{
// NW_SENSE_PIN - input - C0.3
// NE_SENSE_PIN - input - C0.4
// SW_SENSE_PIN - input - C0.5
// SE_SENSE_PIN - input - C0.6

    TEST_ASSERT_EQUAL(0b01111000, read_register(CONFIGURATION_PORT0));
}

void IO_verify_power_up_default_configuration_port1(void) 
{
    TEST_ASSERT_EQUAL(0x00, read_register(CONFIGURATION_PORT1));
}

void IO_set_bit_port_0(void)
{
    uint8_t before = read_register(OUTPUT_PORT0);
    TEST_ASSERT_EQUAL(0xFF, before);
    io_test.set_value(GPIO_1, LOW);
    TEST_ASSERT_EQUAL(0b01111111, read_register(OUTPUT_PORT0));
    io_test.set_value(GPIO_1, HIGH);
    TEST_ASSERT_EQUAL(0xFF, read_register(OUTPUT_PORT0));
}

void IO_set_bit_port_1(void)
{
    uint8_t before = read_register(OUTPUT_PORT1);
    TEST_ASSERT_EQUAL(0b11111111, before);
    io_test.set_value(GPIO_3, LOW);
    io_test.set_value(GPIO_5, LOW);
    TEST_ASSERT_EQUAL(0b11110101, read_register(OUTPUT_PORT1));
    io_test.set_value(GPIO_3, HIGH);
    TEST_ASSERT_EQUAL(0b11110111, read_register(OUTPUT_PORT1));
    io_test.set_value(GPIO_5, HIGH);
    TEST_ASSERT_EQUAL(0b11111111, read_register(OUTPUT_PORT1));
}

void IO_set_motor_control_bits(void)
{
    io_test.set_value(LEFT_MOTOR_INPUT1, LOW);
    io_test.set_value(LEFT_MOTOR_INPUT2, LOW);
    io_test.set_value(RIGHT_MOTOR_INPUT1, LOW);
    io_test.set_value(RIGHT_MOTOR_INPUT2, LOW);
    TEST_ASSERT_EQUAL(0b00001111, read_register(OUTPUT_PORT1));
    io_test.set_value(LEFT_MOTOR_INPUT1, HIGH);
    io_test.set_value(LEFT_MOTOR_INPUT2, HIGH);
    io_test.set_value(RIGHT_MOTOR_INPUT1, HIGH);
    io_test.set_value(RIGHT_MOTOR_INPUT2, HIGH);
    TEST_ASSERT_EQUAL(0b11111111, read_register(OUTPUT_PORT1));
}

/*
1) Kjør testene og spinn opp motorene samtidig
2) Forsøk digitalwrite til enable-pinnene for å se om det er PWM som er problemet
3) PWM-frekvens ?
4) er L293DD-vhippen skadet ?7
*/



void setup() {
    delay(2000);
    Wire.begin();
    io_test.begin();
    // Note: Before running the test suite:
    //  1) Place the bot on a white / light reflecting surface.
    //  2) Disconnect the USB cablebe
    //  3) Power cycle the bot
    //  4) Reconnect the USB cable.

    UNITY_BEGIN();    
    RUN_TEST(IO_verify_power_up_default_input_port0);
    RUN_TEST(IO_verify_power_up_default_input_port1);
    RUN_TEST(IO_verify_power_up_default_output_port0);
    RUN_TEST(IO_verify_power_up_default_output_port1);
    RUN_TEST(IO_verify_power_up_default_polarity_inversion_port0);
    RUN_TEST(IO_verify_power_up_default_polarity_inversion_port1);
    RUN_TEST(IO_verify_power_up_default_configuration_port0);
    RUN_TEST(IO_verify_power_up_default_configuration_port1);
    RUN_TEST(IO_set_bit_port_0);
    RUN_TEST(IO_set_bit_port_1);
    RUN_TEST(IO_set_motor_control_bits);
    UNITY_END();
}

void loop() 
{
}

