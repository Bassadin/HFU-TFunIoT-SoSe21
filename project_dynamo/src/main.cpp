#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <Ticker.h>
#include <deque>

//Deep sleep
#include "driver/rtc_io.h"
Ticker goToDeepSleepTimer;
RTC_DATA_ATTR int bootCount = 0;

unsigned long espStartTime = 0;
unsigned long lastGameStartTime = 0;

//Game warming up led countdown
int gameWarmingUpLEDCounter = 6;
unsigned long gameWarmupTimeout = 800;
unsigned long gameWarmupTimer = 0;

unsigned long ledBlinkTimeout = 750;
unsigned long ledBlinkTimer = 0;
bool lastLEDBLinkState = true;

//During game values
const int maxMeasurementVoltage = 2200;
const int gameEndMillivoltsThreshold = 1100;
const int gracePeriodMilliseconds = 5000; //The amount of time to wait until low measurement values end the game

//After game values
int lastGameDurationMilliseconds = 0;

#include "pinSetup.h"
#include "helpers.h"
#include "melodySetup.h"
#include "webserverSetup.h"
#include "gameStateManagement.h"

void setup()
{
    //Initalize serial connection
    Serial.begin(9600);
    while (!Serial)
        ;

    //Increment boot number and print it every reboot
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    //Set up external wake source
    rtc_gpio_pullup_en(GPIO_NUM_4);
    rtc_gpio_pulldown_dis(GPIO_NUM_4);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 0);

    if (bootCount == 1)
    {
        Serial.println("First boot, going to sleep to wait for button wakeup.");
        esp_deep_sleep_start();
    }

    easyButtonButton.onPressed([]()
                               { changeGameState(countdown); });

    setupPins();
    loadMelodies();

    espStartTime = millis();

    changeGameState(countdown);
}

void loop()
{
    easyButtonButton.read();
    int elapsedTimeSinceGameStart = millis() - lastGameStartTime;

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

        if (elapsedTimeSinceGameStart > gracePeriodMilliseconds && averagedMeasurement <= gameEndMillivoltsThreshold)
        {
            lastGameDurationMilliseconds = elapsedTimeSinceGameStart;
            Serial.print("Game over! Score: ");
            Serial.println(lastGameDurationMilliseconds);
            player.playAsync(victoryMelody);
            changeGameState(hostingWebpageForHighscore);
        }

        int ledIndex = ceil(normalizedMeasurement * ledPinsSize);

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
    case countdown:
    {
        setNumberOfLEDsToLightUp(gameWarmingUpLEDCounter);
        if (gameWarmingUpLEDCounter > 0)
        {
            if (millis() > gameWarmupTimeout + gameWarmupTimer)
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