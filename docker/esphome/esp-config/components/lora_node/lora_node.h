#include "esphome.h"
#include <LoRa.h>

class LoRaNode : public esphome::Component {
 public:
  LoRaNode(const std::string &node_name) : node_name_(node_name) {}

  void setup() override {
    LoRa.setPins(18, 14, 26);  // NSS, RST, DIO0
    if (!LoRa.begin(868E6)) {
      ESP_LOGE("LoRa", "Starten mislukt!");
    } else {
      ESP_LOGI("LoRa", "LoRa gestart");
    }
  }

  void loop() override {
    static unsigned long last_send = 0;
    static int counter = 0;

    if (millis() - last_send > 10000) {
      last_send = millis();
      String msg = node_name_.c_str();
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
      id(received_msg) = received.c_str();
    }
  }

 private:
  std::string node_name_;
};
