#include "lora_sender.h"
#include "audio_sensor_service.h"
#include "dust_sensor_service.h"

// Variables (definitions for extern declarations in header)
char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];
bool lora_idle = true;
static RadioEvents_t RadioEvents;

void lora_sender_setup() {
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetTxConfig(
        MODEM_LORA,
        TX_OUTPUT_POWER,
        0,
        LORA_BANDWIDTH,
        LORA_SPREADING_FACTOR,
        LORA_CODINGRATE,
        LORA_PREAMBLE_LENGTH,
        LORA_FIX_LENGTH_PAYLOAD_ON,
        true,
        0,
        0,
        LORA_IQ_INVERSION_ON,
        LORA_TRANSMISSION_TIMEOUT
    );
    Serial.println("LoRa sender module initialized.");
}

void lora_sender_loop() {
    if (!lora_idle) {
        Radio.IrqProcess();
        return;
    }
    
    // Read sensor data
    audio_sensor_loop();
    dust_sensor_loop();
    
    float mean_audio_value = get_audio_mean();
    float current_dust_density = get_dust_density();
    
    // Send the data
    send_sensor_data(mean_audio_value, current_dust_density);
}

void send_sensor_data(float audio_level, float dust_density) {
    if (!lora_idle) {
        return; // Still transmitting
    }
    
    snprintf(txpacket, BUFFER_SIZE, "Audio=%.2f, Dust=%.2f", audio_level, dust_density);
    Serial.printf("Sending LoRa packet: \"%s\" (length: %d)\n", txpacket, strlen(txpacket));
    
    Radio.Send((uint8_t *)txpacket, strlen(txpacket));
    lora_idle = false;
}

bool is_lora_ready() {
    return lora_idle;
}

// LoRa callback functions
void OnTxDone(void) {
    Serial.println("LoRa TX done...");
    lora_idle = true;
}

void OnTxTimeout(void) {
    Serial.println("LoRa TX Timeout...");
    Radio.Sleep();
    lora_idle = true;
}