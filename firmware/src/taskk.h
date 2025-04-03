#include "header.h"

#ifndef TASK_GLOBAL_H
#define TASK_GLOBAL_H

// task handele
TaskHandle_t wifiTask_ = NULL;
TaskHandle_t bLedTask_ = NULL;

uint32_t WifiTaskTime = 0;
uint32_t BLedTaskTime = 0;

extern void wifiTask(void *parameter);
// extern void sensorTask(void *parameter);
extern void bLedTask(void *parameter);
extern void taskTakip();

#elif !defined(TASK_LOCAL_H)
#define TASK_LOCAL_H
#pragma once

void wifiTask(void *parameter)
{
  utils.serialWrite("wifiTask > Çekirdek: " + String(xPortGetCoreID()) + " Öncelik: " + String(uxTaskPriorityGet(NULL)));
  vTaskDelay(5000);
  WifiTaskTime = millis();
  // Görevin periyodik olarak çalışması için zamanlama değişkenleri
  TickType_t xLastWakeTime = xTaskGetTickCount(); // Son uyanma zamanını tutan değişken
  const TickType_t xFrequency = 100;              // Görevin çalışma periyodu (100ms)
  BaseType_t xWasDelayed = pdFALSE;               // Gecikme durumunu tutan değişken

  // bu değişkenleri bool olarak buraya yazıyoruz ki daha sonra portaldan değiştirilirse yeniden başlatmadan aktif olmasınlar
  bool portalOpened = false;
  bool wifiOn = (wifiManager.wifiMode == WifiOn_);
  bool wifiTimeUpdate = (wifiManager.wifiMode == WifiTimeUpdate_);
  uint16_t retryCounter = 8700; // ilk denemeyi 30 sn sonra yap

  // Başlangıçta güç ayarını yap
  wifiManager.setWifiPower();
  wifiManager.loadNetworks();   // liste hazır olsun
  WiFi.setAutoReconnect(false); // Otomatik yeniden bağlanmayı kapat
  WiFi.persistent(false);       // Ayarları flash'a kaydetme

  // Portal butonu kontrolü - Öncelikli olarak kontrol et
  if (wifiManager.portalBtn == PortalBtnPressed_)
  {
    utils.serialWrite("Portal butonu basıldı, portal açılıyor");
    // Portal açılacak, diğer modları sıfırla
    wifiOn = false;
    wifiTimeUpdate = false;
    portalOpened = true;

    wifiManager.startPortal();
    goto wifiLoop;
  }
  else if (wifiManager.portalMode == PortalOff_ && wifiManager.wifiMode == WifiOff_)
  {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    utils.serialWrite("Portal&wifiOFF, wifi kapatılıyor");
    vTaskDelete(NULL);
  }
  else if (wifiManager.portalMode == PortalOn_)
  {
    utils.serialWrite("PortalOn_ modu, portal açılıyor");
    wifiManager.startPortal();
    portalOpened = true;
  }

  // wifimod kontrolü
  if (wifiManager.wifiMode == WifiOn_ || wifiManager.wifiMode == WifiTimeUpdate_)
  {
    wifiManager.connectWifi();
  }

  // PortalNoNet_ modu kontrol et wifi başarısız olmuş veya timeUpdate modunda ise portal aç
  if (wifiManager.portalMode == PortalNoNet_ && (wifiManager.statusWifi != WifiConnected_ || wifiManager.wifiMode == WifiTimeUpdate_))
  {
    wifiManager.startPortal();
    portalOpened = true;
  }

wifiLoop:
  wifiManager.scanNetworks(); // listeyi güncelle
  WifiTaskTime = millis();
  for (;;)
  {
    xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);

    // Web sunucu loop önce çalışsın ki bağlantı kopmuş olsa bile son istek boşalsın
    serverLoop();
    // OTA servisi
    ArduinoOTA.handle();

    // WiFi bağlantı kontrolü
    if (WiFi.status() == WL_CONNECTED)
    {
      retryCounter = 8700; // ilk denemeyi 30 sn sonra yap
      // wifi servisleri handle bu alanda wifi bağlı zaten sadece servisleri çalışacak
      // MDNS güncellemeye gerek yok, başlatmak yeterli
    }
    else if (wifiOn)
    {
      ledcWrite(BLedChannel_, 0);
      retryCounter++;

      if (retryCounter >= 9000) // 9000 * 100ms = 900 saniye = 15dk
      {
        retryCounter = 0;
        utils.serialWrite("WiFi bağlantısı koptumuş, yeniden deneniyor");
        wifiManager.connectWifi();
      }
    }

    if (wifiTimeUpdate)
    {
      if (TimeUpdated)
      {
        // wifiyi kapat ve timeUpdate değişkenini false yap, portalda kapalıysa taskı komple silebiliriz
        if (portalOpened)
        {
          utils.serialWrite("Zaman güncellendi, WiFi kapatılıyor");
          WiFi.disconnect();
          TimeUpdated = false;
        }
        else
        {
          utils.serialWrite("Zaman güncellendi, task siliniyor");
          xTaskNotify(bLedTask_, 0, eSetValueWithOverwrite);
          WiFi.disconnect();
          WiFi.mode(WIFI_OFF);
          vTaskDelete(NULL);
        }
      }
      else if (WiFi.status() != WL_CONNECTED)
      {
        // birsüre sonra tekrar bağlanmaya çalışsın
        retryCounter++;

        if (retryCounter >= 9000) // 9000 * 100ms = 900 saniye = 15dk
        {
          retryCounter = 0;
          utils.serialWrite("TimeUpdate WiFi koptumuş, yeniden deneniyor");
          wifiManager.connectWifi();
        }
      }
    }

    if (portalOpened)
    {
      // portal servisleri handle
      DnsServer.processNextRequest();
    }
    WifiTaskTime = millis();
  }
}

void bLedTask(void *parameter)
{
  vTaskDelay(250); // Başlangıçta sistemin oturması için bekle
  utils.serialWrite("bLedTask > Çekirdek: " + String(xPortGetCoreID()));

  int8_t mode;
  int8_t minBrightness = 0; // BLed Minumum parlaklık değeri

  for (;;)
  {
    // Task Notification Örneği:
    // - Timeout 0 olduğu için beklemeden kontrol eder
    // - Queue'ya göre daha hafif bir haberleşme yöntemi
    uint32_t notification;
    if (xTaskNotifyWait(0, 0, &notification, 0) == pdTRUE)
    {
      minBrightness = (int8_t)notification; // Yeni minimum parlaklık
      utils.serialWrite("LED minimum -> ", String(minBrightness));
    }

    // Queue Örneği:
    // - FIFO (First In First Out) yapısı
    // - 1000ms timeout ile veri bekler
    if (xQueueReceive(bledQueue, &mode, 1000) == pdTRUE)
    {
      switch (mode)
      {
      case BLedHata_:
        // Hata durumunda 10 kez yanıp sön (5 saniye)
        for (uint8_t i = 0; i < 10; i++)
        {
          ledcWrite(BLedChannel_, 255); // Tam parlaklık
          vTaskDelay(100);
          ledcWrite(BLedChannel_, 0); // Kapalı
          vTaskDelay(100);
        }
        ledcWrite(BLedChannel_, minBrightness);
        break;
      case BLedSendData_:
        // Veri gönderirken random parlaklık efekti
        for (uint8_t i = 0; i < 3; i++) // 3 kez yanıp sönsün
        {
          uint8_t brightness = random(100, 255); // Her yanıp sönmede farklı parlaklık
          ledcWrite(BLedChannel_, brightness);
          vTaskDelay(100);
          ledcWrite(BLedChannel_, minBrightness);
          vTaskDelay(100);
        }
        ledcWrite(BLedChannel_, minBrightness);
        break;
      case BLedAcilis_:
        utils.acilisLed(); // Açılış animasyonu
        break;
      case BLedWait_:
        vTaskDelay(20000);
        break;
      }
    }
    else
    {
      ledcWrite(BLedChannel_, minBrightness);
    }
    BLedTaskTime = millis();
  }
}

void taskTakip()
{

  utils.serialWrite("");
  utils.serialWrite("----task---- ");
  utils.serialWrite("Uptime: " + String(millis() / 86400000) + " gün");
  utils.serialWrite("WifiMode ", String(WiFi.getMode()));
  utils.serialWrite("WifiStatus ", String(WiFi.status()));
  utils.serialWrite("Sta ip ", WiFi.localIP().toString());
  utils.serialWrite("Ap ip ", WiFi.softAPIP().toString());
  utils.serialWrite("TCP/IP Stack: ", String(esp_netif_get_nr_of_ifs()));
  utils.serialWrite("Sıcaklık: ", String(getTemperature()) + "°C");
  utils.serialWrite("Max Sıcaklık: ", String(MaxCpuTemp) + "°C");

  utils.serialWrite("main loop ", String(uxTaskGetStackHighWaterMark(NULL)), " core ", String(xTaskGetAffinity(NULL)), " prioty ", String(uxTaskPriorityGet(NULL)));
  if (wifiTask_)
  {
    utils.serialWrite("wifiTask_ ", String(uxTaskGetStackHighWaterMark(wifiTask_)), " core ", String(xTaskGetAffinity(wifiTask_)), " prioty ", String(uxTaskPriorityGet(wifiTask_)));
  }
  else
  {
    utils.serialWrite("wifiTask_ yok");
  }
  if (bLedTask_)
  {
    utils.serialWrite("bLedTask_ ", String(uxTaskGetStackHighWaterMark(bLedTask_)), " core ", String(xTaskGetAffinity(bLedTask_)), " prioty ", String(uxTaskPriorityGet(bLedTask_)));
  }
  else
  {
    utils.serialWrite("bLedTask_ yok");
  }

  utils.serialWrite("bellek: ", String(ESP.getFreeHeap()));
  // 4. Stack Kullanım Detayları
  utils.serialWrite("En Büyük Stack: ", String(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)) + "B");
  // 6. Sistem Sağlığı
  utils.serialWrite("Kesmeler: ", String(uxTaskGetNumberOfTasks()));
  utils.serialWrite("----task---- ");
  utils.serialWrite("");
}

#endif
