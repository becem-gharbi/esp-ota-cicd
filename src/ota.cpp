#include "ota.h"

char *gUrl;
char *gCertPerm;

void ota_task(void *pvParameters)
{
    HttpsOTA.onHttpEvent([](HttpEvent_t *event) {});
    HttpsOTA.begin(gUrl, gCertPerm);

    for (;;)
    {
        bool ota_running = true;

        switch (HttpsOTA.status())
        {
        case (HTTPS_OTA_IDLE):
            Serial.printf("[ota] upgrade have not started yet");
            break;
        case (HTTPS_OTA_UPDATING):
            Serial.printf("[ota] upgarde is in progress");
            break;
        case (HTTPS_OTA_SUCCESS):
            Serial.printf("[ota] upgrade is successful");
            ota_running = false;
            break;
        case (HTTPS_OTA_FAIL):
            Serial.printf("[ota] upgrade failed");
            ota_running = false;
            break;
        case (HTTPS_OTA_ERR):
            Serial.printf("[ota] error occured while creating xEventGroup()");
            ota_running = false;
            break;
        default:
            break;
        }

        if (!ota_running)
        {
            break;
        }

        delay(500);
    }

    vTaskDelete(NULL);
}

void start_ota(const char *url, const char *certPem)
{
    strcpy(gUrl, url);
    strcpy(gCertPerm, certPem);
    xTaskCreatePinnedToCore(ota_task, "ota_task", 4096, NULL, 6, NULL, 1);
}