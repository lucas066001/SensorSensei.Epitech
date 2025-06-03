#ifndef SENSOR_COMMUNITY_SERVICE_H
#define SENSOR_COMMUNITY_SERVICE_H

#include <gateway_config.h>
#include <wifi/wifi.h>
#include <Arduino.h>

namespace SensorSensei::Services::External
{
    class SensorCommunityService
    {
    public:
        SensorCommunityService(WifiService *injectedWifiService);

        void sendDustData(float meanDustDensity25, float meanDustDensity10);
        void sendSoundData(float meanSoundLevel);

    private:
        WifiService *wifiService;
    };
}

#endif
