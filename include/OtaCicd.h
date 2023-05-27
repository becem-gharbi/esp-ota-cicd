#ifndef OTA_CICD_H
#define OTA_CICD_H

#include <Arduino.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <HttpsOTAUpdate.h>
#include <esp_https_ota.h>
#include <esp_http_client.h>

struct ReleaseMessage
{
    String repository;
    String version;
    String url;
};

class OtaCicd
{
public:
    bool init(String certPem);
    void start(String message);
    String getVersion();

private:
    String _certPem;
    Preferences _preferences;
    bool _setVersion(String version);
    ReleaseMessage _parseMessage(String message);
};

#endif