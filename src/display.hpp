#pragma once

#include <stdarg.h>

#include <Arduino.h>
#include <LiquidCrystal.h>

namespace greenhouse::display
{
    LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

    template <int C = 16, int R = 2>
    class Display
    {
    public:
        Display() = default;

        void hello()
        {
            display("Hello, World!   :)");
        }

        void display(const String &s)
        {
            lcd.clear();

            for (int i = 0; i < R; i++)
            {
                int beginIndex = i * C;
                int endIndex = min((i + 1) * C, s.length());

                if (!(endIndex <= beginIndex))
                {
                    lcd.setCursor(0, i);
                    lcd.print(s.substring(beginIndex, endIndex)); // endIndex is exclusive
                }
            }
        }
    };
}
