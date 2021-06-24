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
const int ledPins[ledPinsSize] = {LED_PIN_R1, LED_PIN_R2, LED_PIN_Y1, LED_PIN_Y2, LED_PIN_G1, LED_PIN_G2};

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

void setNumberOfPowerMeterLEDsToLightUpStatic(unsigned int ledNumber)
{
    int correctedLedNumber = ledNumber;
    if (ledNumber > ledPinsSize)
    {
        correctedLedNumber = ledPinsSize;
    }

    // Serial.print("Power Meter LEDs state: ");
    for (int i = 0; i < ledPinsSize; i++)
    {
        // digitalWrite(ledPins[i], i < correctedLedNumber ? HIGH : LOW);

        ledcWrite(i + 5, i < correctedLedNumber ? 255 : 0);

        // Serial.print(i < ledNumber ? "1" : "0");
        // Serial.print("-");
    }
    // Serial.println(".");
}

void setNumberOfPowerMeterLEDsToLightUpDynamic(float progressNumber)
{
    float correctedProgressNumber = progressNumber;
    if (progressNumber > ledPinsSize)
    {
        correctedProgressNumber = ledPinsSize;
    }
    else if (correctedProgressNumber < 0)
    {
        correctedProgressNumber = 0;
    }

    // Serial.printf("setNumberOfPowerMeterLEDsToLightUpDynamic correctedProgressNumber value: %f\n", correctedProgressNumber);

    for (int i = 0; i < ledPinsSize; i++)
    {
        int dutyCycleNumber;
        if (i + 1 < correctedProgressNumber)
        {
            dutyCycleNumber = 255;
        }
        else
        {
            dutyCycleNumber = (correctedProgressNumber - i) * 255;
            if (dutyCycleNumber < 0)
                dutyCycleNumber = 0;
        }
        ledcWrite(i + 5, dutyCycleNumber);
    }
}

void setupPins()
{
    //Initialize LED pins
    Serial.println("Initializing LED pins");
    for (int i = 0; i < ledPinsSize; i++)
    {
        int currentLedChannel = 5 + i;
        ledcSetup(currentLedChannel, 5000, 8);
        ledcAttachPin(ledPins[i], currentLedChannel);
        Serial.printf("Attaching pin %i to PWM channel %i\n", ledPins[i], currentLedChannel);
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