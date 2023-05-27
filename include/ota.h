#ifndef OTA_H
#define OTA_H

#include <WiFi.h>
#include <HttpsOTAUpdate.h>
#include <esp_https_ota.h>
#include <esp_http_client.h>

void start_ota();
void ota_task(void *pvParameters);

#endif