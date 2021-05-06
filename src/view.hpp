#pragma once

#include <Arduino.h>
#include "display.hpp"
#include "utils.hpp"

namespace greenhouse::view
{
    template <int N>
    class View
    {
    private:
        String _messages[N];
        int _n;          // how many messages have been inserted
        int _i;          // the message to show
        bool _slideshow; // whether slideshow is enabled

    public:
        View() : _n(0), _i(0), _slideshow(true)
        {
        }

        void add_message(const String &msg)
        {
            if (_n < 0 || _n >= N)
                return;

            _messages[_n] = msg;
            ++_n;
        }

        String get_message() const
        {
            if (_i < 0 || _i >= N)
                return greenhouse::utils::eps;

            return _messages[_i];
        }

        void next(bool force = false)
        {
            if (!_slideshow && !force)
                return;

            _i = (_i + 1) % N;
        }

        void clear()
        {
            _n = 0;
        }

        void enable()
        {
            _slideshow = true;
        }

        void disable()
        {
            _slideshow = false;
        }
    };
} // namespace greenhouse::view
