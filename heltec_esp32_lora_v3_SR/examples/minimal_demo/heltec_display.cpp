#include "heltec_display.h"
#include "heltec_battery.h"
#include <WiFi.h>
#include <Wire.h>

SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED, GEOMETRY_128_64);
OLEDDisplayUi ui(&display);

// Example frame callbacks
void frame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setFont(ArialMT_Plain_16);
    display->drawString(x, y, "Frame 1: ");
}

void frame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setFont(ArialMT_Plain_16);
    int percent = heltec_battery_percent();
    display->drawString(x, y, "Battery: " + String(percent) + "%");
}

void frame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setFont(ArialMT_Plain_16);
    display->drawString(x, y, "Frame 3: ");
}

void frame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setFont(ArialMT_Plain_16);
    String mac = WiFi.softAPmacAddress();
    display->drawString(x, y, mac);
}

// Externs for LoRa messages
#include "heltec_radio.h"
extern String RXLoRaMsg;
extern String TXLoRaMsg;

FrameCallback frames[4] = { frame1, frame2, frame3, frame4 };

void overlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
    display->setFont(ArialMT_Plain_16);
    display->drawString(0, 16, "RX: " + RXLoRaMsg);
    display->drawString(0, 32, "TX: " + TXLoRaMsg);
}

void heltec_display_setup() {
    battery_init();
    pinMode(VEXT, OUTPUT);
    digitalWrite(VEXT, LOW);
    delay(200);
    pinMode(RST_OLED, OUTPUT);
    digitalWrite(RST_OLED, LOW);
    delay(50);
    digitalWrite(RST_OLED, HIGH);
    delay(50);
    Wire.begin(SDA_OLED, SCL_OLED);
    display.init();
    display.clear();
    display.setContrast(255);
    //display.flipScreenVertically();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    ui.setTargetFPS(30);
    ui.setFrames(frames, 4);
    ui.setOverlays(new OverlayCallback[1]{ overlay }, 1);
    ui.init();
}

void heltec_display_update() {
    display.clear();
    int remainingTimeBudget = ui.update();
    delay(remainingTimeBudget);
}