#pragma once

#include <Arduino.h>
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "heltec_wifi_lora.h"
#include <LoRa.h>
#include <SPI.h>
#include <WiFi.h>


#define sendMessage_BUFFER_LEN 150

namespace esphome
{
  namespace heltec_wifi_lora
  {

    class heltec_wifi_lora : public PollingComponent 
    {
    public:
      void setup() override;
      void loop() override;
      void update() override;
      void dump_config() override;
      void set_node_name(const std::string &name) { node_name = name; }


    protected:
      std::string node_name = "";
      unsigned long previousMillis = 0;
      int counter = 0;
      unsigned long previousSendMillis = 0;
    };

  } // namespace heltec_wifi_lora
} // namespace esphome