#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <Ticker.h>
#include <deque>

//Wifi settings
#include "WiFi.h"
const char *ssid = "teste-deine-kraft";
const char *password = NULL;

//DNS
#include <DNSServer.h>
#include <ESPmDNS.h>
const byte DNS_PORT = 53;
DNSServer dnsServer;
AsyncWebServer server(80);

// Use this IP adress after connecting to the AP
IPAddress accessPointIP(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

#pragma region Pin Settings
//LEDs

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

#pragma endregion Pin Settings

//Melody player settings
#include <melody_player.h>
#include <melody_factory.h>
MelodyPlayer player(BUZZER_PIN, 0, LOW);

String notes[] = {"C3", "F3", "A3", "C4", "A3", "C4", "C4", "C4"};
const char melodyString[] = "ff6_victory:d=4,o=5,b=140:32d6,32p,32d6,32p,32d6,32p,d6,a#,c6,16d6,8p,16c6,2d6";

//Deep sleep timer
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

std::deque<int> measurementDeque;
const int maxDequeSize = 15;

//State management
enum GameState
{
    gameRunning = 0,
    hostingWebpageForHighscore = 1,
    warmingUp = 2
};
GameState currentGameState = warmingUp;

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/captivePortalRedirect.html", "text/html");
    }
};

void setNumberOfLEDsToLightUp(unsigned int ledNumber)
{
    for (int i = 0; i < ledPinsSize; i++)
    {
        digitalWrite(led_pins[i], i < ledNumber ? HIGH : LOW);
    }
}

void goToDeepSleep()
{
    esp_deep_sleep_start();
}

void setupWiFiAndWebServer()
{
    //Set up wifi
    Serial.println("Setting up WiFi AP");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(accessPointIP, accessPointIP, subnet);

    //Initialize SPIFFS
    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    //DNS Server setup
    Serial.println("Starting DNS server");
    dnsServer.setTTL(300);
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", accessPointIP);
    Serial.println("DNS server started");

    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)…");
    server
        .serveStatic("/", SPIFFS, "/")
        .setDefaultFile("index.html");

    //Captive portal connection handlers
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->redirect("/index.html"); });

    Serial.println("Starting server...");
    server.begin();
    Serial.println("Setup done.");
}

void handleEndGame()
{
    setupWiFiAndWebServer();
    goToDeepSleepTimer.once(120, goToDeepSleep); //Go to sleep after 120 seconds/2 minutes
}

int handleMeasurementQueueForAverageValue(int newMeasurementValue)
{
    int millivoltsDynamoMeasurement = analogReadMilliVolts(DYNAMO_MEASUREMENT_PIN);
    measurementDeque.push_front(millivoltsDynamoMeasurement);

    if (measurementDeque.size() < 5)
    {
        return newMeasurementValue;
    }

    if (measurementDeque.size() > maxDequeSize)
    {
        measurementDeque.pop_back();
    }

    int dequeSum = 0;
    for (int i : measurementDeque)
    {
        dequeSum += i;
    }

    return dequeSum / measurementDeque.size();
}

void changeGameState(GameState newGameState)
{
    Serial.print("Switching to new game State: ");
    Serial.println(newGameState);
    currentGameState = newGameState;
    //TODO place actions on game state change to happen once herei
}

void setupPins() {
    //Initialize LED pins
    Serial.println("Initializing LED pins");
    for (int currentPin : led_pins)
    {
        pinMode(currentPin, OUTPUT);
    }

    //Initialize other pins
    Serial.println("Initializing other pins");
    easyButtonButton.begin();
    // easyButtonButton.onPressed(onButtonPressed);

    pinMode(DYNAMO_MEASUREMENT_PIN, INPUT);
}

void setup()
{
    //Initalize serial connection
    Serial.begin(9600);
    while (!Serial)
        ;

    // Load and play a correct melody
    Serial.println("Playing test melody...");
    Melody melody = MelodyFactory.load("Victory tune", 140, notes, 8);

    Melody melody2 = MelodyFactory.loadRtttlString(melodyString);

    player.playAsync(melody2);

    //Set up external wake source
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);

    setupPins();

    espStartTime = millis();
}

void loop()
{
    easyButtonButton.read();

    int currentElapsedTime = espStartTime - millis();

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
            handleEndGame();
        }

        int ledIndex = ceil(normalizedMeasurement * ledPinsSize);
        // Serial.println(ledIndex);

        if (ledIndex == ledPinsSize)
        {
            if (millis() > ledBlinkTimeout + ledBlinkTimer)
            {
                lastLEDBLinkState = !lastLEDBLinkState;
                setNumberOfLEDsToLightUp(lastLEDBLinkState ? ledPinsSize : 0);
                // Serial.println("LED Blink trigger");
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
    }
}