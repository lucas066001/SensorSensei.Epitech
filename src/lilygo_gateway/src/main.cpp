#include "services/gps_service/gps_service.h"
#include "services/lora_service/lora_service.h"

void setup(){
  // setup_lora();
  setup_gps();
}

void loop(){
  // loop_lora();
  loop_gps();
}