/*
    This example provides a complete solution using the built in MQTT client
    Please make sure to set your CI/CD <https://github.com/becem-gharbi/esp-ota-cicd>
*/

#include <Arduino.h>
#include <WiFi.h>
#include "OtaCicd.h"

OtaCicd otaCicd;

const String ssid = "";          // WiFi AP ssid
const String password = "";      // WiFi AP password
const char *mqtt_uri = "";       // MQTT url <protocol>://<host>:<port>
const char *mqtt_user = "";      // MQTT username
const char *mqtt_password = "";  // MQTT password
const char *mqtt_client_id = ""; // MQTT client id
const char *releaseTopic = "";   // MQTT software release topic
const char *mqtt_cert_pem = "";  // MQTT broker's CA certificate in PEM format
const String s3CertPem = "";     // S3 public URL's CA certificate in PEM format

void setup()
{
    Serial.begin(115200);
    Serial.println();

    WiFi.begin(ssid.c_str(), password.c_str());

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected");

    esp_mqtt_client_config_t mqttConfig = {
        .uri = mqtt_uri,
        .client_id = mqtt_client_id,
        .username = mqtt_user,
        .password = mqtt_password,
        .cert_pem = mqtt_cert_pem};

    otaCicd.init(s3CertPem, releaseTopic, mqttConfig);
}

void loop()
{
}
