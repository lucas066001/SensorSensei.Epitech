#ifndef DUST_SENSOR_SERVICE_H
#define DUST_SENSOR_SERVICE_H

#include <Arduino.h>

// Pin configuration
#define DUST_SENSOR_VOUT_PIN 2
#define DUST_SENSOR_LED_PIN 35

// Function declarations
void dust_sensor_setup();
void dust_sensor_loop();
float get_dust_density();

#endif // DUST_SENSOR_SERVICE_H
