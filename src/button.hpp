#pragma once

#include <Arduino.h>
#include "delay.hpp"

namespace greenhouse::button
{

    // @brief A debounce button, i.e., with noise cancellation.
    //
    // As stated in https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
    // we need to check two times within a (short) period of time for HIGH
    // input on pin before setting the button state to pressed. This prevents
    // noise and undefined behaviours.
    //
    // @tparam Pin The button pin
    // @tparam DelayMs The delay in milliseconds to wait from a measurement
    //         and another before setting button state to pressed or not
    template <uint8_t Pin = 0, uint32_t DelayMs = 50>
    class DebounceButton
    {
    private:
        greenhouse::delay::Delay<DelayMs> _delay;
        int _value;
        int _prev_value;

    public:
        DebounceButton() : _value(LOW), _prev_value(LOW)
        {
            pinMode(Pin, INPUT);
        }

        void read()
        {
            _delay.tick(millis());
            if (_delay.fire())
            {
                _prev_value = _value;
                _value = digitalRead(Pin);
                _delay.fired();
            }
        }

        int raw()
        {
            return _value;
        }

        bool pressed()
        {
            // Conservative for HIGH in transition states
            return _prev_value == HIGH && _value == HIGH;
        }
    };

} // namespace greenhouse::button
