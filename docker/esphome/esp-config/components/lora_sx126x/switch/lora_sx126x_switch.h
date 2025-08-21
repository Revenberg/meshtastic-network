#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
    namespace lora_sx126x {

        // This class represents a custom switch for the LoRa SX126X module.
        // It supports dynamic state evaluation, optimistic updates, and automation triggers.
        class LoRaSX126XSwitch : public switch_::Switch, public Component {
        public:
            // Constructor with configurable defaults
            LoRaSX126XSwitch(bool optimistic = false, bool assumed_state = false);

            // ESPHome lifecycle methods
            void setup() ;
            void dump_config() ;

            // Set a lambda function for dynamic state evaluation
            void set_state_lambda(std::function<optional<bool>()> &&f);

            // Get triggers for automation
            Trigger<> *get_turn_on_trigger() const;
            Trigger<> *get_turn_off_trigger() const;

            // Set optimistic mode
            void set_optimistic(bool optimistic);

            // Set assumed state
            void set_assumed_state(bool assumed_state);

            // Main loop function
            void loop() ;

            // Get setup priority
            float get_setup_priority() const ;

        protected:
            // Check if the switch has an assumed state
            bool assumed_state() ;

            // Write the state of the switch
            void write_state(bool state) ;

            // Helper method to handle triggers
            void handle_trigger(bool state);

            // Lambda function for state evaluation
            optional<std::function<optional<bool>()>> f_;

            // Flags for optimistic mode and assumed state
            bool optimistic_;
            bool assumed_state_;

            // Triggers for state changes
            Trigger<> *turn_on_trigger_;
            Trigger<> *turn_off_trigger_;
            Trigger<> *prev_trigger_;
        };

    }  // namespace lora_sx126x
}  // namespace esphome
