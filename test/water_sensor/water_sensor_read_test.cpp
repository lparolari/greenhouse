#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "utils.hpp"
#include "water.hpp"

using greenhouse::utils::eps;

greenhouse::water::WaterSensor<3> water_sensor;

uint8_t i = 0;
const uint8_t max_reads = 5;
const uint8_t err = 5; // error permitted in analog read

void test_water_sensor_is_0_before_read(void)
{
    TEST_ASSERT_EQUAL(water_sensor.raw(), 0);
}

void test_water_sensor_read_from_analog_pin(void)
{
    water_sensor.read();

    int value_pin = analogRead(3);
    int value_sensor = water_sensor.raw();

    TEST_ASSERT_TRUE(abs(value_sensor - value_pin) < err);
    TEST_ASSERT_NOT_EQUAL(value_sensor, 0);
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_water_sensor_is_0_before_read);
}

void loop()
{
    if (i < max_reads)
    {
        RUN_TEST(test_water_sensor_read_from_analog_pin);
        i++;
    }
    else
    {
        UNITY_END(); // stop unit testing
    }

    delay(100);
}
