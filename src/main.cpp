#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#include "dht.hpp"
#include "display.hpp"
#include "serial.hpp"
#include "moisture.hpp"
#include "utils.hpp"
#include "brightness.hpp"

const uint16_t MESSAGE_TEMP = 0;
const uint16_t MESSAGE_LIGHT = 1;
const uint16_t MESSAGE_MOISUTRE = 2;

int light_max = 250;
int light_min = 0;

struct Config
{
  uint32_t dht_delay_ms = 1000;
  uint16_t message_toggle = MESSAGE_LIGHT;
  bool toggle = false;
};

Config config;

unsigned long previous_millis = 0;
unsigned long previous_millis_2 = 0;

void display_temperature_and_humidity();

greenhouse::display::Display<> display;
greenhouse::brightness::BrightnessSensor<> brightness_sensor;
greenhouse::moisture::MoistureSensor<> moisture_sensor;

void setup()
{
  serial::serial.begin(9600);
  greenhouse::display::lcd.begin(16, 2);
  dht::dht.begin();

  display.hello();

  dht::sensor_t temperature_sensor;
  dht::sensor_t humidity_sensor;

  dht::dht.temperature().getSensor(&temperature_sensor);
  dht::dht.humidity().getSensor(&humidity_sensor);

  dht::print_dht_sensor_specs(temperature_sensor);
  dht::print_dht_sensor_specs(humidity_sensor);

  config.dht_delay_ms = max(temperature_sensor.min_delay, humidity_sensor.min_delay);
}

void loop()
{
  using greenhouse::utils::eps;

  const unsigned long current_millis = millis();

  if (current_millis - previous_millis > 500)
  {
    previous_millis = current_millis;

    if (config.message_toggle == MESSAGE_TEMP)
    {
      display_temperature_and_humidity();
    }
    else if (config.message_toggle == MESSAGE_LIGHT)
    {
      brightness_sensor.read();
      display.display(eps + "Light = " + brightness_sensor.get() + " %");
    }
    else if (config.message_toggle == MESSAGE_MOISUTRE)
    {
      moisture_sensor.read();
      display.display(eps + "Light = " + moisture_sensor.get() + " %");
    }
    else
    {
      display.display(":)");
    }

    if (config.toggle)
      config.message_toggle = (config.message_toggle + 1) % 4;
  }

  delay(1);
}

void display_temperature_and_humidity()
{
  sensors_event_t event;
  dht::dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    // display::lcd.clear();
    // display::lcd.setCursor(0, 0);
    // display::lcd.print(F("Temp = error"));
  }
  else
  {
    // display::lcd.clear();
    // display::lcd.setCursor(0, 0);
    // display::lcd.print(F("Temp = "));
    // display::lcd.print(event.temperature);
    // display::lcd.print(F(" C"));
  }
  // Get humidity event and print its value.
  dht::dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    // display::lcd.setCursor(0, 1);
    // display::lcd.print(F("Hygro = error"));
  }
  else
  {
    // display::lcd.setCursor(0, 1);
    // display::lcd.print(F("Hygro = "));
    // display::lcd.print(event.relative_humidity);
    // display::lcd.print(F(" %"));
  }
}
