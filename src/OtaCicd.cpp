#include "OtaCicd.h"

bool OtaCicd::init(String certPem)
{
    _certPem = certPem;
    return _preferences.begin("ota-cicd");
}

void OtaCicd::start(String message)
{
    ReleaseMessage releaseMessage = _parseMessage(message);

    String releaseVersion = releaseMessage.version;
    String currentVersion = getVersion();

    if (releaseVersion == currentVersion)
    {
        Serial.printf("[otaCicd] firmware up to date \n");
        return;
    }

    Serial.printf("[otaCicd] new firmware found \n");

    HttpsOTA.onHttpEvent([](HttpEvent_t *event) {});

    HttpsOTA.begin(releaseMessage.url.c_str(), _certPem.c_str());

    for (;;)
    {
        bool ota_running = true;

        switch (HttpsOTA.status())
        {
        case (HTTPS_OTA_IDLE):
            Serial.printf("[otaCicd] upgrade have not started yet \n");
            break;
        case (HTTPS_OTA_UPDATING):
            Serial.printf("[otaCicd] upgarde is in progress \n");
            break;
        case (HTTPS_OTA_SUCCESS):
            Serial.printf("[otaCicd] upgrade is successful \n");
            ota_running = false;
            _setVersion(releaseVersion);
            break;
        case (HTTPS_OTA_FAIL):
            Serial.printf("[otaCicd] upgrade failed \n");
            ota_running = false;
            break;
        case (HTTPS_OTA_ERR):
            Serial.printf("[otaCicd] error occured while creating xEventGroup() \n");
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
}

String OtaCicd::getVersion()
{
    return _preferences.getString("version", "unknown");
}

bool OtaCicd::_setVersion(String version)
{
    if (_preferences.putString("version", version) > 0)
    {
        return true;
    }
    return false;
}

ReleaseMessage OtaCicd::_parseMessage(String message)
{
    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, message);

    if (error)
    {
        Serial.printf("[otaCicd] failed to parse message \n");
    }

    ReleaseMessage releaseMessage;

    releaseMessage.repository = doc["repository"].as<String>();
    releaseMessage.url = doc["url"].as<String>();
    releaseMessage.version = doc["tag"].as<String>();

    return releaseMessage;
}