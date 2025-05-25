#ifndef AUDIO_SENSOR_REPOSITORY_H
#define AUDIO_SENSOR_REPOSITORY_H

#include <Arduino.h>
#include <driver/i2s.h>

#pragma region Defines
#define I2S_WS 47
#define I2S_SD 48
#define I2S_SCK 46
#define I2S_PORT I2S_NUM_0
#define bufferLen 64
#pragma endregion

namespace SensorSensei::Repositories
{

    class AudioSensorRepository
    {
    public:
        AudioSensorRepository();

        void audioSensorSetup();
        float audioRead();

    private:
        void i2sInstall();
        void i2sSetpin();
        int16_t sBuffer[bufferLen];
    };
}

#endif