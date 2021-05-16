#pragma once

#include <Arduino.h>

#include "pin.hpp"
#include "sensor.hpp"

namespace greenhouse::brightness
{

    // @brief The brightness sensor
    // @tparam Pin The number of (analog) pin
    // @tparam DarkValue Min value of the sensor (dark)
    // @tparam LightValue Max value of the sensor (ligh)
    template <uint8_t Pin = 0U, int LightValue = 250, int DarkValue = 0>
    using BrightnessSensor = greenhouse::sensor::PercentSensor<Pin, greenhouse::pin::AnalogInput<Pin>, DarkValue, LightValue, float>;

} // namespace greenhouse::brightness
