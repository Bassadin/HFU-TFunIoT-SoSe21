#include "WiFi.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <DNSServer.h>
#include <ESPmDNS.h>

const char *ssid = "teste-deine-kraft";
const char *password = NULL;

//DNS
const byte DNS_PORT = 53;
DNSServer dnsServer;

AsyncWebServer server(80);

// Use this IP adress after connecting to the AP
IPAddress accessPointIP(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

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
const int led_pins[6] = {LED_PIN_R1, LED_PIN_R2, LED_PIN_Y1, LED_PIN_Y2, LED_PIN_G1, LED_PIN_G2};

//Other pins

const int BUTTON_PIN = 16;
const int BUZZER_PIN = 17;

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

void setup()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(accessPointIP, accessPointIP, subnet);

    //Initialize LED pins
    for (int currentPin : led_pins)
    {
        pinMode(currentPin, OUTPUT);
    }

    //Initialize other pins
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);

    //Test led method
    setNumberOfLEDsToLightUp(5);

    //Initalize serial connection
    Serial.begin(115200);

    //Initialize SPIFFS
    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    //DNS Server setup
    dnsServer.setTTL(300);
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", accessPointIP);
    Serial.println("DNS server started");

    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)…");
    server
        .serveStatic("/", SPIFFS, "/")
        .setDefaultFile("index.html");

    //Connection handlers
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/index.html");
    });

    Serial.println("Starting server...");
    server.begin();
    Serial.println("Setup done.");
}

void loop()
{
    dnsServer.processNextRequest();
}