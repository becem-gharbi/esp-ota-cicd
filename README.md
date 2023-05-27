# ESP OTA CI/CD

This project provides an CI/CD pipeline designed for seamless remote software updates to `ESP32`. Plus, a PlatformIO library to automatically handle updates. 

## How it works

- Upon a new software release, depending on the CI/CD tool used, a workflow is triggered.

- The code is compiled to generate the executable file `firmware.bin`.

- The executable is uploaded to an S3 compatible bucket.

- An MQTT message is sent the release topic on which `ESP32` is subscribed. The message contains the firmware version and the download URL.

- The library receives the message and checks the version against the running version.

- If the received firmware is newer, then the library downloads it and performs the necessary flashing process.

## CI/CD settings

The settings depends on the CI/CD tool used. An integration with `Github Actions` is provided via `.github/workflows/release.yml`.
The variables and secrets **should** be set as described in [docs](https://docs.github.com/en/actions/learn-github-actions/variables).

### Secrets

| **Secrets**          | **Notes**                                                                          |
| -------------------- | ---------------------------------------------------------------------------------- |
| MQTT_HOST            | The MQTT host, eg `broker.hivemq.com`                                              |
| MQTT_PORT            | The MQTT TCP port, eg `1883`                                                       |
| MQTT_SECURE          | Whether a default SSL configuration is used, can be set to `false` or ` ` for true |
| MQTT_VERSION         | The MQTT version can be set to `3` or `5`                                          |
| MQTT_USER            | The username for MQTT authentication                                               |
| MQTT_PASSWORD        | The password for MQTT authentication                                               |
| S3_ACCESS_KEY_ID     | The S3 client ID                                                                   |
| S3_SECRET_ACCESS_KEY | The S3 client secret                                                               |
| S3_ENDPOINT_URL      | The S3 endpoint, check provider                                                    |
| S3_REGION            | The S3 region, check provider                                                      |
| S3_BUCKET            | The S3 bucket used, should be public                                               |
| S3_PUBLIC_URL        | The public URL of the provided bucket                                              |

### Variables

| **Variables**  | **Notes**                                                     |
| -------------- | ------------------------------------------------------------- |
| MQTT_PUB_TOPIC | The MQTT topic to which the release message will be published |
| PIO_ENV        | Environment to build, check `platformio.ini`                  |

