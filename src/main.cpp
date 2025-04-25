#include <Arduino.h>
#include "lora_receiver.h"

void setup(){
  setup_lora();
  //setup_gps();
}

void loop(){
  loop_lora();
  //loop_gps();
}