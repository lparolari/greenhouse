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

    // @brief A toggle button with protection agaist noise
    //
    // The button is based on a DFA with 5 states, 3 are main states and
    // 2 are transition states.
    //
    // Formal definition of DFA
    //
    // Alphabet = { low, high }
    // States = { off, off2on, on, maytoggle, maytoggle2off }
    // Initial state = off
    // Accepted state* = { on, maytoggle, maytoggle2off }
    // Transition function =
    //
    // State           Input   State'
    // -----           -----   ------
    // off*            low     off
    // off             high    off2on
    // off2on          low     off
    // off2on          high    on
    // on              low     maytoggle
    // on              high    on
    // maytoggle       low     maytoggle
    // maytoggle       high    maytoggle2off
    // maytoggle2off   low     maytoggle
    // maytoggle2off   high    off
    //
    // * As accepted states we mean states that makes the button pressed.
    //
    // @tparam Pin The button pin
    // @tparam DelayMs The delay in milliseconds to wait from a measurement
    //         and another before setting button state to pressed or not
    template <uint8_t Pin = 0, uint32_t DelayMs = 50>
    class ToggleButton
    {
    private:
        greenhouse::delay::Delay<DelayMs> _delay;
        uint8_t _state;

        static constexpr uint8_t _OFF = 0;
        static constexpr uint8_t _OFF_TO_ON = 1;
        static constexpr uint8_t _ON = 2;
        static constexpr uint8_t _MAY_TOGGLE = 3;
        static constexpr uint8_t _MAY_TOGGLE_TO_OFF = 4;

        uint8_t get_state(uint8_t state, int input) const
        {
            switch (state)
            {
            case _OFF:
                if (input == LOW)
                    return _OFF;
                if (input == HIGH)
                    return _OFF_TO_ON;
            case _OFF_TO_ON:
                if (input == LOW)
                    return _OFF;
                if (input == HIGH)
                    return _ON;
            case _ON:
                if (input == LOW)
                    return _MAY_TOGGLE;
                if (input == HIGH)
                    return _ON;
            case _MAY_TOGGLE:
                if (input == LOW)
                    return _MAY_TOGGLE;
                if (input == HIGH)
                    return _MAY_TOGGLE_TO_OFF;
            case _MAY_TOGGLE_TO_OFF:
                if (input == LOW)
                    return _MAY_TOGGLE;
                if (input == HIGH)
                    return _OFF;
            default:
                return _OFF;
            }
        }

    public:
        ToggleButton() : _state(_OFF)
        {
            pinMode(Pin, INPUT);
        }

        void read()
        {
            _delay.tick(millis());
            if (_delay.fire())
            {
                const int input = digitalRead(Pin);

                _state = get_state(_state, input);

                _delay.fired();
            }
        }

        bool pressed()
        {
            return _state == _ON || _state == _MAY_TOGGLE || _state == _MAY_TOGGLE_TO_OFF;
        }
    };

} // namespace greenhouse::button
