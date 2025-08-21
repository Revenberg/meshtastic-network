import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor

# Import the namespace for the LoRa SX126X component
lora_sx126x_rssi_ns = cg.esphome_ns.namespace("lora_sx126x")

# Define the LoraSX126Xpkt class, inheriting from TextSensor and Component
PktSensor = lora_sx126x_rssi_ns.class_(
    "LoraSX126Xpkt", text_sensor.TextSensor, cg.Component
)

# Define the configuration schema for the LoraSX126Xpkt text sensor
CONFIG_SCHEMA = (
    text_sensor.text_sensor_schema(
        PktSensor,
    )
    .extend(cv.COMPONENT_SCHEMA)  # Ensure the component schema is included
)

# Function to generate the C++ code for the sensor
async def to_code(config):
    # Create a new text sensor instance
    var = await text_sensor.new_text_sensor(config)

    # Register the component with ESPHome
    await cg.register_component(var, config)

    # Validate dependencies (example: ensure the LoRa SX126X driver is initialized)
    if not hasattr(cg.global_ns, "LoraSX126XDriver"):
        raise cv.Invalid("LoraSX126XDriver is not initialized. Ensure the driver is configured.")
