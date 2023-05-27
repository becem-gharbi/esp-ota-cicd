#include "OtaCicd.h"

String OtaCicd::_certPem;
String OtaCicd::_releaseTopic;
Preferences OtaCicd::_preferences;
esp_mqtt_client_handle_t OtaCicd::mqttClient;

bool OtaCicd::init(String certPem)
{
    if (_preferences.begin("ota-cicd"))
    {
        String currentVersion = getVersion();
        Serial.printf("[ota-cicd] detected version %s \n", currentVersion);

        _certPem = certPem;
        return true;
    }

    return false;
}

bool OtaCicd::init(String certPem, String releaseTopic, esp_mqtt_client_config_t mqttConfig)
{
    mqttClient = esp_mqtt_client_init(&mqttConfig);

    if (mqttClient == NULL)
    {
        Serial.printf("[ota-cicd] unable to create MQTT client \n");
        return false;
    }

    esp_err_t err = esp_mqtt_client_register_event(mqttClient, MQTT_EVENT_ANY, _onMqttData, mqttClient);

    if (err != ESP_OK)
    {
        Serial.printf("[ota-cicd] unable to register event handler \n");
        return false;
    }

    err = esp_mqtt_client_start(mqttClient);

    if (err != ESP_OK)
    {
        Serial.printf("[ota-cicd] unable to start MQTT client \n");
        return false;
    }

    _releaseTopic = releaseTopic;

    return init(certPem);
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

    Serial.printf("[otaCicd] new version found %s \n", releaseVersion);

    HttpsOTA.onHttpEvent([](HttpEvent_t *event) {});

    HttpsOTA.begin(releaseMessage.url.c_str(), _certPem.c_str());

    bool otaRunning = true;
    bool updateSuccess = false;

    while (otaRunning)
    {
        switch (HttpsOTA.status())
        {
        case HTTPS_OTA_IDLE:
            Serial.printf("[otaCicd] update have not started yet \n");
            break;

        case HTTPS_OTA_UPDATING:
            Serial.printf("[otaCicd] update in progress \n");
            break;

        case HTTPS_OTA_SUCCESS:
            Serial.printf("[otaCicd] update is successful \n");
            otaRunning = false;
            updateSuccess = true;
            break;

        case HTTPS_OTA_FAIL:
            Serial.printf("[otaCicd] update failed \n");
            otaRunning = false;
            break;

        case HTTPS_OTA_ERR:
            Serial.printf("[otaCicd] error occured while creating xEventGroup() \n");
            otaRunning = false;
            break;
        }

        delay(1000);
    }

    if (updateSuccess)
    {
        _setVersion(releaseVersion);
        ESP.restart();
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

void OtaCicd::_onMqttData(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = *((esp_mqtt_event_handle_t *)(&event_data));
    esp_mqtt_client_handle_t client = event->client;

    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
        Serial.printf("[ota-cicd] mqtt client connected \n");
        esp_mqtt_client_subscribe(client, _releaseTopic.c_str(), 2);
        break;

    case MQTT_EVENT_DISCONNECTED:
        Serial.printf("[ota-cicd] mqtt client disconnected \n");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        Serial.printf("[ota-cicd] mqtt client subscribed to release topic\n");
        break;

    case MQTT_EVENT_DATA:
        if (strcmp(event->topic, _releaseTopic.c_str()) == 0)
        {
            start(event->data);
        }
        break;
    }
}
