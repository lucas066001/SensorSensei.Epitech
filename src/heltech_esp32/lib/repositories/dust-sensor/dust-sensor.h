#ifndef DUST_SENSOR_REPOSITORY_H
#define DUST_SENSOR_REPOSITORY_H

#include <Arduino.h>
#include <esp32-hal-gpio.h>
#include <tuple>

namespace SensorSensei::Repositories
{

    class DustSensorRepository
    {
    public:
        DustSensorRepository();

        void dustSensorSetup();
        std::tuple<float, float> dustRead();

    private:
        /** User-defined variables **/
        int voutPin;
        int ledPin;

        /** System-defined variables **/
        int samplingTime;
        int deltaTime;
        int sleepTime;
        int voMeasured;
        float calcVoltage;
        float dustDensity;
    };
}

#endif
