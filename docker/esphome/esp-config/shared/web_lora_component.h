#pragma once
#include "esphome.h"
#include <SPI.h>
#include <LoRa.h>

class WebLoraComponent : public esphome::Component {
 public:
  void setup() override {
    // LoRa setup (adjust pins/frequency as needed)
    LoRa.setPins(8, 14, 12); // NSS, RST, DIO0 (example for Heltec v3)
    if (!LoRa.begin(868E6)) {
      ESP_LOGE("lora", "Starting LoRa failed!");
    } else {
      ESP_LOGI("lora", "LoRa init OK");
    }
  }

  void send_number(int value) {
    ESP_LOGI("lora", "Sending number: %d", value);
    LoRa.beginPacket();
    LoRa.print(value);
    LoRa.endPacket();
  }
};
