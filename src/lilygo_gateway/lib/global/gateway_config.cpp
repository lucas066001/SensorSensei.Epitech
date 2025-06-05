#include <gateway_config.h>

namespace SensorSensei::Constants::GatewayConfig
{
    int GpsMaxRetry = 5;
    int RetryDelay = 1000;
    String DustSensorId = "esp32-7895456213";
    String NoiseSensorId = "esp32-31415921877";
    String SensorCommunityEndpoint = "https://api.sensor.community/v1/push-sensor-data/";
}