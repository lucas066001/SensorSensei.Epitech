#ifndef GATEWAY_CONFIG_CONSTANTS_H
#define GATEWAY_CONFIG_CONSTANTS_H

#pragma once

#include <Arduino.h>

namespace SensorSensei::Constants::GatewayConfig
{
    extern int GpsMaxRetry;                // Number of retry before passing task
    extern int RetryDelay;                 // In ms
    extern String DustSensorId;            // Retreived from sensor community
    extern String NoiseSensorId;           // Retreived from sensor community
    extern String SensorCommunityEndpoint; // In ms
}

#endif