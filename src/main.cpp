#include <Arduino.h>
#include <WiFi.h>
// #include <mqtt.h>
#include <credentials.h>
#include <ota.h>

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

  delay(1000);

  // start_mqtt(mqtt_uri, mqtt_client_id, mqtt_user, mqtt_password, mqtt_cert_pem);

  start_ota();
}

void loop()
{
}
