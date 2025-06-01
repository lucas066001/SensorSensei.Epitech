#ifndef LORA_SENDER_H
#define LORA_SENDER_H

#include "LoRaWan_APP.h"
#include "Arduino.h"

// LoRa transmission configuration
#define RF_FREQUENCY                868000000 // Frequency in Hz
#define TX_OUTPUT_POWER             5         // Transmission power in dBm
#define LORA_BANDWIDTH              0         // [0: 125 kHz]
#define LORA_SPREADING_FACTOR       7         // [SF7..SF12]
#define LORA_CODINGRATE             1         // [1: 4/5]
#define LORA_PREAMBLE_LENGTH        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON        false
#define LORA_TRANSMISSION_TIMEOUT   3000
#define BUFFER_SIZE                 64        // Max payload size

// Transmission buffers
extern char txpacket[BUFFER_SIZE];
extern char rxpacket[BUFFER_SIZE];

// Transmission state
extern bool lora_idle;

// Function declarations
void lora_sender_setup();
void lora_sender_loop();
void send_sensor_data(float audio_level, float dust_density);
bool is_lora_ready();

// Callback function declarations
void OnTxDone(void);
void OnTxTimeout(void);

#endif // LORA_SENDER_H