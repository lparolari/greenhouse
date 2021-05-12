#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "utils.hpp"
#include "button.hpp"

using greenhouse::utils::eps;

const uint8_t pin = 22;
const uint8_t button_delay = 50;

void button_input_is_high_test(void)
{
    pinMode(pin, INPUT);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(pin));
}

void button_is_not_pressed_if_value_not_read_test(void)
{
    greenhouse::button::DebounceButton<pin> debounce_button;

    TEST_ASSERT_FALSE(debounce_button.pressed());
}

void button_is_not_pressed_after_first_read_with_high_value_test(void)
{
    greenhouse::button::DebounceButton<pin> debounce_button;

    debounce_button.read();
    TEST_ASSERT_FALSE(debounce_button.pressed());
}

void button_is_pressed_if_after_delay_input_pin_is_high_test(void)
{
    greenhouse::button::DebounceButton<pin, button_delay> debounce_button;

    debounce_button.read();
    TEST_ASSERT_FALSE(debounce_button.pressed());

    delay(25);

    debounce_button.read();
    TEST_ASSERT_FALSE(debounce_button.pressed());

    delay(50);

    debounce_button.read();

    TEST_ASSERT_TRUE(debounce_button.pressed());
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(button_input_is_high_test);
    RUN_TEST(button_is_not_pressed_if_value_not_read_test);
    RUN_TEST(button_is_not_pressed_after_first_read_with_high_value_test);
    RUN_TEST(button_is_pressed_if_after_delay_input_pin_is_high_test);

    UNITY_END();
}

void loop()
{
}