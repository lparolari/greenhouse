#pragma once

#include <Arduino.h>
#include "delay.hpp"
#include "pin.hpp"

namespace greenhouse::button
{

    // @brief A debounce button, i.e., button with noise cancellation.
    //
    // As stated in https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
    // we need to check two times within a (short) period of time for HIGH
    // input on pin before setting the button state to pressed. This prevents
    // noise and undefined behaviours.
    //
    // @tparam Pin The button pin
    // @tparam DelayMs The delay in milliseconds to wait from a measurement
    //         and another before setting button state to pressed or not
    template <class InputPin, class Delay>
    class _DebounceButton
    {
    private:
        Delay _delay;
        InputPin _input_pin;
        int _value;
        int _prev_value;

    public:
        _DebounceButton() : _value(LOW), _prev_value(LOW)
        {
        }

        // @brief Initializes the componet
        void begin()
        {
            _input_pin.begin();
        }

        // @brief Forward the delay
        void tick(uint64_t ms = millis())
        {
            _delay.tick(ms);
            if (_delay.is_fire())
            {
                _prev_value = _value;
                _value = _input_pin.get();
                _delay.fired();
            }
        }

        // @brief Reads sensor data
        void read()
        {
            _input_pin.read();
        }

        // @brief Returns whether the button is pressed or not. The button is
        // pressed if we receive two subsequent high values within the delay
        // time.
        // @returns A boolean value representig button pressed or not
        bool pressed() const
        {
            // Conservative for HIGH in transition states
            return _prev_value == HIGH && _value == HIGH;
        }
    };

    template <uint8_t Pin = 0, uint64_t DelayMs = 50>
    using DebounceButton = _DebounceButton<greenhouse::pin::DigitalInput<Pin>, greenhouse::delay::Delay<DelayMs>>;

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
    // off             low     off
    // off             high    off2on
    // off2on          low     off
    // off2on          high    on
    // on              low     maytoggle
    // on              high    on
    // maytoggle       low     maytoggle
    // maytoggle       high    maytoggle2off
    // maytoggle2off   low     maytoggle
    // maytoggle2off   high    keepoff
    // keepoff         low     off
    // keepoff         high    keepoff
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
        static constexpr uint8_t _KEEP_OFF = 5;

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
                    return _KEEP_OFF;
            case _KEEP_OFF:
                if (input == LOW)
                    return _OFF;
                if (input == HIGH)
                    return _KEEP_OFF;
            default:
                return _OFF;
            }
        }

    public:
        ToggleButton() : _state(_OFF)
        {
        }

        // @brief Initializes the componet. To be called in Arduino's `setup()` function.
        void begin()
        {
            pinMode(Pin, INPUT);
        }

        void read()
        {
            _delay.tick(millis());
            if (_delay.is_fire())
            {
                const int input = digitalRead(Pin);

                _state = get_state(_state, input);

                _delay.fired();
            }
        }

        bool pressed() const
        {
            return _state == _ON || _state == _MAY_TOGGLE || _state == _MAY_TOGGLE_TO_OFF;
        }
    };

    // @brief A simple button with *pressed* and *released* events.
    //
    // Emits *pressed* event when there is a change from low to high in input
    // sequence, if instead there is a change from high to low it emits
    // *released* event. When the signal is stationary no event is emitted.
    //
    // More precisely, this component implements a convolution between three values
    // of the sequence and filter 1x2. The filter is [-1, 1]. For first value of
    // convolution (when not enough data is available) convolution is executed with
    // zero padding.
    //
    // @tparam InputSequence Is a sequence (x)_t, where x is in {0, 1} while t
    // is a natural number.
    template <class InputSequence>
    class _EventButton
    {
    private:
        InputSequence _input_sequence;
        int values[2] = {0, 0};
        int filter[2] = {-1, 1};

        // @brief Computes the convolution between three values in the input
        // sequence and and [-1, 1] filter.
        int conv() const
        {
            return values[0] * filter[0] + values[1] * filter[1];
        }

    public:
        // @brief Initializes the component.
        void begin()
        {
            _input_sequence.begin();
        }

        // @brief Reads input sequence value and memorize it in the buffer.
        void read()
        {
            _input_sequence.read();

            int x_t = _input_sequence.pressed() ? 1 : 0;

            values[0] = values[1];
            values[1] = x_t;
        }

        // @brief Ticks the component.
        void tick(uint64_t ms = millis())
        {
            _input_sequence.tick(ms);
        }

        // @brief Returns true if the button is pressed, false otherwise.
        // @returns A boolean value representing button pressed event.
        bool pressed() const
        {
            return conv() == 1;
        }

        // @brief Returns true if the button is released, false otherwise.
        // @returns A boolean value representing button released event.
        bool released() const
        {
            return conv() == -1;
        }
    };

    template <uint8_t Pin = 0, uint64_t DelayMs = 50>
    using EventButton = _EventButton<DebounceButton<Pin, DelayMs>>;

} // namespace greenhouse::button
