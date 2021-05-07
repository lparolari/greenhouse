#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "utils.hpp"
#include "moisture.hpp"

using greenhouse::utils::eps;

const uint8_t pin = 1;
uint8_t i = 0;
const uint8_t max_reads = 5;
const uint8_t err = 5; // error permitted in analog read

greenhouse::moisture::MoistureSensor<pin> moisture_sensor;

void test_moisture_sensor_is_0_before_read(void)
{
    TEST_ASSERT_EQUAL(moisture_sensor.raw(), 0);
}

void test_moisture_sensor_read_from_analog_pin(void)
{
    moisture_sensor.read();

    int value_pin = analogRead(pin);
    int value_sensor = moisture_sensor.raw();

    TEST_ASSERT_INT_WITHIN(err, value_pin, value_sensor);
    TEST_ASSERT_NOT_EQUAL(value_sensor, 0);
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_moisture_sensor_is_0_before_read);
}

void loop()
{
    if (i < max_reads)
    {
        RUN_TEST(test_moisture_sensor_read_from_analog_pin);
        i++;
    }
    else
    {
        UNITY_END(); // stop unit testing
    }

    delay(100);
}
