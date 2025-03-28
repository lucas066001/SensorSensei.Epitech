#include "services/dust_sensor_service/dust_sensor_service.h"
#include "services/audio_sensor_service/audio_sensor_service.h"
#include "services/lora_service/lora_service.h"

void setup() {
  // audio_setup();
  dust_sensor_setup();
  // lora_sender_setup();
}

void loop() {
  // audio_loop();
  dust_sensor_loop();
  // lora_sender_loop();
}