#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "pin.hpp"

const uint8_t test_pin = 3;
const uint8_t in_pin = 14;

void pin_input_is_zero_without_read(void)
{
    analogWrite(test_pin, 0);

    greenhouse::pin::AnalogInput<in_pin> input_pin;
    input_pin.begin();

    TEST_ASSERT_EQUAL(0, input_pin.get());
}

void pin_input_is_zero(void)
{
    analogWrite(test_pin, 0);

    greenhouse::pin::AnalogInput<in_pin> input_pin;
    input_pin.begin();

    input_pin.read();

    TEST_ASSERT_EQUAL(0, input_pin.get());
}

void pin_input_is_1023_test(void)
{
    analogWrite(test_pin, 255);

    greenhouse::pin::AnalogInput<in_pin> input_pin;
    input_pin.begin();

    input_pin.read();

    TEST_ASSERT_EQUAL(1023, input_pin.get());
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);

    UNITY_BEGIN();

    pinMode(test_pin, OUTPUT);

    RUN_TEST(pin_input_is_zero_without_read);
    RUN_TEST(pin_input_is_zero);
    RUN_TEST(pin_input_is_1023_test);

    UNITY_END();
}

void loop()
{
}
