#include "dust_sensor_service.h"

void setup() {
  dust_sensor_setup();  // Initialize the dust sensor service
}

void loop() {
  dust_sensor_loop();   // Run the dust sensor service loop
}

