#include <external/sensor-community/sensor-community.h>
#include "sensor-community.h"

using namespace SensorSensei::Services::External;

SensorCommunityService::SensorCommunityService(WifiService *injectedWifiService) : wifiService(injectedWifiService)
{
}

void SensorCommunityService::sendDustData(float meanDustDensity25, float meanDustDensity10)
{
    String body = "{\"sensordatavalues\":[{\"value_type\":\"P2\",\"value\":\"" + String(meanDustDensity25, 2) + "\"},{\"value_type\":\"P1\",\"value\":\"" + String(meanDustDensity10, 2) + "\"}]}";
    Serial.printf("Sending dust data: %s\n", body.c_str());

    std::map<String, String> headers = {
        {"Content-Type", "application/json"},
        {"X-PIN", "1"},
        {"X-Sensor", Constants::GatewayConfig::DustSensorId}};

    wifiService->sendPostRequest(Constants::GatewayConfig::SensorCommunityEndpoint, headers, body);
}

void SensorCommunityService::sendSoundData(float meanSoundLevel)
{
    String body = "{\"sensordatavalues\":[{\"value_type\":\"noise\",\"value\":\"" + String(meanSoundLevel, 2) + "\"}]}";
    Serial.printf("Sending sound data: %s\n", body.c_str());

    std::map<String, String> headers = {
        {"Content-Type", "application/json"},
        {"X-PIN", "11"},
        {"X-Sensor", Constants::GatewayConfig::NoiseSensorId}};

    wifiService->sendPostRequest(Constants::GatewayConfig::SensorCommunityEndpoint, headers, body);
}