#include "esphome.h"

namespace esphome {

class ExampleComponent : public Component {
 public:
  std::string message_;

  static constexpr auto CONFIG_SCHEMA = \
    esphome::config_schema::make_object(
      esphome::config_schema::make_property<std::string>("message", &ExampleComponent::message_)
    );

  void setup() override {
    ESP_LOGI("ExampleComponent", "Setup complete. Message: %s", message_.c_str());
  }

  void loop() override {
    // Example: log the message every 10 seconds
    static unsigned long last_log = 0;
    if (millis() - last_log > 10000) {
      last_log = millis();
      ESP_LOGI("ExampleComponent", "Message: %s", message_.c_str());
    }
  }
};

} // namespace esphome
