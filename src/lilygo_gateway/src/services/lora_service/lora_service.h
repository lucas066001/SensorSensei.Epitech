#ifndef LORA_RECEIVER_H
#define LORA_RECEIVER_H

#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

extern Adafruit_SSD1306 display; 

#define BAND 868E6
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DI0 26

void setup_lora();
void loop_lora();

#endif