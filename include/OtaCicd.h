#ifndef OTA_CICD_H
#define OTA_CICD_H

#include <Arduino.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <HttpsOTAUpdate.h>
#include <esp_https_ota.h>
#include <esp_http_client.h>
#include <mqtt_client.h>

struct ReleaseMessage
{
    String repository;
    String version;
    String url;
};

class OtaCicd
{
public:
    static bool init(String certPem);
    static bool init(String certPem, String releaseTopic, esp_mqtt_client_config_t mqttConfig);
    static void start(String message);
    static String getVersion();
    static esp_mqtt_client_handle_t mqttClient;

private:
    static bool _setVersion(String version);
    static ReleaseMessage _parseMessage(String message);
    static void _onMqttData(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
    static String _releaseTopic;
};

#endif