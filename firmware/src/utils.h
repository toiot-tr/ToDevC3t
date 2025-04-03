#ifndef UTILS_H
#define UTILS_H

#include "header.h"

// Global Utils sınıfı ve nesnesi
class Utils
{
public:
    static void serialWrite(String mesaj1, String mesaj2 = "", String mesaj3 = "", String mesaj4 = "",
                            String mesaj5 = "", String mesaj6 = "", String mesaj7 = "", String mesaj8 = "",
                            String mesaj9 = "")
    {
        if (!TestMode)
            return;

        struct tm timeinfo;
        time_t now = time(nullptr);
        localtime_r(&now, &timeinfo);

        Serial.print(timeinfo.tm_hour);
        Serial.print(":");
        Serial.print(timeinfo.tm_min);
        Serial.print(" - ");
        Serial.print(mesaj1);
        Serial.print(mesaj2);
        Serial.print(mesaj3);
        Serial.print(mesaj4);
        Serial.print(mesaj5);
        Serial.print(mesaj6);
        Serial.print(mesaj7);
        Serial.print(mesaj8);
        Serial.print(mesaj9);
        Serial.println();
    }

    void acilisTest()
    {
        Serial.println("\n>>>--->>> Sistem Bilgileri <<<---<<<\n");

        // Donanım Bilgileri
        Serial.println("=== Donanım Bilgileri ===");
        Serial.printf("Hardware: %s%s\n", Hardware.c_str(), HardwareVer.c_str());
        Serial.printf("Chip Model: %s\n", ESP.getChipModel());
        Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
        Serial.printf("CPU Frekansı: %d MHz\n", ESP.getCpuFreqMHz());
        Serial.printf("Çalışan Çekirdek: %d\n", xPortGetCoreID());
        Serial.printf("Sıcaklık: %.2f°C\n", getTemperature());

        // Bellek Bilgileri
        Serial.println("\n=== Bellek Bilgileri ===");
        Serial.printf("Heap Boyutu: %d bytes\n", ESP.getHeapSize());
        Serial.printf("Boş Heap: %d bytes\n", ESP.getFreeHeap());
        Serial.printf("Min Boş Heap: %d bytes\n", ESP.getMinFreeHeap());
        Serial.printf("Max Alloc Heap: %d bytes\n", ESP.getMaxAllocHeap());

        if (psramFound())
        {
            Serial.printf("PSRAM Boyutu: %d bytes\n", ESP.getPsramSize());
            Serial.printf("Boş PSRAM: %d bytes\n", ESP.getFreePsram());
        }

        // Depolama Bilgileri
        Serial.println("\n=== Depolama Bilgileri ===");
        Serial.printf("Flash Boyutu: %d bytes\n", ESP.getFlashChipSize());
        Serial.printf("Flash Hızı: %d MHz\n", ESP.getFlashChipSpeed() / 1000000);
        Serial.printf("LittleFS Toplam: %d bytes\n", LittleFS.totalBytes());
        Serial.printf("LittleFS Kullanılan: %d bytes\n", LittleFS.usedBytes());

        // NVS Bilgileri
        Serial.println("\n=== NVS Bilgileri ===");
        nvs_stats_t nvs_stats;
        nvs_get_stats("nvs", &nvs_stats);
        Serial.printf("Boş Girişler: %d\n", nvs_stats.free_entries);
        Serial.printf("Namespace Sayısı: %d\n", nvs_stats.namespace_count);
        Serial.printf("Toplam Girişler: %d\n", nvs_stats.total_entries);
        Serial.printf("Kullanılan Girişler: %d\n", nvs_stats.used_entries);

        // Ağ Bilgileri
        Serial.println("\n=== Ağ Bilgileri ===");
        Serial.printf("STA MAC: %s\n", WiFi.macAddress().c_str());
        Serial.printf("AP MAC: %s\n", WiFi.softAPmacAddress().c_str());

        // Yazılım Bilgileri
        Serial.println("\n=== Yazılım Bilgileri ===");
        Serial.printf("SDK Sürümü: %s\n", ESP.getSdkVersion());
        Serial.printf("IDF Sürümü: %s\n", esp_get_idf_version());

        struct tm timeinfo;
        time_t now = time(nullptr);
        localtime_r(&now, &timeinfo);
        char zaman[32];
        strftime(zaman, sizeof(zaman), "%d/%m/%Y %H:%M", &timeinfo);
        Serial.printf("Sistem Zamanı: %s\n", zaman);

        // NVS İçeriği
        Serial.println("\n=== NVS İçeriği ===");
        nvsPrint("");

        Serial.println("\n>>>--->>> Sistem Bilgileri Sonu <<<---<<<\n");
    }

    void nvsPrint(String nvsName = "")
    {
        nvs_iterator_t it = NULL;
        nvs_handle_t handle; // Tek bir handle tanımı

        if (nvsName == "")
        {
            // Tüm namespace'ler için
            it = nvs_entry_find("nvs", NULL, NVS_TYPE_ANY);
        }
        else
        {
            // Belirli bir namespace için
            nvs_open(nvsName.c_str(), NVS_READONLY, &handle);
            it = nvs_entry_find("nvs", nvsName.c_str(), NVS_TYPE_ANY);
        }

        while (it != NULL)
        {
            nvs_entry_info_t info;
            nvs_entry_info(it, &info);

            // Namespace ve key bilgisini yazdır
            printf("Namespace: %s, Key: %s, Type: ", info.namespace_name, info.key);

            // Değeri okumak için namespace'i aç
            nvs_open(info.namespace_name, NVS_READONLY, &handle);

            // Veri tipine göre okuma yap
            switch (info.type)
            {
            case NVS_TYPE_U8:
            {
                uint8_t value;
                nvs_get_u8(handle, info.key, &value);
                printf("(uint8): %u\n", value);
                break;
            }
            case NVS_TYPE_I8:
            {
                int8_t value;
                nvs_get_i8(handle, info.key, &value);
                printf("(int8): %d\n", value);
                break;
            }
            case NVS_TYPE_U16:
            {
                uint16_t value;
                nvs_get_u16(handle, info.key, &value);
                printf("(uint16): %u\n", value);
                break;
            }
            case NVS_TYPE_I16:
            {
                int16_t value;
                nvs_get_i16(handle, info.key, &value);
                printf("(int16): %d\n", value);
                break;
            }
            case NVS_TYPE_U32:
            {
                uint32_t value;
                nvs_get_u32(handle, info.key, &value);
                printf("(uint32): %u\n", value);
                break;
            }
            case NVS_TYPE_I32:
            {
                int32_t value;
                nvs_get_i32(handle, info.key, &value);
                printf("(int32): %d\n", value);
                break;
            }
            case NVS_TYPE_STR:
            {
                size_t required_size;
                nvs_get_str(handle, info.key, NULL, &required_size);
                char *value = (char *)malloc(required_size);
                if (value)
                {
                    nvs_get_str(handle, info.key, value, &required_size);
                    printf("(string): %s\n", value);
                    free(value);
                }
                break;
            }
            case NVS_TYPE_BLOB:
            {
                size_t required_size;
                nvs_get_blob(handle, info.key, NULL, &required_size);
                uint8_t *value = (uint8_t *)malloc(required_size);
                nvs_get_blob(handle, info.key, value, &required_size);
                printf("(blob): size=%d bytes\n", required_size);
                free(value);
                break;
            }
            default:
                printf("unknown type\n");
                break;
            }

            nvs_close(handle);
            it = nvs_entry_next(it);
        }
        nvs_release_iterator(it);
    }

    void acilisLed()
    {
        if (BLedPin < 0)
            return;

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
        ledcWrite(BLedChannel_, 0);
    }

    float mapFloat(float value, float fromLow, float fromHigh, float toLow, float toHigh)
    {
        return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
    }

    uint8_t karakterSayisi(String str)
    {
        if (str.length() > 250)
            return 255;

        uint8_t sayac = 0;
        for (uint8_t i = 0; i < str.length(); i++)
        {
            if ((str[i] & 0xC0) != 0x80)
                sayac++;
        }
        return sayac;
    }

    esp_reset_reason_t resetNedeni()
    {
        esp_reset_reason_t reason = esp_reset_reason();
        serialWrite("Reset nedeni: ", String((int)reason));

        switch (reason)
        {
        case ESP_RST_POWERON:
            serialWrite("Power on reset");
            logAl("Power on reset");
            break;
        case ESP_RST_EXT:
            serialWrite("External pin reset");
            logAl("External pin reset");
            break;
        case ESP_RST_SW:
            serialWrite("Software reset");
            logAl("Software reset");
            break;
        case ESP_RST_PANIC:
            serialWrite("Software reset (panic)");
            logAl("Software reset (panic)");
            break;
        case ESP_RST_INT_WDT:
            serialWrite("Interrupt watchdog reset");
            logAl("Interrupt watchdog reset");
            break;
        case ESP_RST_TASK_WDT:
            serialWrite("Task watchdog reset");
            logAl("Task watchdog reset");
            break;
        case ESP_RST_WDT:
            serialWrite("Watchdog reset");
            logAl("Watchdog reset");
            break;
        case ESP_RST_DEEPSLEEP:
            serialWrite("Deep sleep reset");
            logAl("Deep sleep reset");
            break;
        case ESP_RST_BROWNOUT:
            serialWrite("Brownout reset");
            logAl("Brownout reset");
            break;
        case ESP_RST_SDIO:
            serialWrite("SDIO reset");
            logAl("SDIO reset");
            break;
        default:
            serialWrite("bilinmeyen reset");
            logAl("bilinmeyen reset");
            break;
        }

        return reason;
    }

    void checkMemoryHealth()
    {

        size_t largestBlock = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
        if (largestBlock < 1024 * 50) // 50KB altında loglama yap
        {
            logAl("Bellek 50KB altında!");
            espRestart();
        }
    }
};

static Utils utils; // Global nesne

#endif