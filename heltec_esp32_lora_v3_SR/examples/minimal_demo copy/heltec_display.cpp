#include "heltec_display.h"
#include <heltec_h_sr.h>
#include "heltec_webserver.h"

SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED, GEOMETRY_128_64);
OLEDDisplayUi ui(&display);

#define VEXT GPIO_NUM_36

void frame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setFont(ArialMT_Plain_16);
    display->drawString(x, y, "LoRa RX:");
    display->drawString(x, y + 16, lastLoRaMsg);
}

void frame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setFont(ArialMT_Plain_16);
    float bat = heltec_vbat();
    display->drawString(x, y, "Batterij:");
    display->drawString(x, y + 16, String(bat) + "%");
}

void frame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setFont(ArialMT_Plain_16);
    display->drawString(x, y, "Button demo");
    // button.update(); // optioneel
    // if(button.isSingleClick()) {
    //     display->drawString(x, y + 16, "Single click!");
    // }
}

FrameCallback frames[3] = { frame1, frame2, frame3 };

void overlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
    extern String ssid;
    int bat = getBatteryPercent();
    display->setFont(ArialMT_Plain_16);
    display->drawString(0, 0, ssid);
    display->drawString(100, 0, String(bat) + "%");
}

void heltec_display_setup() {
    display.init();

    heltec_display_power(true);

    pinMode(VEXT, OUTPUT);
    digitalWrite(VEXT, LOW);  // ✅ laag = OLED krijgt stroom
    delay(10);

    display.setContrast(255);
    display.flipScreenVertically();

    display.setContrast(255);

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "Hello World!");
    display.drawString(0, 16, "Battery: " + String(heltec_battery_percent()) + "%");

    ui.setTargetFPS(30);
    ui.setFrames(frames, 3);
    ui.setOverlays(new OverlayCallback[1]{ overlay }, 1);
    ui.init();
}

void heltec_display_update() {
    int remainingTimeBudget = ui.update();
    delay(remainingTimeBudget);
    ui.switchToFrame(currentFrame);
}
