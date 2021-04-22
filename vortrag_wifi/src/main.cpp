#include "WiFi.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <DNSServer.h>
#include <ESPmDNS.h>

const char *ssid = "ESP32 Goodness";
const char *password = NULL;
const byte DNS_PORT = 53;
DNSServer dnsServer;

AsyncWebServer server(80);

// Use this IP adress after connecting to the AP
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

#define LED_PIN 19

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

void setup()
{
  // Setup soft ap
  // WiFi.hostname("esp_demo"); //This does not work as of yet
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);

  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  //DNS Server setup
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  // dnsServer.start(DNS_PORT, "www.example.com", local_ip);
  dnsServer.start(DNS_PORT, "*", local_ip);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();

  Serial.println(WiFi.localIP());

  server.serveStatic("/", SPIFFS, "/").setCacheControl("max-age=600");

  //Connection handlers
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("/index.html");
  });

  server.on("/led/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Switching LED on");
    digitalWrite(LED_PIN, HIGH);
  });

  server.on("/led/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Switching LED off");
    digitalWrite(LED_PIN, LOW);
  });

  server.begin();
}

void loop()
{
  dnsServer.processNextRequest();
}