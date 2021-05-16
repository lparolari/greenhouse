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
#include "pin.hpp"

using greenhouse::utils::eps;

const bool debug = true;

greenhouse::display::Display<7, 8, 9, 10, 11, 12> display;
greenhouse::dht::Dht<2> dht;
greenhouse::waterpump::WaterPump<23> water_pump;
greenhouse::moisture::MoistureSensor<1> moisutre_sensor;
greenhouse::brightness::BrightnessSensor<2> brightness_sensor;
greenhouse::water::WaterSensor<3> water_sensor;
greenhouse::pin::DigitalOutput<22> light_pin; // TODO: change pin

greenhouse::view::View<5> view;

greenhouse::delay::Delay<3000> display_delay;
greenhouse::delay::Delay<1000> debug_delay;
greenhouse::delay::Delay<100> button_delay;

greenhouse::button::ToggleButton<53> display_slider_button;
greenhouse::button::DebounceButton<52> slide_button;
greenhouse::button::DebounceButton<51> waterpump_button;
greenhouse::button::ToggleButton<50> light_button;

// Many (not functional) functions, better proceudres.
// This should be changed for program maintainability. However, up to now
// the only file who knows about components is this one (main.cpp).

void waterPumpOnToggleHandler();
void viewScrollToggleHandler();
void viewSliderToggleHandler();
void lightToggleHandler();
void printDebugSerial();

void setup()
{
    // init
    greenhouse::serial::serial.begin(9600);
    display._lcd.begin(16, 2);
    dht._dht.begin();
    water_pump.begin();
    moisutre_sensor.begin();
    brightness_sensor.begin();
    water_sensor.begin();
    light_pin.begin();
    display_slider_button.begin();
    slide_button.begin();
    waterpump_button.begin();
    light_button.begin();

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

    // read also button status
    display_slider_button.read();
    slide_button.read();
    waterpump_button.read();
    light_button.read();

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
    if (display_delay.is_fire())
    {
        display_delay.fired();

        viewScrollToggleHandler();

        display.display(view.get_message());
    }

    // trigger view slider based on button pressed
    if (button_delay.is_fire())
    {
        button_delay.fired();

        // A delay may not be needed in this case, however
        // we want to avoid noise (which also buttons handle).

        waterPumpOnToggleHandler();
        viewSliderToggleHandler();
        lightToggleHandler();
    }

    // trigger dalayed actions for debug
    if (debug_delay.is_fire() && debug)
    {
        debug_delay.fired();
        printDebugSerial();
    }

    // wait 1 ms, prevent oversampling [without source]
    delay(1);
}

void waterPumpOnToggleHandler()
{
    if (waterpump_button.pressed())
        water_pump.on();
    else
        water_pump.off();
}

void viewScrollToggleHandler()
{
    if (slide_button.pressed())
        view.next(true);
    else
        view.next();
}

void viewSliderToggleHandler()
{
    if (display_slider_button.pressed())
        view.disable();
    else
        view.enable();
}

void lightToggleHandler()
{
    if (light_button.pressed())
        light_pin.high();
    else
        light_pin.low();
}

void printDebugSerial()
{
    Serial.println("=======");
    Serial.println(eps + "Temp = " + dht.temperature());
    Serial.println(eps + "Humi = " + dht.relative_humidity());
    Serial.println(eps + "Wate = " + water_sensor.get() + " (" + water_sensor.raw() + ")");
    Serial.println(eps + "Brig = " + brightness_sensor.get() + " (" + brightness_sensor.raw() + ")");
    Serial.println(eps + "Mois = " + moisutre_sensor.get() + " (" + moisutre_sensor.raw() + ")");
    Serial.println(eps + "Slid = " + display_slider_button.pressed());
    Serial.println(eps + "Next = " + slide_button.pressed());
    Serial.println(eps + "Wate = " + waterpump_button.pressed());
    Serial.println(eps + "Ligh = " + light_button.pressed());
}
