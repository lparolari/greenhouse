#include <Arduino.h>
#include <Wire.h>

#include "serial.hpp"
#include "display.hpp"
#include "dht.hpp"
#include "water.hpp"
#include "brightness.hpp"
#include "moisture.hpp"
#include "utils.hpp"

// Sensors:
// * [X] brightness
// * [X] water
// * [X] moisture
// * [X] temperature
// * [X] humidity

// Attuators:
// * [X] serial
// * [X] display
// * [ ] water pump

greenhouse::display::Display<> display;
greenhouse::dht::Dht<> dht;
greenhouse::moisture::MoistureSensor<1> moisutre_sensor;
greenhouse::brightness::BrightnessSensor<2> brightness_sensor;
greenhouse::water::WaterSensor<3> water_sensor;

void setup()
{
    // create

    // init
    greenhouse::serial::serial.begin(9600);
    display._lcd.begin(16, 2);
    dht._dht.begin(); // _dht.begin();

    // hello
    greenhouse::serial::hello();
    display.hello();
    dht.sensor_specs();
}

void loop()
{
    using greenhouse::utils::eps;

    water_sensor.read();
    brightness_sensor.read();
    moisutre_sensor.read();

    Serial.println("=======");
    Serial.println(eps + "Temp = " + dht.temperature());
    Serial.println(eps + "Humi = " + dht.relative_humidity());
    Serial.println(eps + "Wate = " + water_sensor.get());
    Serial.println(eps + "Brig = " + brightness_sensor.get());
    Serial.println(eps + "Mois = " + moisutre_sensor.get());

    delay(1000);
}
