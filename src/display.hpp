#pragma once

#include <LiquidCrystal.h>
#include <string.h>

namespace display
{
    LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // LCD

    class Displayable
    {
        virtual void display() = 0;
    };

    class Display
    {
        void print_simple(const String &s)
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            // TODO 2 string split
        }
    };

    String buf[10];

    int i = 0;
    int j = 0;

    void hello()
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Hello, World!");
        lcd.setCursor(0, 1);
        lcd.print(":)");
    }

    void add_message(const String &s)
    {
        buf[i] = s;
        i = (i + 1) % 10;
    }

    void print()
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(buf[j]);
        j = (j + 1) % 10;
    }
} // namespace display
