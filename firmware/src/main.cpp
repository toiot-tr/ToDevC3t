#include "header.h"

void setup()
{
    if (TestMode == true)
    {
        vTaskDelay(5000);
    }
    bledQueue = xQueueCreate(10, sizeof(int8_t));

    // Seri port başlatma
    Serial.begin(115200);
    if (TestMode == true)
    {
        vTaskDelay(10000);
        Serial.setDebugOutput(true);
    }
    else
    {
        Serial.setDebugOutput(false);
    }
    vTaskDelay(100);

    // Başlangıç mesajı
    utils.serialWrite(_VERSION_Project, " başlatılıyor...");

    if (!LittleFS.begin(true))
    {
        utils.serialWrite("LittleFS Fail");
        LittleFS.format();
        if (!LittleFS.begin())
        {
            utils.serialWrite("LittleFS Fail2");
        }
        else
        {
            logAl("FS Fail");
            logAl("log format");
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
        logAl(zaman, " start");
    }

    utils.resetNedeni();
    setupSensor();

    ayarlarioku();
    pinAyarla();

    xTaskCreatePinnedToCore(bLedTask, "bLedTask_", 1024 * 2, NULL, 2, &bLedTask_, tskNO_AFFINITY);
    vTaskDelay(500); // needed to start-up task1

    int8_t bLedMode = BLedAcilis_;
    xQueueSend(bledQueue, &bLedMode, 0);

    // xTaskCreatePinnedToCore(sensorTask, "sensorTask", 1024 * 2, NULL, 4, &sensorTask_, tskNO_AFFINITY);
    // vTaskDelay(500); // needed to start-up task1

    xTaskCreatePinnedToCore(wifiTask, "wifiTask", 1024 * 16, NULL, 10 /* | portPRIVILEGE_BIT */, &wifiTask_, tskNO_AFFINITY);
    vTaskDelay(500); // needed to start-up task1
}

void loop()
{
    // Görevin periyodik olarak çalışması için zamanlama değişkenleri
    TickType_t xLastWakeTime = xTaskGetTickCount(); // Son uyanma zamanını tutan değişken
    const TickType_t xFrequency = 100;              // Görevin çalışma periyodu (100ms)
    BaseType_t xWasDelayed = pdFALSE;               // Gecikme durumunu tutan değişken

    vTaskPrioritySet(NULL, 5);
    esp_task_wdt_init(220, true); // 60 saniye timeout
    esp_task_wdt_add(NULL);       // Main task'i izlemeye al

    uint32_t weekCheck = 0;   // hafta
    uint32_t dailyCheck = 0;  // gün
    uint32_t hourCheck = 0;   // saat
    uint32_t minuteCheck = 0; // dakika

    for (;;)
    {
        xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);

        esp_task_wdt_reset();

        if (millis() - minuteCheck > Ms1Dakika_) // bir dakikadan fazla olduysa bellek kontrol et
        {
            minuteCheck = millis();
            uint8_t cpuTemp = getTemperature();
            if (cpuTemp > MaxCpuTemp)
            {
                MaxCpuTemp = cpuTemp;
            }
            if (cpuTemp > 100)
            {
                logAl("cpuTemp: ", String(cpuTemp) + "°C !");
                espRestart();
            }

            if (millis() - WifiTaskTime > Ms1Dakika_) // 1 dakikadan fazla olduysa yazdır
            {
                utils.serialWrite("WifiTaskTime: " + String((millis() - WifiTaskTime) / 1000) + " saniyedir çalışmıyor");
            }

            if (millis() - BLedTaskTime > Ms1Dakika_) // 1 dakikadan fazla olduysa yazdır
            {
                utils.serialWrite("BLedTaskTime: " + String((millis() - BLedTaskTime) / 1000) + " saniyedir çalışmıyor");
            }
        }

        if (millis() - hourCheck > Ms1Saat_) // bir saatten fazla olduysa bellek kontrol et
        {
            hourCheck = millis();
            utils.checkMemoryHealth();
            if (TestMode == true) // test modundaysa
            {
                taskTakip();
            }
        }

        if (millis() - dailyCheck > Ms1Gun_) // bir günden fazla olduysa bellek kontrol et
        {
            dailyCheck = millis();
            UptimeDay++;
        }

        if (millis() - weekCheck > Ms1Hafta_) // bir haftadan fazla olduysa bellek kontrol et
        {
            espRestart();
        }

        portalButonOku();

        vTaskDelay(100);
    }
}