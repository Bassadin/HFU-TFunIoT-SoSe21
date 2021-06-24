//Wifi settings
#include "WiFi.h"
const char *ssid = "teste-deine-kraft";
const char *password = NULL;

//DNS
#include <DNSServer.h>
const byte DNS_PORT = 53;
DNSServer dnsServer;
AsyncWebServer server(80);

// Use this IP adress after connecting to the AP
IPAddress accessPointIP(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

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
        request->redirect("/index.html");
    }
};

void setupWiFiAndWebServer()
{
    //Set up wifi
    Serial.println("Setting up WiFi AP");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(accessPointIP, accessPointIP, subnet);
    WiFi.softAP(ssid, password);

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
    // server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->redirect("/index.html"); });

    server.on("/lastGameScore", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                request->send(200, "text/plain", String(lastGameDurationMilliseconds));
                goToDeepSleepTimer.once(30, goToDeepSleep); //Faster fall asleep time when request was made
                });

    //Capture requests menat for google servers that check for internet connection
    server.on("/generate_204", HTTP_ANY, [](AsyncWebServerRequest *request)
              { request->send(204); });

    server.on("/gen_204", HTTP_ANY, [](AsyncWebServerRequest *request)
              { request->send(204); });

    server.on("/chrome-variations/seed", HTTP_ANY, [](AsyncWebServerRequest *request)
              { request->send(204); });

    // server.onNotFound([](AsyncWebServerRequest *request)
    //                   {
    //                       Serial.print(request->host());
    //                       Serial.print(" - ");
    //                       Serial.println(request->url());
    //                       request->send(204);
    //                   });

    Serial.println("Starting server...");
    server.begin();
    Serial.println("Webserver setup complete.");
}