#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "utils.hpp"
#include "pin.hpp"

using greenhouse::utils::eps;

const uint8_t pin = 0;

void pin_output_is_low_after_creation_test(void)
{
    greenhouse::pin::OutputPin<pin> out_pin;

    pinMode(pin, INPUT);
    TEST_ASSERT_EQUAL(LOW, digitalRead(pin));
}

void pin_output_is_high_after_setting_high_test(void)
{
    greenhouse::pin::OutputPin<pin> out_pin;

    pinMode(pin, INPUT);
    TEST_ASSERT_EQUAL(LOW, digitalRead(pin));
    pinMode(pin, OUTPUT);

    out_pin.high();

    pinMode(pin, INPUT);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(pin));
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(pin_output_is_low_after_creation_test);
    RUN_TEST(pin_output_is_high_after_setting_high_test);

    UNITY_END();
}

void loop()
{
}
