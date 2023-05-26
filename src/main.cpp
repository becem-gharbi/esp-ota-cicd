#include <Arduino.h>
#include <WiFi.h>
#include "mqtt.h"
#include "credentials.h"
#include "ota.h"

void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  // start_mqtt(mqtt_uri, mqtt_client_id, mqtt_user, mqtt_password, mqtt_cert_pem);

  const char *otaUrl = "https://pub-f38c89bde7d24feeacb0c8fd5299a9b3.r2.dev/becem-gharbi/esp-ota-cicd/0.0.0.bin";

  start_ota(otaUrl, ota_cert_pem);
}

void loop()
{
}
