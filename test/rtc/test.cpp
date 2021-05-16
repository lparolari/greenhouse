#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "rtc.hpp"
#include "mock_clock.hpp"

using RtcReal = greenhouse::rtc::Rtc<RTC_DS3231>;
using RtcDummy = greenhouse::rtc::Rtc<MockClock>;

RtcReal rtc_real;
RtcDummy rtc_dummy;

void rtc_returns_same_date_as_clock_test(void)
{
    TEST_ASSERT_EQUAL(2021, rtc_dummy.now().year());
    TEST_ASSERT_EQUAL(05, rtc_dummy.now().month());
    TEST_ASSERT_EQUAL(16, rtc_dummy.now().day());
    TEST_ASSERT_EQUAL(17, rtc_dummy.now().hour());
    TEST_ASSERT_EQUAL(20, rtc_dummy.now().minute());
    TEST_ASSERT_EQUAL(00, rtc_dummy.now().second());
}

void rtc_fires_alarm_in_time_test(void)
{
    rtc_real.begin();

    int t = 0; // time

    // Alarm should not be already fired
    TEST_ASSERT_FALSE(rtc_real.alarmFired());

    // Set the alarm to 1 sec from now
    rtc_real.setAlarm(rtc_real._raw_rtc().now() + TimeSpan(1));

    delay(t += 250);

    // Alarm should not be fired before time
    TEST_ASSERT_FALSE(rtc_real.alarmFired());

    delay(t += 750 + 50); // keep a margin of 50 ms

    // Alarm should now be fired
    TEST_ASSERT_TRUE(rtc_real.alarmFired());
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);
    UNITY_BEGIN();

    rtc_real.begin();
    rtc_dummy.begin();

    RUN_TEST(rtc_returns_same_date_as_clock_test);
    RUN_TEST(rtc_fires_alarm_in_time_test);

    UNITY_END();
}

void loop()
{
}
