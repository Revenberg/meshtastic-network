/**
 * @file heltec_unofficial.h
 * @brief Header file for the Heltec library.
 *
 * This file contains the definitions and declarations for the Heltec library.
 * The library provides functions for controlling the Heltec ESP32 LoRa V3
 * board, including LED brightness control, voltage measurement, deep sleep
 * mode, and more.
 */

#pragma once

#include "RadioLib.h"
#include "HotButton.h"

#define BUTTON GPIO_NUM_0 
#define LED_PIN GPIO_NUM_35 
#define LED_FREQ 5000 
#define LED_CHAN 0 
#define LED_RES 8 // External power control 
#define VEXT GPIO_NUM_36 // Battery voltage measurement 
#define VBAT_CTRL GPIO_NUM_37 
#define VBAT_ADC GPIO_NUM_1 // SPI pins 
#define SS GPIO_NUM_8 
#define MOSI GPIO_NUM_10 
#define MISO GPIO_NUM_11 
#define SCK GPIO_NUM_9 // Radio pins 
#define DIO1 GPIO_NUM_14 
#define RST_LoRa GPIO_NUM_12 
#define BUSY_LoRa GPIO_NUM_13 // Display pins 

// Variable declarations
extern HotButton button;
extern Module loraModule;
extern SX1262 radio;
extern volatile int currentFrame;

extern uint8_t loraBuf[128];
extern String lastLoRaMsg;

// Function declarations
void heltec_led(int percent);
void heltec_send_lora(const String& inputText);
void heltec_ve(bool state);
float heltec_vbat();
int heltec_battery_percent(float vbat = -1);
bool heltec_wakeup_was_button();
bool heltec_wakeup_was_timer();
float heltec_temperature();
void heltec_deep_sleep(int seconds = 0);
void heltec_display_power(bool on);
void heltec_setup();
void heltec_loop();
void heltec_delay(int ms);
