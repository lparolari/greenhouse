#pragma once

#include <Arduino.h>
#include "pin.hpp"
#include "delay.hpp"

namespace greenhouse::waterpump
{

    enum State
    {
        OFF,
        ON
    };

    // @brief An interface to a moisture sensor
    // @tparam Pin The pin sensor is attached to
    template <uint8_t Pin = 0U>
    class WaterPump
    {
    private:
        greenhouse::pin::DigitalOutput<Pin> _control_pin;
        greenhouse::delay::Timer<3000> _poweroff_timer;

        State _state;

    public:
        WaterPump() : _state(State::OFF) {}

        void begin()
        {
            _control_pin.begin();
        }

        void tick(uint64_t ms)
        {
            if (_state == State::ON)
            {
                _control_pin.high();
                _poweroff_timer.tick(ms);

                if (_poweroff_timer.is_after())
                {
                    off();
                }
            }
            if (_state == State::OFF)
            {
                _control_pin.low();
                _poweroff_timer.reset();
            }
        }

        void on()
        {
            _state = State::ON;
        }

        void off()
        {
            _state = State::OFF;
        }
    };
} // namespace greenhouse::waterpump
