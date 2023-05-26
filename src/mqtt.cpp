#include "mqtt.h"

bool start_mqtt(const char *uri, const char *clientId, const char *user, const char *password, const char *cert_pem)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri = uri,
        .client_id = clientId,
        .username = user,
        .password = password,
        .cert_pem = cert_pem};

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    if (client == NULL)
    {
        Serial.printf("[mqtt] unable to create MQTT client \n");
        return false;
    }

    esp_err_t err = esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, client);
    if (err != ESP_OK)
    {
        Serial.printf("[mqtt] unable to register event handler \n");
        return false;
    }
    err = esp_mqtt_client_start(client);

    if (err != ESP_OK)
    {
        Serial.printf("[mqtt] unable to start MQTT client \n");
        return false;
    }
    return true;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = *((esp_mqtt_event_handle_t *)(&event_data));
    esp_mqtt_client_handle_t client = event->client;

    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
    {
        Serial.printf("[mqtt] client connected \n");
        esp_mqtt_client_subscribe(client, "test1", 1);
        break;
    }
    case MQTT_EVENT_DISCONNECTED:
        Serial.printf("[mqtt] client disconnected \n");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        Serial.printf("[mqtt] client subscribed \n");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        break;
    case MQTT_EVENT_PUBLISHED:
        break;
    case MQTT_EVENT_ERROR:
        Serial.printf("[mqtt] error occured \n");
        break;
    case MQTT_EVENT_DATA:
        Serial.printf("[mqtt] data received from topic %s \n", event->topic);
        Serial.printf("[mqtt] data is %s \n", event->data);
        break;
    default:
        break;
    }
}