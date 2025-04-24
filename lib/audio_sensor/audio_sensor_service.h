#ifndef AUDIO_SENSOR_SERVICE_H
#define AUDIO_SENSOR_SERVICE_H

#include <Arduino.h>
#include <driver/i2s.h>

// I2S pin configuration
#define I2S_WS_PIN 47    // Word select (LRCLK)
#define I2S_SD_PIN 48    // Serial data in
#define I2S_SCK_PIN 46   // Serial clock
#define I2S_PORT I2S_NUM_0

#define AUDIO_BUFFER_LENGTH 64

extern int16_t audio_buffer[AUDIO_BUFFER_LENGTH]; // Audio data buffer

// Function declarations
void audio_sensor_setup();
void audio_sensor_loop();
float get_audio_mean();
void install_i2s_driver();
void configure_i2s_pins();

#endif // AUDIO_SENSOR_SERVICE_H
