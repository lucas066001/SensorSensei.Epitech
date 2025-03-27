#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define BAND 868E6
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DI0 26


void setup() {
    Serial.begin(115200);
    while (!Serial);

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        while (1);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("LoRa Receiver Ready");
    display.display();

    // Initialize LoRa module
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DI0);

    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("LoRa Failed!");
        display.display();
        while (1);
    }

    // Ensure parameters match the sender
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
}


 

void loop() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        Serial.print("Received: ");
        String receivedText = "";
        while (LoRa.available()) {
            char c = (char)LoRa.read();
            Serial.print(c);
            receivedText += c;
        }

        Serial.println();

        // Display received message on OLED
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Received:");
        display.println(receivedText);
        display.display();
    }
}