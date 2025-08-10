#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/spi/spi.h"
#include "esphome/core/log.h"
#include "heltec_wifi_lora.h"
#include <ArduinoJson.h>
#include <LoRa.h>

namespace esphome
{
  namespace heltec_wifi_lora
  {

    static const char *TAG = "heltec_wifi_lora";

    void heltec_wifi_lora::setup()
    {
      previousMillis = 0;
      previousSendMillis = 0;
    }

    void heltec_wifi_lora::loop()
    {

    }

    void heltec_wifi_lora::update()
    {
    }

    void heltec_wifi_lora::dump_config()
    {
      ESP_LOGCONFIG(TAG, "heltec_wifi_lora Component");
    }

  } // namespace heltec_wifi_lora
} // namespace esphome



class LoRaNode : public Component {
 public:
  std::string node_name_;

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


};

} // namespace esphome
