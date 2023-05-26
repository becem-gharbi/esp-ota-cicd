#ifndef MQTT
#define MQTT

#include <Arduino.h>
#include "mqtt_client.h"

bool start_mqtt(const char *uri, const char *clientId, const char *user, const char *password, const char *cert_pem);
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

#endif