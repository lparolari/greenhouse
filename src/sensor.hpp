#pragma once

#include <Arduino.h>

#include "pin.hpp"

namespace greenhouse::sensor
{
    template <uint8_t Pin>
    using AnalogInput = greenhouse::pin::AnalogInput<Pin>;

    // @brief A sensor interface.
    // Represents a sensor which retrieve data in form of percents,
    // e.g., moisture, brightness, humidity, an many more.
    // @tparam Pin The pin number
    // @tparam InputPin The type of the input pin to use (e.g., AnalogInput or DigitalInput)
    // @tparam MinVal The minimum value of the scale
    // @tparam MaxVal The maximum value of the scale
    // @tparam T The measurement type
    template <uint8_t Pin = 0U, typename InputPin = AnalogInput<Pin>, int MinVal = 0, int MaxVal = 100, typename T = float>
    class PercentSensor
    {
    private:
        InputPin input;

    public:
        // @brief Initializes the sensor
        void begin()
        {
            input.begin();
        }

        // @brief Reads the raw value
        void read()
        {
            input.read();
        };

        // @brief Computes the percentage of value scaled to max and min value.
        // Given $x$ the value, returns $x / (max - min) * 100$.
        // @returns The percentage of value among max and min val
        T get() const
        {
            return static_cast<T>(input.get()) / (MaxVal - MinVal) * 100;
        };

        // @returns The sensor's raw value
        int raw() const
        {
            return input.get();
        };
    };
} // namespace greenhouse::sensor
