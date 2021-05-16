#pragma once

#include <Arduino.h>

#include "component.hpp"

namespace greenhouse::pin
{
    using Component = greenhouse::component::Component;

    // @brief A digital output pin
    // @tparam Pin Number of pin
    template <uint8_t Pin = 0>
    class DigitalOutput : public Component
    {
    public:
        DigitalOutput() = default;

        void begin() override
        {
            pinMode(Pin, OUTPUT);
            digitalWrite(Pin, LOW);
        }

        // @brief Set pin value to low (i.e., 0)
        void low()
        {
            digitalWrite(Pin, LOW);
        }

        // @brief Set pin value to high (i.e., 1)
        void high()
        {
            digitalWrite(Pin, HIGH);
        }
    };

    // @brief A digital input pin
    // @tparam Pin Number of pin
    template <uint8_t Pin = 0>
    class DigitalInput : public Component
    {
    private:
        int _value;

    public:
        DigitalInput() : _value(LOW){};

        void begin() override
        {
            pinMode(Pin, INPUT);
        }

        // @brief Read value from pin and store it
        void read()
        {
            _value = digitalRead(Pin);
        }

        // @brief Return stored value
        // @returns A integer representing either low or high values with 0 or 1
        int get() const
        {
            return _value;
        }
    };

    // @brief An analog input pin
    // @tparam Pin Number of pin
    template <uint8_t Pin = 0>
    class AnalogInput : public Component
    {
    private:
        int _value;

    public:
        AnalogInput() : _value(0){};

        void begin() override
        {
            pinMode(Pin, INPUT);
        }

        // @brief Read value from pin and store it
        void read()
        {
            _value = analogRead(Pin);
        }

        // @brief Return stored value
        // @returns A integer between 0 and 1023
        int get() const
        {
            return _value;
        }
    };
} // namespace greenhouse::pin
