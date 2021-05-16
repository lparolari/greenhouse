#pragma once

#include <Arduino.h>

#include "pin.hpp"
#include "sensor.hpp"

namespace greenhouse::water
{

    // @brief The water sensor
    // @tparam Pin The (analog) pin number
    // @tparam AirValue The raw value of sensor in dry conditions (in the air)
    // @tparam WaterValue The raw value of sensor in wet conditions (in the water)
    template <uint8_t Pin = 0U, int AirValue = 250, int WaterValue = 0>
    using WaterSensor = greenhouse::sensor::PercentSensor<Pin, greenhouse::pin::AnalogInput<Pin>, WaterValue, AirValue, float>;

} // namespace greenhouse::brightness
