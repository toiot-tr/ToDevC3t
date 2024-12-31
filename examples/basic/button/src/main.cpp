#include <Arduino.h>

// Pin Tanımlamaları
#define LED_PIN     5   // Durum LED'i
#define BUTTON_PIN  6   // Kontrol Butonu

// Değişkenler
bool ledState = false;      // LED'in mevcut durumu
bool lastButtonState = true;  // Butonun son durumu
unsigned long lastDebounceTime = 0;  // Son debounce zamanı
unsigned long debounceDelay = 50;    // Debounce süresi (ms)

void setup() {
    // Serial başlatma
    Serial.begin(115200);
    Serial.println("ToDevC3tf Button Example");
    
    // Pin modlarını ayarla
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    // LED'i başlangıçta söndür
    digitalWrite(LED_PIN, ledState);
}

void loop() {
    // Buton durumunu oku
    bool reading = digitalRead(BUTTON_PIN);
    
    // Debounce kontrolü
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }
    
    // Debounce süresinden sonra durum değişimi
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // Buton basıldığında (LOW)
        if (reading == LOW) {
            ledState = !ledState;  // LED durumunu tersle
            digitalWrite(LED_PIN, ledState);
            Serial.print("LED: ");
            Serial.println(ledState ? "ON" : "OFF");
        }
    }
    
    lastButtonState = reading;
} 