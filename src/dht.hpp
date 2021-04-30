#pragma once

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

namespace dht
{
    DHT_Unified dht(2, DHT11);

    typedef sensor_t sensor_t;

    void print_dht_sensor_specs(sensor_t &sensor)
    {
        Serial.print(F("Sensor Type: "));
        Serial.println(sensor.name);
        Serial.print(F("Driver Ver:  "));
        Serial.println(sensor.version);
        Serial.print(F("Unique ID:   "));
        Serial.println(sensor.sensor_id);
        Serial.print(F("Max Value:   "));
        Serial.print(sensor.max_value);
        Serial.println(F("°C"));
        Serial.print(F("Min Value:   "));
        Serial.print(sensor.min_value);
        Serial.println(F("°C"));
        Serial.print(F("Resolution:  "));
        Serial.print(sensor.resolution);
        Serial.println(F("°C"));
    }
} // namespace dht
