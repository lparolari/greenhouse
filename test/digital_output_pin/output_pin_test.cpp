#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "utils.hpp"
#include "pin.hpp"

using greenhouse::utils::eps;

const uint8_t o_pin = 10; // read values for test
const uint8_t i_pin = 11; // controlled by the component to test

void pin_output_is_low_after_creation_test(void)
{
    greenhouse::pin::DigitalOutput<o_pin> out_pin;
    out_pin.begin();

    TEST_ASSERT_EQUAL(LOW, digitalRead(i_pin));
}

void pin_output_is_high_after_setting_high_test(void)
{
    greenhouse::pin::DigitalOutput<o_pin> out_pin;
    out_pin.begin();

    TEST_ASSERT_EQUAL(LOW, digitalRead(i_pin));

    out_pin.high();
    out_pin.apply();

    TEST_ASSERT_EQUAL(HIGH, digitalRead(i_pin));
}

void pin_output_can_toggle_off_test(void)
{
    greenhouse::pin::DigitalOutput<o_pin> out_pin;
    out_pin.begin();

    TEST_ASSERT_EQUAL(LOW, digitalRead(i_pin));

    out_pin.high();
    out_pin.apply();

    TEST_ASSERT_EQUAL(HIGH, digitalRead(i_pin));

    out_pin.low();
    out_pin.apply();

    TEST_ASSERT_EQUAL(LOW, digitalRead(i_pin));
}

void pin_output_is_high_after_setting_low_if_inverted_test(void)
{
    greenhouse::pin::DigitalOutput<o_pin, true> out_pin;
    out_pin.begin();

    TEST_ASSERT_EQUAL(HIGH, digitalRead(i_pin));

    out_pin.high();
    out_pin.apply();

    TEST_ASSERT_EQUAL(LOW, digitalRead(i_pin));

    out_pin.low();
    out_pin.apply();

    TEST_ASSERT_EQUAL(HIGH, digitalRead(i_pin));
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);

    UNITY_BEGIN();

    pinMode(i_pin, INPUT);

    RUN_TEST(pin_output_is_low_after_creation_test);
    RUN_TEST(pin_output_is_high_after_setting_high_test);
    RUN_TEST(pin_output_can_toggle_off_test);
    RUN_TEST(pin_output_is_high_after_setting_low_if_inverted_test);

    UNITY_END();
}

void loop()
{
}
