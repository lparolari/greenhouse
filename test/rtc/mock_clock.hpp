#include "RTClib.h"

class MockClock
{
public:
    void begin() {}
    bool lostPower() { return false; }
    void adjust(const DateTime &d) {}
    DateTime now() const { return DateTime(2021, 05, 16, 17, 20, 0); }
    void disable32K() {}
    void clearAlarm(int x) {}
    void disableAlarm(int x) {}
    void writeSqwPinMode(int x) {}
};
