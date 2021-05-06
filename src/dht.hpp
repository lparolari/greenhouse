#pragma once

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

namespace greenhouse::dht
{
    template <uint8_t Pin = 2U, uint8_t Type = DHT11>
    class Dht
    {
    public:
        DHT_Unified _dht;

        Dht() : _dht(Pin, Type) {}

        float temperature()
        {
            sensors_event_t event;
            _dht.temperature().getEvent(&event);

            return event.temperature;
        }

        float relative_humidity()
        {
            sensors_event_t event;
            _dht.humidity().getEvent(&event);

            return event.relative_humidity;
        }

        void sensor_specs()
        {
            sensor_t sensor;
            _dht.temperature().getSensor(&sensor);

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
    };
} // namespace dht
