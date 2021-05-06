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

greenhouse::display::Display<7, 8, 9, 10, 11, 12> display;
greenhouse::dht::Dht<2> dht;
greenhouse::waterpump::WaterPump<3> water_pump;
greenhouse::moisture::MoistureSensor<1> moisutre_sensor;
greenhouse::brightness::BrightnessSensor<2> brightness_sensor;
greenhouse::water::WaterSensor<3> water_sensor;

greenhouse::view::View<5> view;

unsigned long previous_millis = 0;
unsigned long current_millis = 0;

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
    water_sensor.read();
    brightness_sensor.read();
    moisutre_sensor.read();

    current_millis = millis();

    view.clear();
    view.add_message(eps + "Temp = " + dht.temperature() + " %");
    view.add_message(eps + "Humi = " + dht.relative_humidity() + " %");
    view.add_message(eps + "Wate = " + water_sensor.get() + " %" + " (" + water_sensor.raw() + ")");
    view.add_message(eps + "Brig = " + brightness_sensor.get() + " %" + " (" + brightness_sensor.raw() + ")");
    view.add_message(eps + "Mois = " + moisutre_sensor.get() + " %" + " (" + moisutre_sensor.raw() + ")");

    if (current_millis - previous_millis > 2000)
    {
        previous_millis = current_millis;

        display.display(view.get_message());
        view.next();
    }

    delay(100);

    // Serial.println("=======");
    // Serial.println(eps + "Temp = " + dht.temperature());
    // Serial.println(eps + "Humi = " + dht.relative_humidity());
    // Serial.println(eps + "Wate = " + water_sensor.get() + " (" + water_sensor.raw() + ")");
    // Serial.println(eps + "Brig = " + brightness_sensor.get() + " (" + brightness_sensor.raw() + ")");
    // Serial.println(eps + "Mois = " + moisutre_sensor.get() + " (" + moisutre_sensor.raw() + ")");
}
