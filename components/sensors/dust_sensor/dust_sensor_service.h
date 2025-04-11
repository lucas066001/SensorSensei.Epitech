#ifndef DUST_SENSOR_H
#define DUST_SENSOR_H

#include <Arduino.h>
#include <esp32-hal-gpio.h>

/** User-defined variables **/
extern int voutPin;
extern int ledPin;

/** System-defined variables **/
extern int samplingTime;
extern int deltaTime;
extern int sleepTime;
extern int voMeasured;
extern float calcVoltage;
extern float dustDensity;

/** Function prototypes **/
void dust_sensor_setup();
void dust_sensor_loop();

#endif // DUST_SENSOR_H
