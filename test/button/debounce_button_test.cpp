#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "button.hpp"

const uint8_t out_pin = 10;    // simulate button open/closed
const uint8_t button_pin = 11; // read button value
const uint8_t button_delay = 50;

void button_input_is_high_test(void)
{
    pinMode(button_pin, INPUT);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(button_pin));
}

void button_is_not_pressed_if_value_not_read_test(void)
{
    greenhouse::button::DebounceButton<button_pin> debounce_button;
    debounce_button.begin();

    TEST_ASSERT_FALSE(debounce_button.pressed());
}

void button_is_not_pressed_after_first_read_with_high_value_test(void)
{
    greenhouse::button::DebounceButton<button_pin> debounce_button;
    debounce_button.begin();

    debounce_button.read();
    debounce_button.tick();
    TEST_ASSERT_FALSE(debounce_button.pressed());
}

void button_is_pressed_if_after_delay_input_pin_is_high_test(void)
{
    greenhouse::button::DebounceButton<button_pin, button_delay> debounce_button;
    debounce_button.begin();

    debounce_button.read();
    debounce_button.tick();
    TEST_ASSERT_FALSE(debounce_button.pressed());

    delay(25);

    debounce_button.read();
    debounce_button.tick();
    TEST_ASSERT_FALSE(debounce_button.pressed());

    delay(50);

    debounce_button.read();
    debounce_button.tick();
    TEST_ASSERT_TRUE(debounce_button.pressed());
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);
    Serial.begin(9600);
    UNITY_BEGIN();

    pinMode(out_pin, OUTPUT);
    digitalWrite(out_pin, HIGH);

    RUN_TEST(button_input_is_high_test);
    RUN_TEST(button_is_not_pressed_if_value_not_read_test);
    RUN_TEST(button_is_not_pressed_after_first_read_with_high_value_test);
    RUN_TEST(button_is_pressed_if_after_delay_input_pin_is_high_test);

    UNITY_END();
}

void loop()
{
}
