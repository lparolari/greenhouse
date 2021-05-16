#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "pin.hpp"

const uint8_t test_pin = 22; // read values for test
const uint8_t in_pin = 23;   // controlled by the component to test

void pin_input_is_low_without_read(void)
{
    digitalWrite(test_pin, LOW);

    greenhouse::pin::DigitalInput<in_pin> input_pin;
    input_pin.begin();

    TEST_ASSERT_EQUAL(LOW, input_pin.get());
}

void pin_input_is_low_test(void)
{
    digitalWrite(test_pin, LOW);

    greenhouse::pin::DigitalInput<in_pin> input_pin;
    input_pin.begin();

    input_pin.read();

    TEST_ASSERT_EQUAL(LOW, input_pin.get());
}

void pin_input_is_high_test(void)
{
    digitalWrite(test_pin, HIGH);

    greenhouse::pin::DigitalInput<in_pin> input_pin;
    input_pin.begin();

    input_pin.read();

    TEST_ASSERT_EQUAL(HIGH, input_pin.get());
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);

    UNITY_BEGIN();

    pinMode(test_pin, OUTPUT);

    RUN_TEST(pin_input_is_low_without_read);
    RUN_TEST(pin_input_is_low_test);
    RUN_TEST(pin_input_is_high_test);

    UNITY_END();
}

void loop()
{
}
