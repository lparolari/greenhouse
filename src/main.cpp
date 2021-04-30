#include <Arduino.h>
#include <Wire.h>

#include <LiquidCrystal.h>

#include "setup.h"
#include "dht.hpp"
#include "display.hpp"
#include "serial.hpp"

const uint16_t MESSAGE_TEMP = 0;
const uint16_t MESSAGE_LIGHT = 1;

int light_max = 250;
int light_min = 0;

struct Config
{
  uint32_t dht_delay_ms = 1000;
  uint16_t message_toggle = 0U;
};

Config config;

unsigned long previous_millis = 0;
unsigned long previous_millis_2 = 0;

void display_temperature_and_humidity();

void setup()
{
  serial::serial.begin(9600);
  display::lcd.begin(16, 2);
  dht::dht.begin();

  display::hello();

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
  const unsigned long current_millis = millis();

  if (current_millis - previous_millis > 3000)
  {
    previous_millis = current_millis;

    if (config.message_toggle == MESSAGE_TEMP)
    {
      display_temperature_and_humidity();
    }
    else if (config.message_toggle == MESSAGE_LIGHT)
    {
      const int light_value = analogRead(0);

      display::lcd.clear();
      display::lcd.setCursor(0, 0);
      display::lcd.print("Light = ");
      display::lcd.print((float)light_value / (light_max - light_min) * 100);
      display::lcd.print(" %");
    }
    else
    {
      display::lcd.clear();
      display::lcd.setCursor(0, 0);
      display::lcd.print(":)");
    }

    config.message_toggle = (config.message_toggle + 1) % 3;
  }

  delay(1);
}

void display_temperature_and_humidity()
{
  sensors_event_t event;
  dht::dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    display::lcd.clear();
    display::lcd.setCursor(0, 0);
    display::lcd.print(F("Temp = error"));
  }
  else
  {
    display::lcd.clear();
    display::lcd.setCursor(0, 0);
    display::lcd.print(F("Temp = "));
    display::lcd.print(event.temperature);
    display::lcd.print(F(" C"));
  }
  // Get humidity event and print its value.
  dht::dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    display::lcd.setCursor(0, 1);
    display::lcd.print(F("Hygro = error"));
  }
  else
  {
    display::lcd.setCursor(0, 1);
    display::lcd.print(F("Hygro = "));
    display::lcd.print(event.relative_humidity);
    display::lcd.print(F(" %"));
  }
}
