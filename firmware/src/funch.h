#include "header.h"
#ifndef FUNCH_GLOBAL_H
#define FUNCH_GLOBAL_H

// extern void serialWrite(String mesajj1, String mesajj2, String mesajj3, String mesajj4, String mesajj5, String mesajj6, String mesajj7, String mesajj8, String mesajj9);
extern bool logAl(String mesajj1, String mesajj2 = "");
extern void espRestart();
extern void restoreDefaults();
extern void ayarlarioku();
extern bool pinAyarla();
// extern void acilisLed();
extern void portalButonOku(int8_t pin = BtnPin);
// extern uint8_t karakterSayisi(String mesaj);
// extern float mapFloat(float value, float fromLow, float fromHigh, float toLow, float toHigh);
// extern esp_reset_reason_t resetNedeni();

#elif !defined(FUNCH_LOCAL_H)
#define FUNCH_LOCAL_H
#pragma once

bool logAl(const String mesajj1, const String mesajj2) 
{
    utils.serialWrite(" ");
    utils.serialWrite("  >>>>>>>>>>>logAl>>>>>>>>>>>> ");
    String logStr = "";
    struct tm timeinfoo;
    time_t now = time(nullptr);
    localtime_r(&now, &timeinfoo);
    char saat[3];
    char dakika[3];
    sprintf(saat, "%02d", timeinfoo.tm_hour);
    sprintf(dakika, "%02d", timeinfoo.tm_min);
    logStr.concat(saat);
    logStr.concat(":");
    logStr.concat(dakika);
    logStr += " ";
    logStr += mesajj1;
    logStr += mesajj2;
    utils.serialWrite("  :", logStr);
    File file = LittleFS.open("/log.txt", "a"); // dosyayı aç
    if (file)
    {
        if (file.size() > 1500) // Dosya boyutunu kontrol et
        {
            utils.serialWrite("  ****file.size() > 1500****  ");
            vTaskDelay(500);
            file.close();                          // dosya kapat
            file = LittleFS.open("/log.txt", "r"); // dosyayı oku
            if (!file)
            {
                utils.serialWrite("!!!!!file.size() > 500 acılamadı1!!!!");
                utils.serialWrite("  <<<<<<<<<<<<<logAl<<<<<<<<<<<<<< ");
                utils.serialWrite(" ");
                return false;
            }
            for (uint8_t i = 0; i < 20; i++)
            {
                file.readStringUntil('\n'); // satırı atla
            }
            String geciciLogStr = file.readString(); // kalanı oku
            // serialWrite("geciciLogStr ", geciciLogStr);
            file.close();                          // dosya kapat
            file = LittleFS.open("/log.txt", "w"); // dosyayı tekrar aç
            if (file)
            {
                file.print(geciciLogStr); // geri kalanı dosyaya yazdır
                file.close();             // dosya kapat
                return true;              // işlemler başarılıysa true döndür
            }
            else
            {
                utils.serialWrite("!!!!!file.size() > 500 acılamadı2!!!!");
                utils.serialWrite("  <<<<<<<<<<<<<logAl<<<<<<<<<<<<<< ");
                utils.serialWrite(" ");
                return false;
            }
        }
        else
        {
            file.println(logStr); // Yeni log mesajını yeni satır olarak dosya sonuna ekle                // log dosyasını seriale yazdır
            file.close();         // dosya kapatma
            utils.serialWrite("  logAl OK ");
            utils.serialWrite("  <<<<<<<<<<<<<logAl<<<<<<<<<<<<<< ");
            utils.serialWrite(" ");
            return true; // işlemler başarılıysa true döndür
        }
    }
    else
    {
        utils.serialWrite("!!!!!Log dosyası acılamadı!!!!");
        utils.serialWrite("  <<<<<<<<<<<<<logAl<<<<<<<<<<<<<< ");
        utils.serialWrite(" ");
        return false; // dosya açılamadıysa false döndür
    }
}

void espRestart()
{
    esp_restart();
}

void ayarlarioku()
{
    DevConfig.begin("userData", false);

    // Versiyon kontrolü
    String kayitliProje = DevConfig.getString("ProjectName", "");
    String kayitliVersiyon = DevConfig.getString("FirmwareVersion", "");

    String mevcutProje = String(_VERSION_Project);
    String mevcutVersiyon = String(_VERSION_String);

    if (kayitliProje != mevcutProje)
    {
        utils.serialWrite("İlk açılış tespit edildi - Temiz başlangıç yapılıyor");
        DevConfig.end();

        // LittleFS'yi formatla
        LittleFS.format();
        if (!LittleFS.begin(true))
        {
            utils.serialWrite("LittleFS Fail");
            LittleFS.format();
            if (!LittleFS.begin())
            {
                utils.serialWrite("LittleFS Fail2");
            }
        }
        else
        {
            utils.serialWrite("LittleFS ok");
            struct tm timeinfo;
            time_t now = time(nullptr);
            localtime_r(&now, &timeinfo);
            char zaman[12];
            strftime(zaman, sizeof(zaman), "%d/%m/%Y", &timeinfo);
            logAl(zaman, " firstRun");
        }

        // Varsayılan ayarları yükle
        restoreDefaults();

        // // Yeniden başlat
        // ESP.restart();
    }
    else if (kayitliVersiyon != mevcutVersiyon)
    {
        utils.serialWrite("Yeni yazılım versiyonu tespit edildi");
        logAl("Versiyon güncelleme: ", kayitliVersiyon + " -> " + mevcutVersiyon);
        DevConfig.end();

        // Ayarları sıfırla ama logları koru
        restoreDefaults();
        // // Yeniden başlat
        // ESP.restart();
    }

    // cihaz bilgilerini al
    DevName = DevConfig.getString("DevName", "ToioT");

    // Mevcut ayar okuma işlemleri aynen devam...
    // Pin ayarları
    SdaPin = DevConfig.getInt("SdaPin", SdaPin);
    SclPin = DevConfig.getInt("SclPin", SclPin);
    BLedPin = DevConfig.getInt("BLedPin", BLedPin);
    BtnPin = DevConfig.getInt("BtnPin", BtnPin);

    // UI ayarları
    UiGiris = DevConfig.getInt("UiGiris", UiGiris);

    // WiFi ayarları
    wifiManager.wifiMode = DevConfig.getInt("WifiMode", WifiOff_);
    wifiManager.portalMode = DevConfig.getInt("PortalMode", PortalOff_);
    wifiManager.PortalPsk = DevConfig.getString("PortalPsk", "toiot1234");
    wifiManager.wifiPower = DevConfig.getInt("WifiPower", WifiPwrMed_);
    wifiManager.wifiChannel = DevConfig.getInt("WifiChannel", 0);
    wifiManager.lastIP = DevConfig.getString("LastIP", "");
    wifiManager.portalBtn = DevConfig.getInt("PortalBtn", PortalBtnPressed_);
    if (wifiManager.portalBtn == PortalBtnPressed_)
    {
        DevConfig.putInt("PortalBtn", PortalClosed_);
    }

    DevConfig.end();

    TimeSetup(); // Zaman ayarlarını yap

    if (TestMode == true)
    {
        utils.acilisTest();
    }
}

void restoreDefaults()
{
    logAl("restoreDefaults");
    utils.serialWrite("fabrika sifirlama");
    vTaskDelay(100);
    nvs_flash_erase();
    nvs_flash_init();
    DevConfig.begin("userData", false);

    // cihaz bilgilerini sıfırla
    DevConfig.putString("DevName", "ToioT");

    // Pin ayarları
    DevConfig.putInt("SdaPin", SdaPin);
    DevConfig.putInt("SclPin", SclPin);
    DevConfig.putInt("BLedPin", BLedPin);
    DevConfig.putInt("BtnPin", BtnPin);

    // UI ayarları
    DevConfig.putInt("UiGiris", 1);

    // WiFi ayarları
    DevConfig.putInt("WifiMode", WifiOff_);
    DevConfig.putInt("PortalMode", PortalOn_);
    DevConfig.putString("PortalPsk", "");
    DevConfig.putInt("WifiPower", WifiPwrMed_);
    DevConfig.putInt("WifiChannel", 0);
    DevConfig.putString("LastIP", "");
    DevConfig.putInt("PortalBtn", PortalBtnPressed_);

    // Yeni proje bilgilerini kaydet
    DevConfig.putString("ProjectName", _VERSION_Project);
    DevConfig.putString("FirmwareVersion", _VERSION_String);

    DevConfig.end();
    espRestart();
}

bool pinAyarla()
{
    pinMode(io0, INPUT);           //
    pinMode(io1, INPUT);           //
    pinMode(io2, INPUT);           //
    pinMode(io3, INPUT);           //
    pinMode(io4, INPUT);           //
    pinMode(io5, INPUT_PULLDOWN);  //
    pinMode(io6, INPUT_PULLDOWN);  //
    pinMode(io7, INPUT_PULLDOWN);  //
    pinMode(io8, INPUT_PULLDOWN);  //
    pinMode(io9, INPUT_PULLDOWN);  //
    pinMode(io10, INPUT_PULLDOWN); //
    // pinMode(io18, INPUT_PULLUP); // usb
    // pinMode(io19, INPUT_PULLUP); // usb
    // pinMode(io20, INPUT_PULLDOWN); // rx
    // pinMode(io21, INPUT_PULLDOWN); // tx

    // pinleri yazdır
    utils.serialWrite("   SdaPin ", String(SdaPin), "  SclPin ", String(SclPin));
    utils.serialWrite("   BLedPin ", String(BLedPin), "  BtnPin ", String(BtnPin));

    if (BLedPin > 0)
    {
        ledcSetup(BLedChannel_, BLedFrekans_, BLedResolution_);
        ledcAttachPin(BLedPin, BLedChannel_);
    }

    if (BtnPin > 0)
    {
        pinMode(BtnPin, INPUT_PULLUP);
    }

    return true;
}

void portalButonOku(int8_t pin) // sıfırla komutunu hubada gönder ******
{                               // ?
    if (pin < 0)
    {
        return;
    }

    uint32_t TetikZaman = millis();
    if (digitalRead(pin) == BtnTrig)
    {
        utils.serialWrite("buton tetik");

        while (millis() < TetikZaman + 50)
        { // parazit filtre
            if (digitalRead(pin) != BtnTrig)
                return;
            vTaskDelay(1);
        }

        while (digitalRead(pin) == BtnTrig)
        {
            if (millis() - TetikZaman > ButonSifirlaSure_)
            {
                ledcWrite(BLedChannel_, 0);
                vTaskDelay(50);
                ledcWrite(BLedChannel_, 255);
                vTaskDelay(50);
            }
            else
            {
                ledcWrite(BLedChannel_, 255);
                vTaskDelay(50);
            }
        }

        if (millis() - TetikZaman > ButonSifirlaSure_)
        { // 5snden falza basılmışsa tüm ayarları sil
            restoreDefaults();
        }
        else
        {
            DevConfig.begin("userData", false);
            DevConfig.putInt("PortalBtn", PortalBtnPressed_);
            DevConfig.end();
            espRestart();
        }
    }
}

#endif // _funch_h
