#include <Arduino.h>

namespace greenhouse::serial
{
    HardwareSerial serial = Serial;

    void hello()
    {
        Serial.println("Hello, World!");
    }
} // namespace greenhouse::serial
