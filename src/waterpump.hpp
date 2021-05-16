#pragma once

#include <Arduino.h>
#include "pin.hpp"

namespace greenhouse::waterpump
{

    // @brief An interface to a moisture sensor
    // @tparam Pin The pin sensor is attached to
    template <uint8_t Pin = 0U>
    class WaterPump
    {
    private:
        OutputPin<Pin> _pin;

    public:
        void begin()
        {
            _pin.begin();
        }

        void on()
        {
            _pin.high();
        }

        void off()
        {
            _pin.low();
        }
    };
} // namespace greenhouse::waterpump
