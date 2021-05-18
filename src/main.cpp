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
#include "rtc.hpp"
#include "light.hpp"

using greenhouse::rtc::DateTime;
using greenhouse::utils::eps;

const bool debug = true;

greenhouse::display::Display<32, 31, 30, 29, 28, 27> display;
greenhouse::brightness::BrightnessSensor<0> brightness_sensor;
greenhouse::dht::Dht<23> dht;
greenhouse::waterpump::WaterPump<24> water_pump;
greenhouse::moisture::MoistureSensor<25> moisutre_sensor;
greenhouse::water::WaterSensor<26> water_sensor;
greenhouse::light::Light<33> light;

greenhouse::view::View<5> view;

greenhouse::delay::Delay<3000> display_delay;
greenhouse::delay::Delay<1000> debug_delay;
greenhouse::delay::Delay<100> button_delay;

greenhouse::delay::Timer<3000> waterpump_timer;

greenhouse::button::ToggleButton<60> display_slider_button;
greenhouse::button::DebounceButton<60> slide_button;
greenhouse::button::DebounceButton<52> waterpump_button;
greenhouse::button::ToggleButton<53> light_button;

greenhouse::rtc::Rtc<> rtc;

// Many (not so functional) functions which use global state and updates
// sensors or attuators. We are not so proud of this solution, and we argue
// that should be changed for program maintainability. However, up to now
// the only file who knows about components is this one (main.cpp).

// Read digital/analog values from sensors.
void forward_sensors();

// Forward all delays with current millis.
void forward_delays();

// Create update messages for LCD view.
void populate_view();

void waterPumpOnToggleHandler(bool, bool);
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
    light.begin();
    display_slider_button.begin();
    slide_button.begin();
    waterpump_button.begin();
    light_button.begin();
    rtc.begin();

    // hello
    greenhouse::serial::hello();
    display.hello();
    dht.sensor_specs();

    // set the alarm for watering the plant at 9 pm
    rtc.setAlarm(DateTime(0, 0, 0, 21, 00, 00), DS3231_A1_Hour);
}

void loop()
{
    forward_sensors();
    forward_delays();

    // trigger delayed actions for display
    if (display_delay.is_fire())
    {
        display_delay.fired();

        populate_view();
        viewScrollToggleHandler();

        display.display(view.get_message());
    }

    // trigger view slider based on button pressed
    if (button_delay.is_fire())
    {
        button_delay.fired();

        // A delay may not be needed in this case, however
        // we want to avoid noise (which also buttons handle).

        waterPumpOnToggleHandler(waterpump_button.pressed(), false);
        viewSliderToggleHandler();
        lightToggleHandler();
    }

    // trigger dalayed actions for debug
    if (debug_delay.is_fire() && debug)
    {
        debug_delay.fired();
        printDebugSerial();
    }

    // trigger watering
    if (rtc.alarmFired())
    {
        rtc.clearAlarm();
        waterPumpOnToggleHandler(false, true);
    }

    // wait 1 ms, prevent oversampling [without source]
    delay(1);
}

void forward_sensors()
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

    // tick also the water pump
    water_pump.tick(millis());
}

void forward_delays()
{
    auto ms = millis();

    display_delay.tick(ms);
    button_delay.tick(ms);
    debug_delay.tick(ms);
}

void populate_view()
{
    view.clear();
    view.add_message(eps + "Temp = " + dht.temperature() + " %");
    view.add_message(eps + "Humi = " + dht.relative_humidity() + " %");
    view.add_message(eps + "Wate = " + water_sensor.get() + " %" + " (" + water_sensor.raw() + ")");
    view.add_message(eps + "Brig = " + brightness_sensor.get() + " %" + " (" + brightness_sensor.raw() + ")");
    view.add_message(eps + "Mois = " + moisutre_sensor.get() + " %" + " (" + moisutre_sensor.raw() + ")");
}

void waterPumpOnToggleHandler(bool pressed, bool daily_watering)
{
    if (pressed || daily_watering)
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
        light.high();
    else
        light.low();
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
    Serial.println(eps + "Time = " + rtc.now().hour() + ":" + rtc.now().minute() + ":" + rtc.now().second());
    Serial.println(eps + "Alar = " + rtc.alarmFired());
}
