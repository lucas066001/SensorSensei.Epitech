#include "communication_service.h"
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>

// === OLED Settings ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// === LoRa Settings ===
#define LORA_BAND 868E6
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS 18
#define LORA_RST 23
#define LORA_DIO0 26

// === GPS Settings ===
#define GPS_TX_PIN 17
#define GPS_RX_PIN 16
#define GPS_BAUD_RATE 9600

// === Global Objects ===
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);  // UART2 for GPS (TX=17, RX=16)

bool loraMessageReceived = false; // Flag to control GPS parsing

void setup_communication() {
    Serial.begin(115200);
    while (!Serial);

    // === Initialize OLED ===
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED display initialization failed.");
        while (true);  // Halt if OLED fails
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Initializing...");
    display.display();

    // === Initialize LoRa ===
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

    if (!LoRa.begin(LORA_BAND)) {
        Serial.println("LoRa initialization failed.");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("LoRa Init Failed!");
        display.display();
        while (true);  // Halt if LoRa fails
    }

    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125000);
    LoRa.setCodingRate4(5);

    Serial.println("LoRa communication initialized.");

    // === Initialize GPS ===
    gpsSerial.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_TX_PIN, GPS_RX_PIN);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("LoRa + GPS Ready");
    display.display();
}

void loop_communication() {
    // === Handle LoRa Reception ===
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        String receivedText = "";
        while (LoRa.available()) {
            receivedText += (char)LoRa.read();
        }

        Serial.print("LoRa Message Received: ");
        Serial.println(receivedText);

        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.println("LoRa Message:");
        display.println(receivedText);
        display.display();

        // Mark that LoRa data was received
        loraMessageReceived = true;
    }

    // === Handle GPS Data (only if LoRa received) ===
    if (loraMessageReceived) {
        while (gpsSerial.available()) {
            char character = gpsSerial.read();
            gps.encode(character);
        }

        // Display GPS Info every 2 seconds
        static unsigned long lastGpsPrintTime = 0;
        if (millis() - lastGpsPrintTime >= 2000) {
            displayGPSInfo();
            lastGpsPrintTime = millis();
            loraMessageReceived = false; // Reset flag to wait for next LoRa message
        }
    }
}

void displayGPSInfo() {
    Serial.println("\n=== GPS Information ===");

    if (gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);

        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);

        Serial.print("Altitude: ");
        Serial.print(gps.altitude.meters());
        Serial.println(" meters");
    } else {
        Serial.println("GPS Location: Not Available");
    }

    Serial.print("Date: ");
    if (gps.date.isValid()) {
        Serial.printf("%02d/%02d/%04d\n", gps.date.month(), gps.date.day(), gps.date.year());
    } else {
        Serial.println("Date: Not Available");
    }

    Serial.print("Time: ");
    if (gps.time.isValid()) {
        Serial.printf("%02d:%02d:%02d\n", gps.time.hour(), gps.time.minute(), gps.time.second());
    } else {
        Serial.println("Time: Not Available");
    }

    Serial.println("========================\n");
}
