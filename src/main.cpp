#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.write("Hi\n");
  delay(1000);
}
