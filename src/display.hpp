#pragma once

#include <stdarg.h>

#include <Arduino.h>
#include <LiquidCrystal.h>

namespace greenhouse::display
{
    // LiquidCrystal lcd(7, ) // (7, 8, 9, 10, 11, 12);

    template <uint8_t rs = 7, uint8_t enable = 8,
              uint8_t d0 = 9, uint8_t d1 = 10, uint8_t d2 = 11, uint8_t d3 = 12,
              int C = 16, int R = 2>
    class Display
    {
    public:
        LiquidCrystal _lcd;

        Display() : _lcd(rs, enable, d0, d1, d2, d3){};

        void hello()
        {
            display("Hello, World!   :)");
        }

        void display(const String &s)
        {
            _lcd.clear();

            for (int i = 0; i < R; i++)
            {
                int beginIndex = i * C;
                int endIndex = min((i + 1) * C, s.length());

                if (!(endIndex <= beginIndex))
                {
                    _lcd.setCursor(0, i);
                    _lcd.print(s.substring(beginIndex, endIndex)); // endIndex is exclusive
                }
            }
        }
    };
}
