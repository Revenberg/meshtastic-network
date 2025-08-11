#include <Arduino.h>
#include "esphome/core/component.h"
#include "esphome/components/text_sensor.h"
#include "esphome/core/log.h"
#include "heltec_wifi_lora.h"
#include <LoRa.h>
#include <SPI.h>
#include <WiFi.h>


namespace esphome {
namespace heltec_wifi_lora {

static const char *TAG = "heltec_wifi_lora";

void heltec_wifi_lora::setup() {
  LoRa.setPins(18, 14, 26);  // NSS, RST, DIO0
  if (!LoRa.begin(868E6)) {
    ESP_LOGE("LoRa", "Starten mislukt!");
  } else {
    ESP_LOGI("LoRa", "LoRa gestart");
  }
}

void heltec_wifi_lora::loop() {
  static unsigned long last_send = 0;
  static int counter = 0;

  if (millis() - last_send > 10000) {
    last_send = millis();
    String msg = this->node_name.c_str();
    msg += " | MAC: ";
    msg += WiFi.macAddress();
    msg += " | Count: ";
    msg += String(counter++);

    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();

    ESP_LOGI("LoRa", "Verzonden: %s", msg.c_str());
  }

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    ESP_LOGI("LoRa", "Ontvangen: %s", received.c_str());
    if (this->received_msg_sensor_) {
      this->received_msg_sensor_->publish_state(received.c_str());
    }
  }
}

void heltec_wifi_lora::update() {
  // Optional: implement periodic updates if needed
}

void heltec_wifi_lora::dump_config() {
  ESP_LOGCONFIG(TAG, "heltec_wifi_lora Component");
}

} // namespace heltec_wifi_lora
} // namespace esphome



