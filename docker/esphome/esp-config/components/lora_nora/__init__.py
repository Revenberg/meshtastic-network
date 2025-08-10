import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart, binary_sensor
from esphome.const import *

floorheating_ns = cg.esphome_ns.namespace('lora_node')
lora_node = floorheating_ns.class_("lora_node", cg.PollingComponent)

DEPENDENCIES = ['uart']
AUTO_LOAD = ['uart', 'sensor', "binary_sensor" ]

CONF_LORA_NODE_NAME = "LORA_NODE_1"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(lora_node),    
    cv.Required(CONF_LORA_NODE_NAME): cv.string,
}).extend(cv.polling_component_schema('60s')).extend(uart.UART_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

    if CONF_LORA_NODE_NAME in config:
        conf = config[CONF_LORA_NODE_NAME]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_lora_node_name_sensor(sens))
        # cg.add(var.set_internal(sens))
