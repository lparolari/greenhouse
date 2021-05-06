#include <Arduino.h>
#include <Wire.h>

#include "serial.hpp"
#include "display.hpp"
#include "dht.hpp"
#include "utils.hpp"

// Sensors:
// * brightness
// * water
// * moisture
// * [X] temperature
// * [X] humidity

// Attuators:
// * [X] serial
// * [X] display
// * [ ] water pump

greenhouse::display::Display<> display;
greenhouse::dht::Dht<> dht;

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

    Serial.println(eps + "Temp " + dht.temperature());
    Serial.println(eps + "Humi " + dht.relative_humidity());
    delay(1000);
}
