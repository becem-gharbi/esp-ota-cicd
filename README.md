# ESP OTA CI/CD

This project provides an CI/CD pipeline designed for seamless software updates to `ESP32` over the air.

## How it works

- Upon a new software release, depending on the CI/CD tool used a workflow is triggered.

- The code is compiled to generate the executable file `firmware.bin`.

- The executable is uploaded to an S3 compatible bucket.

- An MQTT message is sent the release topic on which `ESP32` is subscribed. The message contains the firmware version and the download URL.

- `ESP32` receives the message and checks the version against the running version.

- If the received firmware is newer, `ESP32` then downloads it and performs the necessary flashing process to update its firmware.


## CI/CD settings

The settings depends on the CI/CD tool used. An integration with `Github Actions` is provided.

| **Secrets**          | **Notes** |
| -------------------- | --------- |
| MQTT_HOST            |           |
| MQTT_PORT            |           |
| MQTT_SECURE          |           |
| MQTT_VERSION         |           |
| MQTT_USER            |           |
| MQTT_PASSWORD        |           |
| S3_ACCESS_KEY_ID     |           |
| S3_SECRET_ACCESS_KEY |           |
| S3_ENDPOINT_URL      |           |
| S3_REGION            |           |
| S3_PUBLIC_URL        |           |
| S3_BUCKET            |           |

| **Variables**  | **Notes** |
| -------------- | --------- |
| MQTT_PUB_TOPIC |           |
| PIO_ENV        |           |
