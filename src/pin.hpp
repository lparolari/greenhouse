#pragma once

#include <Arduino.h>

namespace greenhouse::pin
{
    // @brief A digital output pin
    // @tparam Pin Number of pin
    // @tparam Inverted Inverted output: low is high and high is low
    template <uint8_t Pin = 0, bool Inverted = false>
    class DigitalOutput
    {
    private:
        uint8_t _state;

    public:
        DigitalOutput() : _state(Inverted ? HIGH : LOW) {}

        void begin()
        {
            pinMode(Pin, OUTPUT);
            apply();
        }

        // @brief Set pin value to low (i.e., 0)
        void low()
        {
            _state = Inverted ? HIGH : LOW;
        }

        // @brief Set pin value to high (i.e., 1)
        void high()
        {
            _state = Inverted ? LOW : HIGH;
        }

        // @brief Switch button state
        void toggle()
        {
            _state = _state == LOW ? HIGH : LOW;
        }

        // @brief Forward the component
        void apply() const
        {
            digitalWrite(Pin, _state);
        }
    };

    // @brief A digital input pin
    // @tparam Pin Number of pin
    template <uint8_t Pin = 0>
    class DigitalInput
    {
    private:
        int _value;

    public:
        DigitalInput() : _value(LOW){};

        void begin()
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
    class AnalogInput
    {
    private:
        int _value;

    public:
        AnalogInput() : _value(0){};

        void begin()
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
