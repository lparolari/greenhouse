#pragma once

#include <Arduino.h>

namespace greenhouse::brightness
{
    // @brief An interface to a moisture sensor
    // @tparam Pin The pin sensor is attached to
    // @tparam T The measurement type
    // @tparam AirValue The raw value of sensor in dry conditions (in the air)
    // @tparam WaterValue The raw value of sensor in wet conditions (in the water)
    template <uint8_t Pin = 0U, typename T = float, int LightValue = 250, int DarkValue = 0>
    class BrightnessSensor
    {
    private:
        int _value = 0;

    public:
        // @brief Reads the raw value
        void read()
        {
            _value = analogRead(Pin);
        }

        // @returns The percentage of moisture
        T get() const
        {
            return static_cast<T>(_value) / (LightValue - DarkValue) * 100;
        }

        // @returns The sensor's raw value
        int raw() const
        {
            return _value;
        }
    };
} // namespace greenhouse::brightness