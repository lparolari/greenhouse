#pragma once

namespace greenhouse::delay
{
    // @brief Delays the exectution of actions in a imperative world without callbacks.
    // @tparam DelayMs The delay in milliseconds to when the clock should fire
    template <int DelayMs>
    class Delay
    {
    private:
        int current_millis = 0;
        int previous_millis = 0;

    public:
        void tick(int ms)
        {
            current_millis = ms;
        }

        bool is_fire()
        {
            return (current_millis - previous_millis) > DelayMs;
        }

        void fired()
        {
            previous_millis = current_millis;
        }
    };
};
