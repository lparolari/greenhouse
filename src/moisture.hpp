#pragma once

#include <Arduino.h>

#include "pin.hpp"
#include "sensor.hpp"

namespace greenhouse::moisture
{

    // @brief The brightness sensor
    // @tparam Pin The (analog) pin number
    // @tparam AirValue The raw value of sensor in dry conditions (in the air)
    // @tparam WaterValue The raw value of sensor in wet conditions (in the water)
    template <uint8_t Pin = 0U, int AirValue = 530, int WaterValue = 230>
    using MoistureSensor = greenhouse::sensor::PercentSensor<Pin, greenhouse::pin::AnalogInput<Pin>, WaterValue, AirValue, float>;

} // namespace greenhouse::moisture
