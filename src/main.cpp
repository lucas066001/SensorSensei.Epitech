#include <Arduino.h>

#include "audio_sensor_service.h"
#include "dust_sensor_service.h"
#include "lora_sender.h"
#include "sensor_common.h"

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Initialize all sensors and communication
    audio_sensor_setup();
    dust_sensor_setup();
    lora_sender_setup();

    Serial.println("All sensors and LoRa sender initialized.");
}

void loop() {
    // This function now handles both reading and sending
    lora_sender_loop();

    // Delay between consecutive readings and transmissions
    delay(SENSOR_READ_INTERVAL); // Defined in sensor_common.h
}
