#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>
#include <driver/i2s.h>

#define I2S_WS 47
#define I2S_SD 48
#define I2S_SCK 46
#define I2S_PORT I2S_NUM_0
#define bufferLen 64

extern int16_t sBuffer[bufferLen]; // Declared here, defined in audio.cpp

void audio_setup();
void audio_loop();
void i2s_install();
void i2s_setpin();

#endif // AUDIO_H
