#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "utils.hpp"
#include "waterpump.hpp"

using greenhouse::utils::eps;

greenhouse::waterpump::WaterPump<3> water_pump;

uint8_t i = 0;
const uint8_t max_reads = 5;

void test_pump_on(void)
{
    water_pump.on();
    TEST_ASSERT_EQUAL(digitalRead(3), HIGH);
}

void test_pump_off(void)
{
    water_pump.off();
    TEST_ASSERT_EQUAL(digitalRead(3), LOW);
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);
    UNITY_BEGIN();

    // init
    water_pump.begin();
}

void loop()
{

    if (i < max_reads)
    {
        RUN_TEST(test_pump_on);
        delay(500);
        RUN_TEST(test_pump_off);
        delay(500);
        i++;
    }
    else if (i == max_reads)
    {
        UNITY_END(); // stop unit testing
    }
}
