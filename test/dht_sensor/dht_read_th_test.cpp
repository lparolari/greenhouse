#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "utils.hpp"
#include "dht.hpp"

using greenhouse::utils::eps;

const uint8_t pin = 2;
const uint8_t max_reads = 5;
uint8_t i = 0;

// Please note that DHT11 sensors have very low accuracy.
// To define a good error range we should follow their specification details.

const float err_temp = 0.5; // Temp should be +/- 0.5 C wrt raw sensor
const float err_humi = 5.;  // Humidity should be +/- 5 % wrt raw sensor

greenhouse::dht::Dht<pin> dht_sensor;

void test_read_temperature(void)
{
    sensors_event_t event;
    dht_sensor._dht.temperature().getEvent(&event);

    float value_raw = event.temperature;
    float value_sensor = dht_sensor.temperature();

    TEST_ASSERT_FLOAT_WITHIN(err_temp, value_raw, value_sensor);
    TEST_ASSERT_NOT_EQUAL(0, value_sensor);
}

void test_read_humidity(void)
{
    sensors_event_t event;
    dht_sensor._dht.humidity().getEvent(&event);

    float value_raw = event.relative_humidity;
    float value_sensor = dht_sensor.relative_humidity();

    TEST_ASSERT_FLOAT_WITHIN(err_humi, value_raw, value_sensor);
    TEST_ASSERT_NOT_EQUAL(0, value_sensor);
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);
    UNITY_BEGIN();

    // init
    dht_sensor._dht.begin();
}

void loop()
{
    if (i < max_reads)
    {
        RUN_TEST(test_read_temperature);
        RUN_TEST(test_read_humidity);
        i++;
    }
    else
    {
        UNITY_END(); // stop unit testing
    }

    delay(100);
}
