#include <Arduino.h>

// Pin Tanımlamaları
#define LED_PIN     5   // Durum LED'i
#define BUTTON_PIN  6   // Kontrol Butonu
#define RGB_R_PIN   7   // RGB Kırmızı
#define RGB_G_PIN   8   // RGB Yeşil
#define RGB_B_PIN   9   // RGB Mavi

// RGB LED değerleri
uint8_t r = 0, g = 0, b = 0;
uint8_t mode = 0;  // 0: Manuel, 1: Rainbow, 2: Breathing

// Zamanlama değişkenleri
unsigned long lastUpdate = 0;
const long updateInterval = 20;  // 20ms (50Hz)
float hue = 0.0;  // Rainbow efekti için

// HSV'den RGB'ye dönüşüm
void HSVtoRGB(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b) {
    float c = v * s;
    float x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
    float m = v - c;
    
    float r_temp, g_temp, b_temp;
    
    if(h >= 0 && h < 60) {
        r_temp = c; g_temp = x; b_temp = 0;
    } else if(h >= 60 && h < 120) {
        r_temp = x; g_temp = c; b_temp = 0;
    } else if(h >= 120 && h < 180) {
        r_temp = 0; g_temp = c; b_temp = x;
    } else if(h >= 180 && h < 240) {
        r_temp = 0; g_temp = x; b_temp = c;
    } else if(h >= 240 && h < 300) {
        r_temp = x; g_temp = 0; b_temp = c;
    } else {
        r_temp = c; g_temp = 0; b_temp = x;
    }
    
    *r = (r_temp + m) * 255;
    *g = (g_temp + m) * 255;
    *b = (b_temp + m) * 255;
}

void setup() {
    // Serial başlatma
    Serial.begin(115200);
    Serial.println("ToDevC3tf RGB LED Control Example");
    
    // Pin modlarını ayarla
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(RGB_R_PIN, OUTPUT);
    pinMode(RGB_G_PIN, OUTPUT);
    pinMode(RGB_B_PIN, OUTPUT);
    
    // RGB LED'leri başlangıçta söndür
    analogWrite(RGB_R_PIN, 0);
    analogWrite(RGB_G_PIN, 0);
    analogWrite(RGB_B_PIN, 0);
}

void loop() {
    // Buton kontrolü - mod değiştirme
    if (digitalRead(BUTTON_PIN) == LOW) {
        delay(50);  // Debounce
        mode = (mode + 1) % 3;  // 3 mod arasında geçiş
        Serial.print("Mode changed to: ");
        Serial.println(mode);
        while(digitalRead(BUTTON_PIN) == LOW);  // Buton bırakılana kadar bekle
    }
    
    // Mod kontrolü
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdate >= updateInterval) {
        lastUpdate = currentMillis;
        
        switch(mode) {
            case 0:  // Manuel mod - Sabit beyaz
                r = 255;
                g = 255;
                b = 255;
                break;
                
            case 1:  // Rainbow modu
                hue = fmod(hue + 1, 360);  // 0-360 derece arası
                HSVtoRGB(hue, 1.0, 1.0, &r, &g, &b);
                break;
                
            case 2:  // Breathing modu
                float breath = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
                r = breath;
                g = breath;
                b = breath;
                break;
        }
        
        // RGB değerlerini uygula
        analogWrite(RGB_R_PIN, r);
        analogWrite(RGB_G_PIN, g);
        analogWrite(RGB_B_PIN, b);
        
        // Durum LED'ini yakıp söndür
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
    
    // Serial'dan komut kontrolü
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        if (cmd.startsWith("RGB")) {
            // Format: RGB,r,g,b (0-255)
            r = cmd.substring(4,7).toInt();
            g = cmd.substring(8,11).toInt();
            b = cmd.substring(12).toInt();
            mode = 0;  // Manuel moda geç
        }
    }
} 