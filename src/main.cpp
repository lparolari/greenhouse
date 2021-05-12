#include <Arduino.h>
#include <Wire.h>

#include "utils.hpp"
#include "serial.hpp"
#include "display.hpp"
#include "dht.hpp"
#include "water.hpp"
#include "brightness.hpp"
#include "moisture.hpp"
#include "waterpump.hpp"
#include "view.hpp"
#include "delay.hpp"
#include "button.hpp"

// Sensors:
// * [X] brightness
// * [X] water
// * [X] moisture
// * [X] temperature
// * [X] humidity
// * [ ] real time clock

// Attuators:
// * [X] serial
// * [X] display
// * [X] water pump

using greenhouse::utils::eps;

const bool debug = true;

greenhouse::display::Display<7, 8, 9, 10, 11, 12> display;
greenhouse::dht::Dht<2> dht;
greenhouse::waterpump::WaterPump<3> water_pump;
greenhouse::moisture::MoistureSensor<1> moisutre_sensor;
greenhouse::brightness::BrightnessSensor<2> brightness_sensor;
greenhouse::water::WaterSensor<3> water_sensor;

greenhouse::view::View<5> view;

greenhouse::delay::Delay<3000> display_delay;
greenhouse::delay::Delay<1000> debug_delay;
greenhouse::delay::Delay<100> button_delay;

greenhouse::button::ToggleButton<22> toggle_display_slider_button;
greenhouse::button::DebounceButton<23> toggle_slide_button;

void setup()
{
    // init
    greenhouse::serial::serial.begin(9600);
    display._lcd.begin(16, 2);
    dht._dht.begin();
    water_pump.begin();

    // hello
    greenhouse::serial::hello();
    display.hello();
    dht.sensor_specs();
}

void loop()
{
    // get value from sensor at every loop, may be delayed
    // (see sensor's sampling frequency)
    water_sensor.read();
    brightness_sensor.read();
    moisutre_sensor.read();

    toggle_display_slider_button.read();
    toggle_slide_button.read();

    // forward delays
    auto ms = millis();

    display_delay.tick(ms);
    button_delay.tick(ms);
    debug_delay.tick(ms);

    // prepare the view with the display
    view.clear();
    view.add_message(eps + "Temp = " + dht.temperature() + " %");
    view.add_message(eps + "Humi = " + dht.relative_humidity() + " %");
    view.add_message(eps + "Wate = " + water_sensor.get() + " %" + " (" + water_sensor.raw() + ")");
    view.add_message(eps + "Brig = " + brightness_sensor.get() + " %" + " (" + brightness_sensor.raw() + ")");
    view.add_message(eps + "Mois = " + moisutre_sensor.get() + " %" + " (" + moisutre_sensor.raw() + ")");

    // trigger delayed actions for display
    if (display_delay.fire())
    {
        display_delay.fired();

        if (toggle_slide_button.pressed())
            view.next(true);
        else
            view.next();

        display.display(view.get_message());
    }

    // trigger view slider based on button pressed
    if (button_delay.fire())
    {
        button_delay.fired();

        if (toggle_display_slider_button.pressed())
        {
            view.disable();
        }
        else
            view.enable();
    }

    // trigger dalayed actions for debug
    if (debug_delay.fire() && debug)
    {
        debug_delay.fired();
        Serial.println("=======");
        Serial.println(eps + "Temp = " + dht.temperature());
        Serial.println(eps + "Humi = " + dht.relative_humidity());
        Serial.println(eps + "Wate = " + water_sensor.get() + " (" + water_sensor.raw() + ")");
        Serial.println(eps + "Brig = " + brightness_sensor.get() + " (" + brightness_sensor.raw() + ")");
        Serial.println(eps + "Mois = " + moisutre_sensor.get() + " (" + moisutre_sensor.raw() + ")");
        Serial.println(eps + "Slid = " + toggle_display_slider_button.pressed());
        Serial.println(eps + "Next = " + toggle_slide_button.pressed());
    }

    // wait 100 ms, prevent oversampling (source?)
    delay(100);
}
