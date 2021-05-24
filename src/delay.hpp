#pragma once

namespace greenhouse::delay
{
    // @brief Fires after a delay of time
    // @tparam DelayAmount The delay before fire
    template <uint64_t DelayAmount>
    class Delay
    {
    private:
        uint64_t _current_time = 0;
        uint64_t _previous_time = 0;

    public:
        // @brief Forward the delay
        void tick(uint64_t current_time)
        {
            _current_time = current_time;
        }

        // @brief Reset the delay
        void clear()
        {
            _previous_time = _current_time;
        }

        // @brief Return true if `t_c - t_p > d` where `t_c` is the current
        // time, `t_p` is the previous time and `d` is the delay
        // @returns A boolean value representing delay fired or not
        bool is_fired()
        {
            return (_current_time - _previous_time) > DelayAmount;
        }

        // @deprecated Use `is_fired` instead
        bool is_fire()
        {
            return is_fired();
        }

        // @deprecated Use `clear` instead
        void fired()
        {
            clear();
        }
    };

    // TODO: the timer is the same as delay with a small difference: it initlizes "_previous_time" to first value in tick (not zero!)
    template <int TimerMs>
    class Timer
    {
    private:
        bool first = true;
        int start_millis = 0;
        int current_millis = 0;

    public:
        void tick(int ms)
        {
            if (first)
            {
                start_millis = ms;
                first = false;
            }
            current_millis = ms;
        }

        bool is_after() const
        {
            return current_millis > start_millis + TimerMs;
        }

        void reset()
        {
            first = true;
        }
    };
};
