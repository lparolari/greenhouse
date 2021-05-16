#pragma once

#include <Arduino.h>

namespace greenhouse::pin
{
    template <uint8_t Pin = 0>
    class OutputPin
    {
    public:
        OutputPin() = default;

        // @brief Initializes the componet. To be called in Arduino's `setup()` function.
        void begin()
        {
            pinMode(Pin, OUTPUT);
            digitalWrite(Pin, LOW);
        }

        void low()
        {
            digitalWrite(Pin, LOW);
        }

        void high()
        {
            digitalWrite(Pin, HIGH);
        }
    };
} // namespace greenhouse::pin
