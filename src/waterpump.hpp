#pragma once

#include <Arduino.h>

namespace greenhouse::waterpump
{
    // @brief An interface to a moisture sensor
    // @tparam Pin The pin sensor is attached to
    template <uint8_t Pin = 0U>
    class WaterPump
    {
    public:
        // @brief Initializes the componet. To be called in Arduino's `setup()` function.
        void begin()
        {
            pinMode(Pin, OUTPUT);
            digitalWrite(Pin, LOW);
        }

        void on()
        {
            digitalWrite(Pin, HIGH);
        }

        void off()
        {
            digitalWrite(Pin, LOW);
        }
    };
} // namespace greenhouse::waterpump
