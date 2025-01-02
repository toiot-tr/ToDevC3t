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
    Serial.println("ToDevC3tf başlatılıyor...");

    if (!LittleFS.begin(true))
    {
        serialWrite("LittleFS Fail");
        LittleFS.format();
        if (!LittleFS.begin())
        {
            serialWrite("LittleFS Fail2");
        }
        else
        {
            logAl("FS Fail");
            logAl("log format");
        }
    }
    else
    {
        serialWrite("LittleFS ok");
        struct tm timeinfo;
        time_t now = time(nullptr);
        localtime_r(&now, &timeinfo);
        char zaman[12];
        strftime(zaman, sizeof(zaman), "%d/%m/%Y", &timeinfo);
        logAl(zaman, " start");
    }

    resetNedeni();
    ayarlarioku();
    pinAyarla();

    wifiPowerSet();

    xTaskCreatePinnedToCore(bledTask, "bledTask_", 1024 * 2, NULL, 1, &bledTask_, tskNO_AFFINITY);
    vTaskDelay(500); // needed to start-up task1

    BledDurum = BLedAcilis_;
    xQueueSend(bledQueue, &BledDurum, 0); // hata led görevini tetikle

    xTaskCreatePinnedToCore(sensorTask, "sensorTask", 1024 * 24, NULL, configMAX_PRIORITIES || portPRIVILEGE_BIT, &sensorTask_, tskNO_AFFINITY);
    vTaskDelay(500); // needed to start-up task1

    xTaskCreatePinnedToCore(portalTask, "portalTask", 1024 * 16, NULL, 4, &portalTask_, tskNO_AFFINITY);
    vTaskDelay(2000); // needed to start-up task1
}

void loop()
{

    portalButonOku(ButonPin);
    // refVoltajOku();

    vTaskDelay(100);
}