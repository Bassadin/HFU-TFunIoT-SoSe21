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

const int START_LED_PIN = 13;
const int END_LED_PIN = 16;

auto startJled = JLed(START_LED_PIN);
auto endJled = JLed(END_LED_PIN);

#include <EasyButton.h>
EasyButton easyButtonButton(BUTTON_PIN);

void setNumberOfPowerMeterLEDsToLightUp(unsigned int ledNumber)
{
    // Serial.print("Power Meter LEDs state: ");
    for (int i = 0; i < ledPinsSize; i++)
    {
        digitalWrite(led_pins[i], i < ledNumber ? HIGH : LOW);
        // Serial.print(i < ledNumber ? "1" : "0");
        // Serial.print("-");
    }
    // Serial.println(".");
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

    // pinMode(START_LED_PIN, OUTPUT);
    // pinMode(END_LED_PIN, OUTPUT);

    pinMode(DYNAMO_MEASUREMENT_PIN, INPUT);
}

void updateJleds()
{
    startJled.Update();
    endJled.Update();
}