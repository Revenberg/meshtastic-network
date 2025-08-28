#pragma once

// Add any required includes
#include <Arduino.h>

// LoRa pin definitions
#define LORA_DIO1_PIN   14
#define LORA_RST_PIN    12
#define LORA_BUSY_PIN   13

// Externs for shared variables
extern String RXLoRaMsg;
extern String TXLoRaMsg;
extern uint8_t loraBuf[256];

void heltec_radio_setup();
void heltec_radio_update();
void heltec_receive_lora();
void heltec_send_lora(const String& inputText);