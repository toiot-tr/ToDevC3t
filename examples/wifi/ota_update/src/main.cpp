#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

// Pin Tanımlamaları
#define LED_PIN     5   // Durum LED'i
#define BUTTON_PIN  6   // Kontrol Butonu

// WiFi Ayarları
const char* ssid = "ToDevC3tf";
const char* password = "12345678";

// Web Server
AsyncWebServer server(80);

// LED durumu
bool ledState = false;
unsigned long lastBlink = 0;
const long blinkInterval = 1000;  // LED yanıp sönme aralığı (ms)

void setup() {
    // Serial başlatma
    Serial.begin(115200);
    Serial.println("ToDevC3tf OTA Update Example");
    
    // Pin modlarını ayarla
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    // WiFi AP modunu başlat
    WiFi.softAP(ssid, password);
    
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    
    // Web server rotaları
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<html><body>";
        html += "<h1>ToDevC3tf OTA Update</h1>";
        html += "<p>Go to <a href='/update'>/update</a> to upload new firmware</p>";
        html += "</body></html>";
        request->send(200, "text/html", html);
    });
    
    // OTA güncellemesini başlat
    AsyncElegantOTA.begin(&server);
    server.begin();
    
    Serial.println("HTTP server started");
    Serial.println("Navigate to http://" + IP.toString() + "/update");
}

void loop() {
    // LED yanıp sönme kontrolü
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlink >= blinkInterval) {
        lastBlink = currentMillis;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }
    
    // Buton kontrolü - manuel LED kontrolü
    if (digitalRead(BUTTON_PIN) == LOW) {
        delay(50);  // Debounce
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        while(digitalRead(BUTTON_PIN) == LOW);  // Buton bırakılana kadar bekle
    }
} 