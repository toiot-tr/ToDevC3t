/*
pio run -t upload --upload-port 3.3.3.3 > tee pyScript/uplog.txt
*/

#ifndef HEADER_H
#define HEADER_H

#include <Arduino.h>
#include <Preferences.h>
#include <nvs_flash.h>
#include "FS.h"
#include <LittleFS.h>
#include <Wire.h>
#include <SPI.h>
#include "rev.h"
#include <esp_task_wdt.h>  // Watchdog timer başlık dosyasını ekle

#include "esp32-hal.h"  
#include "esp_system.h"
#include "esp_attr.h"


#define FORMAT_LITTLEFS_IF_FAILED true
bool TestMode = true;
// Semaphore handele

// queue_handele
QueueHandle_t bledQueue;


nvs_handle_t nvsHandle;
// Kalıcı ayarlar için
Preferences DevConfig;            // Device Configuration kısaltması
Preferences ssidlist;            // Device Configuration kısaltması




// pin ayarları *****************************************************
//              ToDevC3tf v1.0.0  |      
#define io0 0   // bos            |   
#define io1 1   // bos            |  
#define io2 2   // bos            |  
#define io3 3   // bos            |  
#define io4 4   // bos            |     
#define io5 5   // BLed           |     
#define io6 6   // Button         |      
#define io7 7   // FET            |      
#define io8 8   // FET I2C        |  
#define io9 9   // FET I2C        |  
#define io10 10 // PWM            |      
#define io18 18 // usb+           |  
#define io19 19 // usb-           |  
#define io20 20 // rx             | 
#define io21 21 // tx             | 


// Donanım bilgileri
String Hardware = "ToDevC3tf";     // Cihazın donanım modeli
String HardwareVer = "1.0.0";      // Donanım versiyonu
String Pre = "ToD";              // Cihaz isim öneki (ToD_ledkontrolcu)
String DevName = "ToDevC3";     // Cihaz tam adı

#define HataYok_ 0
#define HataVar_ 1

// Pin durumları için sabitler
#define PinCakisma_ -2            // Pin başka bir işlem için kullanılıyor
#define PinKapali_ -1             // Pin devre dışı
#define PinOk_ 0                  // Pin kullanıma hazır
#define PinBosta_ false           // Pin kullanılmıyor
#define PinDolu_ true             // Pin kullanımda
bool PinStatus[11] = {PinBosta_}; // Pin kullanım durumlarını tutan dizi

// I2C pin ayarları
bool I2cOk = false;               // I2C başlatma durumu
int16_t SdaPin = 8;              // I2C veri pini
int16_t SclPin = 9;              // I2C saat pini

// Bildirim LED'i ayarları
#define BLedChannel_ 0            // LED PWM kanalı
#define BLedFrekans_ 1000        // LED PWM frekansı
#define BLedResolution_ 8        // LED PWM çözünürlüğü
#define BLedHata_ -1             // LED hata durumu
#define BLedAcilis_ 0            // LED açılış durumu
#define BledPortal_ 1            // LED portal modu durumu
#define BLedSendData_ 2          // Veri gönderme bildirimi
#define BLedWait_ 3              // Bekleme bildirimi
int16_t BLedPin = io5;           // Bildirim LED'i pini

// Buton ayarları
#define ButonSifirlaSure_ 5000    // Buton sıfırlama için basılı tutma süresi (ms)
int16_t BtnPin = io6;            // Buton pini
int8_t BtnTrig = false;          // Buton tetiklenme durumu

// çalışma istatistikleri
uint32_t UptimeDay = 0;
uint32_t MaxCpuTemp = 0;
uint32_t CpuUsage = 0;
uint32_t WifiUsage = 0;



#include "taskk.h"
#include "funch.h"
#include "f_display.h"
#include "f_sens.h"
#include "f_time.h"
#include "f_wifi.h"
#include "utils.h"  // En sona ekleyelim


#include "taskk.h"
#include "funch.h"
#include "f_display.h"
#include "f_sens.h"
#include "f_time.h"
#include "f_wifi.h"
// #include "wifimanager.h"



#endif

