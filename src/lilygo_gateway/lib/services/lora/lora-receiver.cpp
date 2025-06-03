#include <lora/lora-receiver.h>

using namespace SensorSensei::Services;

LoraReceiverService::LoraReceiverService() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
{
}

void LoraReceiverService::setup_lora()
{
    while (!Serial)
        ;

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("SSD1306 allocation failed");
        while (1)
            ;
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

    if (!LoRa.begin(BAND))
    {
        Serial.println("Starting LoRa failed!");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("LoRa Failed!");
        display.display();
        while (1)
            ;
    }

    // Ensure parameters match the sender
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
}

String LoraReceiverService::handle()
{
    String receivedText = "";

    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        Serial.print("Received: ");
        receivedText = "";
        while (LoRa.available())
        {
            char c = (char)LoRa.read();
            Serial.print(c);
            receivedText += c;
        }
        Serial.printf("Returned %s: ", receivedText.c_str());
    }

    return receivedText;
}
