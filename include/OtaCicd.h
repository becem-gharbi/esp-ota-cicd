#ifndef OTA_CICD_H
#define OTA_CICD_H

#include <Arduino.h>

struct ReleaseMessage
{
    String repository;
    String version;
    String url;
};

class OtaCicd
{
public:
    bool init();
    void start(String url);
    String getVersion();

private:
    bool _setVersion(String version);
    ReleaseMessage _parseMessage(String data);
};

#endif