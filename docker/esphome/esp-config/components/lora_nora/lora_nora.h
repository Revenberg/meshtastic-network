#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include <LoRa.h>
#include <WiFi.h>

#define sendMessage_BUFFER_LEN 150

namespace esphome
{
  namespace lora_node
  {

    class lora_node : public PollingComponent, public uart::UARTDevice
    {
    public:
      void setup() override;
      void loop() override;
      void update() override;
      void dump_config() override;


    protected:
      std::string node_name = "";
      unsigned long previousMillis = 0;
      int counter = 0;
      unsigned long previousSendMillis = 0;
    };

  } // namespace lora_node
} // namespace esphome