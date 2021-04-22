#include "WiFi.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

const char *ssid = "ESP32 Goodness";
const char *password = NULL;

AsyncWebServer server(80);

// Use this IP adress after connecting to the AP
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

#define LED_PIN 19

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);

  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");

  // Setup soft ap
  // WiFi.hostname("esp_demo"); //This does not work as of yet
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/styles.css", "text/css");
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

void loop() {}