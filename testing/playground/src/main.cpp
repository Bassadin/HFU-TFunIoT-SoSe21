
#include <Arduino.h>

int BUTTON_PIN = 16;
int BUZZER_PIN = 17;

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
    Serial.println(digitalRead(BUTTON_PIN));
    if (digitalRead(BUTTON_PIN) == LOW) { //If button pressed
        digitalWrite(BUZZER_PIN, HIGH);
    } else {
        digitalWrite(BUZZER_PIN, LOW);
    }
}