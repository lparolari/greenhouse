#pragma once

#include <Arduino.h>
#include "pin.hpp"

namespace greenhouse::light
{
    template <uint8_t Pin>
    using DigitalOutput = greenhouse::pin::DigitalOutput<Pin>;

    // @brief
    // This type of relays work inverted
    // (source https://raspberrypi.stackexchange.com/a/73313)
    // @tparam Pin Number of pin
    // @tparam Output The type of the output pin to use
    template <uint8_t Pin = 0, class OutputPin = DigitalOutput<Pin>>
    class Light
    {
    private:
        OutputPin _output_pin;

    public:
        void begin()
        {
            _output_pin.begin();
            low();
        }

        void high()
        {
            _output_pin.low();
        }

        void low()
        {
            _output_pin.high();
        }
    };

} // namespace greenhouse::light
