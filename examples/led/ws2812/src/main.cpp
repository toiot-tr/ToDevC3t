#include <Arduino.h>
#include <FastLED.h>

// Pin Tanımlamaları
#define LED_PIN     5   // Durum LED'i
#define BUTTON_PIN  6   // Kontrol Butonu
#define DATA_PIN    10  // WS2812 Data Pini

// WS2812 LED Ayarları
#define NUM_LEDS    8   // LED sayısı
#define BRIGHTNESS  64  // Parlaklık (0-255)
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

// LED dizisi
CRGB leds[NUM_LEDS];

// Değişkenler
uint8_t mode = 0;      // Animasyon modu
uint8_t gHue = 0;      // Döngüsel renk değişimi için
unsigned long lastUpdate = 0;
const long updateInterval = 20;  // 20ms (50Hz)

void setup() {
    // Serial başlatma
    Serial.begin(115200);
    Serial.println("ToDevC3tf WS2812 LED Example");
    
    // Pin modlarını ayarla
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    // FastLED ayarları
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip)
        .setDither(BRIGHTNESS < 255);
    FastLED.setBrightness(BRIGHTNESS);
    
    // Başlangıçta tüm LED'leri söndür
    FastLED.clear();
    FastLED.show();
}

// Animasyon fonksiyonları
void rainbow() {
    fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void sinelon() {
    fadeToBlackBy(leds, NUM_LEDS, 20);
    int pos = beatsin16(13, 0, NUM_LEDS-1);
    leds[pos] += CHSV(gHue, 255, 192);
}

void bpm() {
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
}

void juggle() {
    fadeToBlackBy(leds, NUM_LEDS, 20);
    uint8_t dothue = 0;
    for(int i = 0; i < 8; i++) {
        leds[beatsin16(i+7, 0, NUM_LEDS-1)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}

void loop() {
    // Buton kontrolü - mod değiştirme
    if (digitalRead(BUTTON_PIN) == LOW) {
        delay(50);  // Debounce
        mode = (mode + 1) % 4;  // 4 mod arasında geçiş
        Serial.print("Mode changed to: ");
        Serial.println(mode);
        while(digitalRead(BUTTON_PIN) == LOW);  // Buton bırakılana kadar bekle
    }
    
    // Animasyon güncelleme
    EVERY_N_MILLISECONDS(20) { gHue++; }  // Renk döngüsü
    
    // Mod kontrolü
    switch(mode) {
        case 0:
            rainbow();
            break;
        case 1:
            sinelon();
            break;
        case 2:
            bpm();
            break;
        case 3:
            juggle();
            break;
    }
    
    // LED'leri güncelle
    FastLED.show();
    
    // Durum LED'ini yakıp söndür
    EVERY_N_MILLISECONDS(1000) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
} 