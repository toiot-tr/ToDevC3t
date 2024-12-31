#include <Arduino.h>

// Pin Tanımlamaları
#define LED_PIN     5   // Durum LED'i
#define BUTTON_PIN  6   // Kontrol Butonu

void setup() {
    // Serial başlatma
    Serial.begin(115200);
    Serial.println("ToDevC3tf Blink Example");
    
    // LED pinini çıkış olarak ayarla
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // LED'i yak
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON");
    delay(1000);
    
    // LED'i söndür
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF");
    delay(1000);
} 