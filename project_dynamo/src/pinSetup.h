//red
const int LED_PIN_R1 = 33;
const int LED_PIN_R2 = 25;
//yellow
const int LED_PIN_Y1 = 26;
const int LED_PIN_Y2 = 27;
//green
const int LED_PIN_G1 = 14;
const int LED_PIN_G2 = 12;

//pin array
const int ledPinsSize = 6;
const int led_pins[ledPinsSize] = {LED_PIN_R1, LED_PIN_R2, LED_PIN_Y1, LED_PIN_Y2, LED_PIN_G1, LED_PIN_G2};

int currentLedCounter = 0;

//Other pins
const int BUTTON_PIN = 4;
const int BUZZER_PIN = 17;
const int DYNAMO_MEASUREMENT_PIN = 35;

#include <EasyButton.h>
EasyButton easyButtonButton(BUTTON_PIN);

void setNumberOfLEDsToLightUp(unsigned int ledNumber)
{
    for (int i = 0; i < ledPinsSize; i++)
    {
        digitalWrite(led_pins[i], i < ledNumber ? HIGH : LOW);
    }
}

void setupPins()
{
    //Initialize LED pins
    Serial.println("Initializing LED pins");
    for (int currentPin : led_pins)
    {
        pinMode(currentPin, OUTPUT);
    }

    //Initialize other pins
    Serial.println("Initializing other pins");
    easyButtonButton.begin();

    pinMode(DYNAMO_MEASUREMENT_PIN, INPUT);
}