#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "sensor.hpp"

class MockInput
{
private:
    bool b = false;
    int v = 0;

public:
    void begin()
    {
        b = true;
    }
    void read()
    {
        v = 50;
    }

    int get() const
    {
        if (!b)
            return 0;
        return v;
    }
};

using PercentSensor = greenhouse::sensor::PercentSensor<0, MockInput, 0, 200, float>;

void sensor_returns_zero_if_not_init_test(void)
{
    PercentSensor s;

    TEST_ASSERT_EQUAL(0, s.get());
}

void sensor_returns_zero_if_not_read_test(void)
{
    PercentSensor s;
    s.begin();

    TEST_ASSERT_EQUAL(0, s.get());
}

void sensor_returns_same_raw_value_as_input_test(void)
{
    PercentSensor s;
    s.begin();

    s.read();

    TEST_ASSERT_EQUAL(50, s.raw());
}

void sensor_returns_correct_percentage_test(void)
{
    PercentSensor s;
    s.begin();

    s.read();

    TEST_ASSERT_EQUAL(25., s.get());
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(sensor_returns_zero_if_not_init_test);
    RUN_TEST(sensor_returns_zero_if_not_read_test);
    RUN_TEST(sensor_returns_same_raw_value_as_input_test);
    RUN_TEST(sensor_returns_correct_percentage_test);

    UNITY_END();
}

void loop()
{
}
