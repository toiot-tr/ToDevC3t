/*
pio run -t upload --upload-port 3.3.3.3 > tee pyScript/uplog.txt
*/

#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <nvs_flash.h>
#include "FS.h"
#include <LittleFS.h>
#include <Wire.h>
#include <SPI.h>





#define FORMAT_LITTLEFS_IF_FAILED true
bool TestMode = true;
// Semaphore handele

// queue_handele
QueueHandle_t bledQueue;


nvs_handle_t nvsHandle;
Preferences ayarlar;




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


// donanaım
String Donanim = "ToDevC3tf";  // donanım
String DonanimVer = "1.0.0"; // donanım versiyonu

// cihaz_ayarları *****************************************************
// String Model = _VERSION_Project; // donanım
// String Yazilim = _VERSION_String; // yzlmv   webte versiyon
String CihazOnEk = "ToD";
String CihazAdi = "ToDevC3";

#define HataYok_ 0
#define HataVar_ 1

// Kullanılan pinleri saklamak için bir dizi
#define PinCakisma_ -2
#define PinKapali_ -1
#define PinOk_ 0
#define PinBosta_ false
#define PinDolu_ true
bool PinKullanildi[11] = {PinBosta_}; // 22 pin var varsayıyoruz

// i2c_pin **********
bool I2cOk = false;
int16_t SdaPin = 8; // i2c master SDA veri gönderme data
int16_t SclPin = 9; // i2c master SCL veri gönderme cloak

// bildirim_ledi_ayarları *****************************************************
#define BLedChannel_ 0
#define BLedFrekans_ 1000
#define BLedResolution_ 8
#define BLedHata_ -1
#define BLedAcilis_ 0
#define BledPortal_ 1
int16_t BLedPin = io5;
int8_t BLedSondur = 0;
int8_t BledDurum = BLedAcilis_;

// buton_ayarları *****************************************************
#define ButonSifirlaSure_ 5000
int16_t ButonPin = io6;
int8_t ButonTetik = false;




#include "funch.h"
#include "f_display.h"
#include "f_sens.h"
#include "f_time.h"
#include "f_wifi.h"
#include "taskk.h"

#include "funch.h"
#include "f_display.h"
#include "f_sens.h"
#include "f_time.h"
#include "f_wifi.h"
#include "taskk.h"

