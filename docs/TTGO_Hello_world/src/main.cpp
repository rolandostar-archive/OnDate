// #include <Arduino.h>

// #include <TFT_eSPI.h>
// #include <SPI.h>
// #include "WiFi.h"
// #include <Wire.h>
// #include <Button2.h>
// #include "esp_adc_cal.h"

// #ifndef TFT_DISPOFF
// #define TFT_DISPOFF 0x28
// #endif

// #ifndef TFT_SLPIN
// #define TFT_SLPIN   0x10
// #endif

// #define ADC_EN          14
// #define ADC_PIN         34
// #define BUTTON_1        35
// #define BUTTON_2        0

// TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

// //! Long time delay, it is recommended to use shallow sleep, which can effectively reduce the current consumption
// void espDelay(int ms) //use-> espDelay(6000);
// {
//     esp_sleep_enable_timer_wakeup(ms * 1000);
//     esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,ESP_PD_OPTION_ON);
//     esp_light_sleep_start();
// }

// void setup()
// {
//     Serial.begin(115200);
//     Serial.println("Start");
//     tft.init();
//     tft.fontHeight(2);
//     tft.setRotation(1);
//     tft.fillScreen(TFT_BLACK);
//     tft.drawString("Hello world", tft.width()/4, tft.height() / 2, 4);  //string,start x,start y, font weight {1;2;4;6;7;8}
// }

// void loop()
// {

// }
#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"

DNSServer dnsServer;
AsyncWebServer server(80);

// class CaptiveRequestHandler : public AsyncWebHandler
// {
// public:
//     CaptiveRequestHandler() {}
//     virtual ~CaptiveRequestHandler() {}

//     bool canHandle(AsyncWebServerRequest *request)
//     {
//         //request->addInterestingHeader("ANY");
//         return true;
//     }

//     void handleRequest(AsyncWebServerRequest *request)
//     {
//         AsyncResponseStream *response = request->beginResponseStream("text/html");
//         response->print("<!DOCTYPE html><html><head><title>Captive Portal</title></head><body>");
//         response->print("<p>This is out captive portal front page.</p>");
//         response->printf("<p>You were trying to reach: http://%s%s</p>", request->host().c_str(), request->url().c_str());
//         response->printf("<p>Try opening <a href='http://%s'>this link</a> instead</p>", WiFi.softAPIP().toString().c_str());
//         response->print("</body></html>");
//         request->send(response);
//     }
// };

void setup()
{
    Serial.begin(115200);
    WiFi.softAP("OnDate Config");
    dnsServer.start(53, "*", WiFi.softAPIP());
    // server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); //only when requested from AP
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Hello, world");
    });

    // Send a GET request to <IP>/sensor/<number>
    server.on("^\\/sensor\\/([0-9]+)$", HTTP_GET, [](AsyncWebServerRequest *request) {
        String sensorNumber = request->pathArg(0);
        request->send(200, "text/plain", "Hello, sensor: " + sensorNumber);
    });

    // Send a GET request to <IP>/sensor/<number>/action/<action>
    server.on("^\\/sensor\\/([0-9]+)\\/action\\/([a-zA-Z0-9]+)$", HTTP_GET, [](AsyncWebServerRequest *request) {
        String sensorNumber = request->pathArg(0);
        String action = request->pathArg(1);
        request->send(200, "text/plain", "Hello, sensor: " + sensorNumber + ", with action: " + action);
    });

    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "Not found");
    });
    server.begin();
}

void loop()
{
    dnsServer.processNextRequest();
}