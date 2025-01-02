#include "header.h"
#ifndef FUNCH_GLOBAL_H
#define FUNCH_GLOBAL_H


extern void serialWrite(String mesajj1, String mesajj2, String mesajj3, String mesajj4, String mesajj5, String mesajj6, String mesajj7, String mesajj8, String mesajj9, String mesajj10, String mesajj11, String mesajj12, String mesajj13, String mesajj14, String mesaj15);
extern bool logAl(String mesajj1, String mesajj2);
extern void espRestart();
extern void fabrikaAyarinaDon();
extern void ayarlarioku();
extern void acilisTest();
extern bool pinAyarla();
extern void acilisLed();
extern void portalButonOku(int8_t pin);
extern uint8_t karakterSayisi(String mesaj);
extern float mapFloat(float value, float fromLow, float fromHigh, float toLow, float toHigh);
extern void taskTakip();
extern esp_reset_reason_t resetNedeni();

#elif !defined(FUNCH_LOCAL_H)
#define FUNCH_LOCAL_H
#pragma once

void serialWrite(String mesajj1, String mesajj2 = "", String mesajj3 = "", String mesajj4 = "", String mesajj5 = "", String mesajj6 = "", String mesajj7 = "", String mesajj8 = "", String mesajj9 = "")
{
    if (TestMode == false)
        return;
    struct tm timeinfoo;
    time_t now = time(nullptr);
    localtime_r(&now, &timeinfoo);
    Serial.print(timeinfoo.tm_hour);
    Serial.print(":");
    Serial.print(timeinfoo.tm_min);
    Serial.print(" - ");
    Serial.print(mesajj1);
    Serial.print(mesajj2);
    Serial.print(mesajj3);
    Serial.print(mesajj4);
    Serial.print(mesajj5);
    Serial.print(mesajj6);
    Serial.print(mesajj7);
    Serial.print(mesajj8);
    Serial.print(mesajj9);
    Serial.println();
}

bool logAl(const String mesajj1, const String mesajj2 = "")
{
    serialWrite(" ");
    serialWrite("  >>>>>>>>>>>logAl>>>>>>>>>>>> ");
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
    serialWrite("  :", logStr);
    File file = LittleFS.open("/log.txt", "a"); // dosyayı aç
    if (file)
    {
        if (file.size() > 1500) // Dosya boyutunu kontrol et
        {
            serialWrite("  ****file.size() > 1500****  ");
            vTaskDelay(500);
            file.close();                          // dosya kapat
            file = LittleFS.open("/log.txt", "r"); // dosyayı oku
            if (!file)
            {
                serialWrite("!!!!!file.size() > 500 acılamadı1!!!!");
                serialWrite("  <<<<<<<<<<<<<logAl<<<<<<<<<<<<<< ");
                serialWrite(" ");
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
                serialWrite("!!!!!file.size() > 500 acılamadı2!!!!");
                serialWrite("  <<<<<<<<<<<<<logAl<<<<<<<<<<<<<< ");
                serialWrite(" ");
                return false;
            }
        }
        else
        {
            file.println(logStr); // Yeni log mesajını yeni satır olarak dosya sonuna ekle                // log dosyasını seriale yazdır
            file.close();         // dosya kapatma
            serialWrite("  logAl OK ");
            serialWrite("  <<<<<<<<<<<<<logAl<<<<<<<<<<<<<< ");
            serialWrite(" ");
            return true; // işlemler başarılıysa true döndür
        }
    }
    else
    {
        serialWrite("!!!!!Log dosyası acılamadı!!!!");
        serialWrite("  <<<<<<<<<<<<<logAl<<<<<<<<<<<<<< ");
        serialWrite(" ");
        return false; // dosya açılamadıysa false döndür
    }
}

void espRestart()
{
    esp_restart();
}

void acilisTest()
{

    Serial.println();
    Serial.print(">>>--->>> ");
    Serial.println("ayarlari oku");

    nvs_open("veri", NVS_READONLY, &nvsHandle);
    nvs_stats_t nvs_stats;
    nvs_get_stats("nvs", &nvs_stats);
    nvs_iterator_t it = nvs_entry_find("nvs", "veri", NVS_TYPE_ANY);
    while (it != NULL)
    {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info); // Anahtar bilgilerini al
        printf("%s: ", info.key);  // Anahtar adını yazdır

        // String okuma denemesi
        char valueStr[32];
        size_t size = sizeof(valueStr);
        if (nvs_get_str(nvsHandle, info.key, valueStr, &size) == ESP_OK)
        {
            printf("(string): %s\n", valueStr);
        }
        else
        {
            // Integer denemesi
            int32_t valueInt;
            if (nvs_get_i32(nvsHandle, info.key, &valueInt) == ESP_OK)
            {
                printf("(int): %d\n", valueInt);
            }
            else
            {
                // Float denemesi
                float valueFloat;
                size = sizeof(valueFloat);
                if (nvs_get_blob(nvsHandle, info.key, &valueFloat, &size) == ESP_OK)
                {
                    printf("(float): %.2f\n", valueFloat);
                }
            }
        }

        // Bir sonraki girişe geç
        it = nvs_entry_next(it);
    }

    // NVS iteratoru serbest bırak
    nvs_release_iterator(it);
    nvs_close(nvsHandle); // NVS'yi kapat

    Serial.print("nvs free_entries ");
    Serial.println(nvs_stats.free_entries);
    Serial.print("nvs namespace_count ");
    Serial.println(nvs_stats.namespace_count);
    Serial.print("nvs total_entries ");
    Serial.println(nvs_stats.total_entries);
    Serial.print("nvs used_entries ");
    Serial.println(nvs_stats.used_entries);

    Serial.print("LittleFS Total: ");
    Serial.println(LittleFS.totalBytes());
    Serial.print("LittleFS Kullanilan: ");
    Serial.println(LittleFS.usedBytes());

    // Serial.print("Model ");
    // Serial.println(Model);
    Serial.print("Donanim ");
    Serial.println(Donanim + DonanimVer);
    // Serial.print("Yazilim ");
    // Serial.println(Yazilim);
    Serial.print("sta mac adres ");
    Serial.println(WiFi.macAddress());
    Serial.print("ap mac adres ");
    Serial.println(WiFi.softAPmacAddress());
    Serial.print("cpu ");
    Serial.print(ESP.getCpuFreqMHz());
    Serial.println(" MHz");
    Serial.print("ChipModel ");
    Serial.println(ESP.getChipModel());
    Serial.print("ChipRevision ");
    Serial.println(ESP.getChipRevision());
    Serial.print("SdkVersion ");
    Serial.println(ESP.getSdkVersion());
    Serial.print("flash ");
    Serial.print(ESP.getFlashChipSpeed() / 1000000);
    Serial.println(" MHz");
    struct tm timeinfo;
    time_t now = time(nullptr);
    localtime_r(&now, &timeinfo);
    char zaman[32];
    strftime(zaman, sizeof(zaman), "%d/%m/%Y %H:%M", &timeinfo);
    Serial.println(zaman);
    Serial.println("<<<---<<<");
    Serial.println();
}

void ayarlarioku()
{
    ayarlar.begin("veri", false);
    if (ayarlar.getInt("acilis", 0) == 0)
    {
        ayarlar.end();
        fabrikaAyarinaDon();
        vTaskDelay(100);
        ESP.restart();
    }


    if (TestMode == true)
    {
        acilisTest();
    }

    // configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
}

void fabrikaAyarinaDon()
{
    logAl("fabrika sifirla");
    serialWrite("fabrika sifirlama");
    vTaskDelay(100);
    nvs_flash_erase();
    nvs_flash_init();
    ayarlar.begin("veri", false);


    ayarlar.putInt("acilis", 1); // ilk kez çalıştığını anla

    ayarlar.end();
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
    serialWrite("   SdaPin ", String(SdaPin), "  SclPin ", String(SclPin));
    serialWrite("   BLedPin ", String(BLedPin), "  ButonPin ", String(ButonPin));


    if (BLedPin > 0)
    {
        ledcSetup(BLedChannel_, BLedFrekans_, BLedResolution_);
        ledcAttachPin(BLedPin, BLedChannel_);
    }

    if (ButonPin > 0)
    {
        pinMode(ButonPin, INPUT_PULLUP);
    }

    return true;

}

void acilisLed()
{

    for (uint8_t i = 0; i < 250; i++)
    {
        ledcWrite(BLedChannel_, i);
        vTaskDelay(1);
    }
    vTaskDelay(200);

    for (uint8_t i = 255; i > 10; i--)
    {
        ledcWrite(BLedChannel_, i);
        vTaskDelay(1);
    }
    vTaskDelay(200);

    for (uint8_t i = 0; i < 250; i++)
    {
        ledcWrite(BLedChannel_, i);
        vTaskDelay(1);
    }
    vTaskDelay(200);

    for (uint8_t i = 255; i > 10; i--)
    {
        ledcWrite(BLedChannel_, i);
        vTaskDelay(1);
    }
    vTaskDelay(200);

    for (uint8_t i = 0; i < 250; i++)
    {
        ledcWrite(BLedChannel_, i);
        vTaskDelay(1);
    }
    vTaskDelay(300);
    ledcWrite(BLedChannel_, BLedSondur);
}

void portalButonOku(int8_t pin) // sıfırla komutunu hubada gönder ******
{                               // ?
    if (pin < 0)
    {
        return;
    }

    uint32_t TetikZaman = millis();
    if (digitalRead(pin) == ButonTetik)
    {
        serialWrite("buton tetik");

        while (millis() < TetikZaman + 50)
        { // parazit filtre
            if (digitalRead(pin) != ButonTetik)
                return;
            vTaskDelay(1);
        }

        while (digitalRead(pin) == ButonTetik)
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
            nvs_flash_erase();
            nvs_flash_init();
            espRestart();
        }
        else
        {
            ayarlar.begin("veri", false);
            // ayarlar.putInt("PortalAcBtn", PortalButonBasilmis_);
            ayarlar.end();
            espRestart();
        }
    }
}

float mapFloat(float value, float fromLow, float fromHigh, float toLow, float toHigh)
{
    float mappedValue = (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
    return mappedValue;
}

void taskTakip()
{

}

esp_reset_reason_t resetNedeni()
{
    serialWrite("");
    serialWrite("------------------");
    esp_reset_reason_t reason = esp_reset_reason();
    serialWrite(String((int)reason));
    switch (reason)
    {
    case ESP_RST_POWERON:
        serialWrite(String((int)reason), " Power on reset");
        logAl("PwrOnRst: ", String((int)reason));
        break;
    case ESP_RST_EXT:
        serialWrite(String((int)reason), " Reset by external pin");
        logAl("PinRst: ", String((int)reason));
        break;
    case ESP_RST_SW:
        serialWrite(String((int)reason), " Software reset");
        logAl("SoftRst: ", String((int)reason));
        break;
    case ESP_RST_PANIC:
        serialWrite(String((int)reason), " Software reset panic");
        logAl("SoftPanicRst: ", String((int)reason));
        break;
    case ESP_RST_INT_WDT:
        serialWrite(String((int)reason), " Reset by interrupt watchdog");
        logAl("IntrrWdogRst: ", String((int)reason));
        break;
    case ESP_RST_TASK_WDT:
        serialWrite(String((int)reason), " Reset by task watchdog");
        logAl("TskWdogRst: ", String((int)reason));
        break;
    case ESP_RST_WDT:
        serialWrite(String((int)reason), " Reset by watchdog");
        logAl("WdogRst: ", String((int)reason));
        break;
    case ESP_RST_DEEPSLEEP:
        serialWrite(String((int)reason), " Reset after exiting deep sleep mode");
        logAl("DpSlpRst: ", String((int)reason));
        break;
    case ESP_RST_BROWNOUT:
        serialWrite(String((int)reason), " Brownout reset");
        logAl("BrownoutRst: ", String((int)reason));
        break;
    case ESP_RST_SDIO:
        serialWrite(String((int)reason), " Reset over SDIO");
        logAl("SDIORst: ", String((int)reason));
        break;
    default:
        serialWrite(String((int)reason), " Reset reason can not be determined");
        logAl("TanımsızRst: ", String((int)reason));
        break;
    }
    serialWrite("------------------");
    serialWrite("");
    return reason;
}

uint8_t karakterSayisi(String str)
{
    if (str.length() > 250)
    {
        return 255;
    }

    uint8_t sayac = 0;
    for (uint8_t i = 0; i < str.length(); i++)
    {
        char c = str[i];
        if ((c & 0xC0) != 0x80)
        { // UTF-8 karakterinin ilk byte'ı mı kontrol ediliyor
            sayac++;
        }
    }
    return sayac;
}


#endif // _funch_h
