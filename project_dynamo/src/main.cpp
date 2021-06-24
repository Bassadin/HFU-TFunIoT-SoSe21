#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <Ticker.h>
#include <deque>
#include <jled.h>

//Deep sleep
#include "driver/rtc_io.h"
Ticker goToDeepSleepTimer;
RTC_DATA_ATTR int bootCount = 0;

unsigned long espStartTime = 0;
unsigned long lastGameStartTime = 0;

//Pre game info timer
unsigned long preGameInfoTimeout = 8000;
unsigned long preGameInfoTimer = 0;

//Game warming up led countdown
int gameLEDCountdownCounter = 6;
unsigned long gameCountdownTimeout = 700;
unsigned long gameCountdownTimer = 0;

unsigned long ledBlinkTimeout = 200;
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

    setupPins();
    loadMelodies();

    espStartTime = millis();

    //Initial game state after wakeup
    changeGameState(preGameInfo);

    easyButtonButton.onPressed([]()
                               { changeGameState(preGameInfo); });
}

void loop()
{
    easyButtonButton.read();
    updateJleds();

    //TODO case-states in einzelne methoden/dateien auslagern
    switch (currentGameState)
    {
    case gameRunning:
    {
        int millivoltsDynamoMeasurement = analogReadMilliVolts(DYNAMO_MEASUREMENT_PIN);
        int averagedMeasurement = handleMeasurementQueueForAverageValue(millivoltsDynamoMeasurement);
        int elapsedTimeSinceGameStart = millis() - lastGameStartTime;

        double normalizedMeasurement = averagedMeasurement / (double)maxMeasurementVoltage;
        if (normalizedMeasurement > 1)
            normalizedMeasurement = 1;

        // int ledIndex = ceil(normalizedMeasurement * ledPinsSize);

        // if (ledIndex == ledPinsSize)
        // {
        //     if (millis() > ledBlinkTimeout + ledBlinkTimer)
        //     {
        //         ledBlinkTimer = millis();
        //         lastLEDBLinkState = !lastLEDBLinkState;
        //         setNumberOfPowerMeterLEDsToLightUpStatic(lastLEDBLinkState ? ledPinsSize : 0);
        //     }
        // }
        // else
        // {
        //     setNumberOfPowerMeterLEDsToLightUpStatic(ledIndex);
        // }

        // setNumberOfPowerMeterLEDsToLightUpStatic(std::floor(elapsedTimeSinceGameStart / 1000 / 10));

        setNumberOfPowerMeterLEDsToLightUpDynamic((float)elapsedTimeSinceGameStart / 1000 / 10);

        if ((elapsedTimeSinceGameStart > gracePeriodMilliseconds && averagedMeasurement <= gameEndMillivoltsThreshold) || elapsedTimeSinceGameStart > 60000)
        {
            lastGameDurationMilliseconds = elapsedTimeSinceGameStart;
            Serial.print("Game over! Score: ");
            Serial.println(lastGameDurationMilliseconds);

            if (lastGameDurationMilliseconds >= 0 && lastGameDurationMilliseconds < 30000)
            {
                player.playAsync(defeatMelody);
            }
            else if (lastGameDurationMilliseconds >= 30000 && lastGameDurationMilliseconds < 55000)
            {
                player.playAsync(silverMelody);
            }
            else if (lastGameDurationMilliseconds >= 55000)
            {
                player.playAsync(victoryMelody);
            }

            changeGameState(hostingWebpageForHighscore);
        }

        break;
    }
    case countdown:
    {
        setNumberOfPowerMeterLEDsToLightUpStatic(gameLEDCountdownCounter);
        if (gameLEDCountdownCounter > 0)
        {
            if (millis() > gameCountdownTimeout + gameCountdownTimer)
            {
                gameCountdownTimer = millis();

                gameLEDCountdownCounter--;
                setNumberOfPowerMeterLEDsToLightUpStatic(gameLEDCountdownCounter);
                Serial.print("Counting down to game start: ");
                Serial.println(gameLEDCountdownCounter);
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
    case preGameInfo:
    {
        if (millis() > preGameInfoTimeout + preGameInfoTimer)
        {
            preGameInfoTimer = millis();
            changeGameState(countdown);
        }
        break;
    }
    default:
        break;
    }
}