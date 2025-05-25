#include <Wire.h>
#include "SSD1306Wire.h"
#include "OLEDDisplay.h"
#include "images.h"
#include "fonts.h"

#define SCREEN_HEADER_HEIGHT 14

SSD1306Wire *display = nullptr;
uint8_t current_screen_line_position = SCREEN_HEADER_HEIGHT - 1;

void screen_setup() {
    display = new SSD1306Wire(0x3C, I2C_SDA, I2C_SCL);
    display->init();
    display->clear();
    display->flipScreenVertically();
    display->setFont(ArialMT_Plain_10);
    display->displayOn();
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(0, 0, "Display initialized.");
    display->display();
}

void screen_off() {
    if (!display) return;
    display->displayOff();
}

void screen_update() {
    if (!display) return;
    display->display();
}

void screen_clear() {
    if (!display) return;
    display->clear();
    current_screen_line_position = SCREEN_HEADER_HEIGHT - 1;
}

void screen_show_logo() {
    if (!display) return;

    uint8_t logo_x = (display->getWidth() - TTN_IMAGE_WIDTH) / 2;
    uint8_t logo_y = SCREEN_HEADER_HEIGHT + (display->getHeight() - SCREEN_HEADER_HEIGHT - TTN_IMAGE_HEIGHT) / 2 + 1;
    display->drawXbm(logo_x, logo_y, TTN_IMAGE_WIDTH, TTN_IMAGE_HEIGHT, TTN_IMAGE);
}

void screen_header() {
    if (!display) return;

    char status_buffer[20];

    // Message count
    snprintf(status_buffer, sizeof(status_buffer), "#%03d", ttn_get_count() % 1000);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0, 2, status_buffer);

    // Voltage or GPS time
    if (axp192_found && (millis() % 8000 < 3000)) {
        float voltage = axp.getBattVoltage() / 1000.0;
        float current_mA = axp.getBattChargeCurrent() - axp.getBattDischargeCurrent();
        snprintf(status_buffer, sizeof(status_buffer), "%.1fV %.0fmA", voltage, current_mA);
    } else {
        gps_time(status_buffer, sizeof(status_buffer));
    }

    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(display->getWidth() / 2, 2, status_buffer);

    // Satellite count
    snprintf(status_buffer, sizeof(status_buffer), "%d", gps_sats());
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(display->getWidth() - SATELLITE_IMAGE_WIDTH - 4, 2, status_buffer);
    display->drawXbm(display->getWidth() - SATELLITE_IMAGE_WIDTH, 0, SATELLITE_IMAGE_WIDTH, SATELLITE_IMAGE_HEIGHT, SATELLITE_IMAGE);
}

void screen_print(const char *text) {
    if (!display) return;

    current_screen_line_position += 10;
    if (current_screen_line_position > display->getHeight() - 10) {
        screen_clear();
        current_screen_line_position = SCREEN_HEADER_HEIGHT + 2;
    }

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0, current_screen_line_position, String(text));
    display->display();
}

void screen_print(const char *text, int x, int y) {
    if (!display) return;
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(x, y, String(text));
    display->display();
}

void screen_loop() {
    // No periodic update logic needed here, handled manually
}
