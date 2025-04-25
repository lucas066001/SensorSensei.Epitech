#ifndef COMMUNICATION_SERVICE_H
#define COMMUNICATION_SERVICE_H

#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// === OLED ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// === LORA ===
#define BAND 868E6
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DI0 26

// === GPS ===
#define GPS_BAUD 9600
#define RX_PIN 12
#define TX_PIN 34

// === Globals (defined in .cpp) ===
extern Adafruit_SSD1306 display;
extern TinyGPSPlus gps;
extern HardwareSerial gpsSerial;

// === Function Declarations ===
void setup_communication();
void loop_communication();
void displayGPSInfo();

#endif
