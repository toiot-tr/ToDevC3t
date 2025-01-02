#include "header.h"

#ifndef TASK_GLOBAL_H
#define TASK_GLOBAL_H

// task handele
TaskHandle_t portalTask_ = NULL;
TaskHandle_t sensorTask_ = NULL;
TaskHandle_t bledTask_ = NULL;

extern void portalTask(void *parameter);

extern void sensorTask(void *parameter);

extern void bledTask(void *parameter);

#elif !defined(TASK_LOCAL_H)
#define TASK_LOCAL_H
#pragma once

void portalTask(void *parameter)
{
  serialWrite("portalTask core ", String(xPortGetCoreID()));
  // TickType_t xLastWakeTime;
  // const TickType_t xFrequency = 10;
  // BaseType_t xWasDelayed;
  // // xLastWakeTime değişkenini mevcut zamanla başlatın.
  // xLastWakeTime = xTaskGetTickCount();
  vTaskDelay(10000);

  portalStart();

  for (;;)
  {
    // xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);

    DnsServer.processNextRequest();
    serverLoop();
    // vPortYield();
    ArduinoOTA.handle();
    vTaskDelay(100);
  }
}

void sensorTask(void *parameter)
{
  serialWrite("sensorTask core ", String(xPortGetCoreID()));
  vTaskDelay(500);
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 1000;
  BaseType_t xWasDelayed;
  // xLastWakeTime değişkenini mevcut zamanla başlatın.
  xLastWakeTime = xTaskGetTickCount();

  for (;;)
  {
    // Bir sonraki döngü için bekleyin.
    // xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);

    vTaskDelay(1000);
  }
}

void bledTask(void *parameter)
{
  for (;;)
  { // Sonsuz döngü

    if (xQueueReceive(bledQueue, &BledDurum, 1000) == pdTRUE)
    {
      if (BLedPin < 0)
      {
        serialWrite("BLedPin PinKapali_");
        continue;
      }
      else if (BledDurum == BLedHata_)
      {
        // serialWrite("BledDurum BLedHata_");
        for (uint8_t i = 0; i < 10; i++)
        {
          ledcWrite(BLedChannel_, 255);
          vTaskDelay(250);
          ledcWrite(BLedChannel_, 0);
          vTaskDelay(250);
        }
        ledcWrite(BLedChannel_, BLedSondur);
      }
      else if (BledDurum == BLedAcilis_)
      {
        // serialWrite("BledDurum BLedAcilis_");
        acilisLed();
      }
    }
    else
    {
      ledcWrite(BLedChannel_, BLedSondur);
    }
  }
}

#endif
