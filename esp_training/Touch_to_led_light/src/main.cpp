#include <Arduino.h>

int LED = 32;
int pin_touch = 4;

void setup()
{
  pinMode(LED, OUTPUT);
}

void loop()
{
  bool touching = touchRead(pin_touch) < 50;
  digitalWrite(LED, touching);
}