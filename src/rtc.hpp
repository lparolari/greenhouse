#pragma once

#include <Arduino.h>
#include <RTClib.h>

namespace greenhouse::rtc
{

    template <class RealTimeClock = RTC_DS3231>
    class Rtc
    {
    private:
        RealTimeClock _rtc;

    public:
        Rtc() = default;

        void begin()
        {
            // We need to follow many steps in order to initialize the RTC, as reference we follow
            // https://github.com/adafruit/RTClib/blob/master/examples/DS3231_alarm/DS3231_alarm.ino

            // Initialize RTC
            _rtc.begin();

            if (_rtc.lostPower())
            {
                // Aadjust to the date and time at compilation
                _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            }

            // Disable 32K pin as not needed
            _rtc.disable32K();

            // Set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
            // If not done, this easily leads to problems, as both register aren't reset on reboot/recompile
            _rtc.clearAlarm(1);
            _rtc.clearAlarm(2);

            // Stop oscillating signals at SQW Pin otherwise setAlarm1 will fail
            _rtc.writeSqwPinMode(DS3231_OFF);

            // Turn off alarm 2 (in case it isn't off already)
            // Again, this isn't done at reboot, so a previously set alarm could easily go overlooked
            _rtc.disableAlarm(2);
        }

        DateTime now() const
        {
            return _rtc.now();
        }

        void setAlarm(const DateTime &dt, int alarm_mode = DS3231_A1_Date)
        {
            _rtc.setAlarm1(dt, alarm_mode);
        }

        bool alarmFired()
        {
            return _rtc.alarmFired(1);
        }

        // @returns A reference to the raw implementation of RTC
        RealTimeClock &_raw_rtc()
        {
            return _rtc;
        }
    };

} // namespace greenhouse::rtc
