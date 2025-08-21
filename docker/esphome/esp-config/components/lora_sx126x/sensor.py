# This file defines the configuration schema and code generation logic
# for the LoraSX126Xrssi sensor component in ESPHome.

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    STATE_CLASS_MEASUREMENT,
)

# Define the namespace for the LoraSX126Xrssi component
lora_sx126x_rssi_ns = cg.esphome_ns.namespace("lora_sx126x")

# Define the RSSI sensor class
RSSISensor = lora_sx126x_rssi_ns.class_(
    "LoraSX126Xrssi", sensor.Sensor, cg.Component
)

# Define the configuration schema for the RSSI sensor
CONFIG_SCHEMA = (
    sensor.sensor_schema(
        RSSISensor,
        accuracy_decimals=1,  # Default accuracy decimals
        state_class=STATE_CLASS_MEASUREMENT,  # State class for measurement
    )
)

# Generate the code for the RSSI sensor
async def to_code(config):
    # Create a new sensor instance
    var = await sensor.new_sensor(config)
    # Register the sensor as a component
    await cg.register_component(var, config)
