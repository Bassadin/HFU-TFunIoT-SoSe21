#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <Ticker.h>
#include <deque>

//Deep sleep
#include "driver/rtc_io.h"
Ticker goToDeepSleepTimer;

unsigned long espStartTime;

//Game warming up led countdown
int gameWarmingUpLEDCounter = 6;
unsigned long gameWarmupTimout = 1500;
unsigned long gameWarmupTimer = 0;

unsigned long ledBlinkTimeout = 750;
unsigned long ledBlinkTimer = 0;
bool lastLEDBLinkState = true;

//During game values
const int maxMeasurementVoltage = 2200;
const int gameEndMillivoltsThreshold = 1100;

#include "pinSetup.h"
#include "melodySetup.h"
#include "webserverSetup.h"
#include "helpers.h"
#include "gameStateManagement.h"

void setup()
{
    //Initalize serial connection
    Serial.begin(9600);
    while (!Serial)
        ;

    //Set up external wake source
    rtc_gpio_pullup_en(GPIO_NUM_4);
    rtc_gpio_pulldown_dis(GPIO_NUM_4);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 0);

    setupPins();
    loadMelodies();

    espStartTime = millis();

    changeGameState(warmingUp);
}

void loop()
{
    easyButtonButton.read();
    int currentElapsedTime = espStartTime - millis();

    //TODO case-states in einzelne methoden/dateien auslagern
    switch (currentGameState)
    {
    case gameRunning:
    {
        int millivoltsDynamoMeasurement = analogReadMilliVolts(DYNAMO_MEASUREMENT_PIN);
        int averagedMeasurement = handleMeasurementQueueForAverageValue(millivoltsDynamoMeasurement);

        double normalizedMeasurement = averagedMeasurement / (double)maxMeasurementVoltage;
        if (normalizedMeasurement > 1)
            normalizedMeasurement = 1;

        if (currentElapsedTime > 3500 && averagedMeasurement <= gameEndMillivoltsThreshold)
        {
            changeGameState(hostingWebpageForHighscore);
            setNumberOfLEDsToLightUp(0);
        }

        int ledIndex = ceil(normalizedMeasurement * ledPinsSize);
        // Serial.println(ledIndex);

        if (ledIndex == ledPinsSize)
        {
            if (millis() > ledBlinkTimeout + ledBlinkTimer)
            {
                lastLEDBLinkState = !lastLEDBLinkState;
                setNumberOfLEDsToLightUp(lastLEDBLinkState ? ledPinsSize : 0);
            }
        }
        else
        {
            setNumberOfLEDsToLightUp(ledIndex);
        }

        break;
    }
    case warmingUp:
    {
        setNumberOfLEDsToLightUp(gameWarmingUpLEDCounter);
        if (gameWarmingUpLEDCounter > 0)
        {
            if (millis() > gameWarmupTimout + gameWarmupTimer)
            {
                gameWarmupTimer = millis();

                gameWarmingUpLEDCounter--;
                setNumberOfLEDsToLightUp(gameWarmingUpLEDCounter);
                Serial.print("Counting down to game start: ");
                Serial.println(gameWarmingUpLEDCounter);
            }
        }
        else
        {
            Serial.println("Game starting...");
            changeGameState(gameRunning);
        }
        break;
    }
    case hostingWebpageForHighscore:
    {
        dnsServer.processNextRequest();
        break;
    }
    default:
        break;
    }
}