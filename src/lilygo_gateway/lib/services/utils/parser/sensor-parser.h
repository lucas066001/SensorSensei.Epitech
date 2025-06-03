#ifndef SENSOR_PARSER_SERVICE_H
#define SENSOR_PARSER_SERVICE_H

#include <Arduino.h>
#include <tuple>

namespace SensorSensei::Services::Utils
{
    class SensorParserService
    {
    public:
        std::tuple<float, float, float> extractSensorData(const String &input);
    };
}

#endif
