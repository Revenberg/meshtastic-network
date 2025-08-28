#pragma once
#include <SSD1306Wire.h>
#include <OLEDDisplayUi.h>
#include <Wire.h>

#define SDA_OLED GPIO_NUM_17 
#define SCL_OLED GPIO_NUM_18 
#define RST_OLED GPIO_NUM_21
#define VEXT GPIO_NUM_36

extern SSD1306Wire display;
extern OLEDDisplayUi ui;

void heltec_display_setup();
void heltec_display_update();