#include "header.h"
#include "time.h"
#include "esp_sntp.h"

#ifndef TIME_GLOBAL_H
#define TIME_GLOBAL_H

#define Ms1Hafta_ 604800000
#define Ms2Gun_ 172800000
#define Ms1Gun_ 86400000
#define Ms20Saat_ 72000000
#define Ms10Saat_ 36000000
#define Ms1Saat_ 3600000
#define Ms30Dakika_ 1800000
#define Ms15Dakika_ 900000
#define Ms10Dakika_ 600000
#define Ms5Dakika_ 300000
#define Ms1Dakika_ 60000

#define NtpServer1_ "pool.ntp.org"
#define NtpServer2_ "time.nist.gov"
#define DaylightOffset_sec_ 0
int16_t GmtOffset = 3; // 3 saat

bool TimeUpdated = false;



extern void TimeSetup();
extern String printLocalTime();
extern void timeavailable(struct timeval *t);

#elif !defined(TIME_LOCAL_H)
#define TIME_LOCAL_H
#pragma once

void TimeSetup()
{

    // NTP sunucularını ve zaman dilimini ayarla
    configTime(GmtOffset * 3600, DaylightOffset_sec_, NtpServer1_, NtpServer2_);

    // Callback ile zaman güncellemesini takip et
    sntp_set_time_sync_notification_cb(timeavailable); // Zaman güncellemesini takip et
    sntp_servermode_dhcp(1); // DHCP'den NTP sunucularını al
}

String printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        utils.serialWrite("Zaman bilgisi alınamadı");
        return " time fail!";
    }
    char timeStr[32];
    strftime(timeStr, sizeof(timeStr), "%d.%m.%Y %H:%M", &timeinfo);
    // utils.serialWrite(timeStr);  // String olarak gönder
    return timeStr;
}

void timeavailable(struct timeval *t)
{
  TimeUpdated = true;
  printLocalTime();
}

#endif