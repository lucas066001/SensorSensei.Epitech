#ifndef LORA_SENDER_H
#define LORA_SENDER_H

#include "LoRaWan_APP.h"
#include "Arduino.h"

// LoRa transmission configuration
#define LORA_FREQUENCY 868000000
#define LORA_BANDWIDTH 0 // 0 = 125 kHz
#define LORA_SPREADING_FACTOR 7
#define LORA_CODING_RATE 1 // 1 = 4/5
#define LORA_PREAMBLE_LENGTH 8
#define LORA_SYMBOL_TIMEOUT 0
#define LORA_FIXED_LENGTH false
#define LORA_IQ_INVERSION false
#define LORA_TRANSMISSION_TIMEOUT 3000 // ms
#define LORA_BUFFER_CAPACITY 64

// Transmission buffers
extern char lora_transmit_buffer[LORA_BUFFER_CAPACITY];
extern char lora_receive_buffer[LORA_BUFFER_CAPACITY];

// Transmission state
extern bool lora_transmitter_idle;

// Function declarations
void lora_sender_setup();
void lora_sender_loop();
void on_transmission_complete();
void on_transmission_timeout();

#endif // LORA_SENDER_H
