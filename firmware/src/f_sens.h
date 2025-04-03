#include "header.h"
#include "driver/temp_sensor.h"


#ifndef SENS_GLOBAL_H
#define SENS_GLOBAL_H

// Global değişkenler ve fonksiyon tanımlamaları
extern void setupSensor();
extern float getTemperature();
extern void setupTemperatureSensor();


#elif !defined(SENS_LOCAL_H)
#define SENS_LOCAL_H
#pragma once

void setupSensor()
{
    setupTemperatureSensor();
}

void setupTemperatureSensor()
{
    // Sıcaklık sensörü yapılandırması
    temp_sensor_config_t temp_sensor = {
        .dac_offset = TSENS_DAC_DEFAULT, // Varsayılan DAC offset
        .clk_div = 6                     // Clock bölücü ayarı
    };

    // Sensörü başlatma ve hata kontrolü
    if (temp_sensor_set_config(temp_sensor) != ESP_OK)
    {
        utils.serialWrite("Sensör yapılandırma hatası!");
        return;
    }

    if (temp_sensor_start() != ESP_OK)
    {
        utils.serialWrite("Sensör başlatma hatası!");
        return;
    }
    getTemperature();
}

float getTemperature()
{
    float temperature;
    // Sıcaklık okuma ve hata kontrolü
    if (temp_sensor_read_celsius(&temperature) == ESP_OK)
    {
        // utils.serialWrite("Çip Sıcaklığı: " + String(temperature) + " °C");
    }
    else
    {
        utils.serialWrite("Çip Sıcaklığı okuma hatası!");
    }
    return temperature;
}



#endif