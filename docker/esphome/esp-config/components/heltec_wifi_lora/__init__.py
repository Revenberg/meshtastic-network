import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import *
from esphome.components import text_sensor

heltec_wifi_lora_ns = cg.esphome_ns.namespace('heltec_wifi_lora')
heltec_wifi_lora = heltec_wifi_lora_ns.class_("heltec_wifi_lora", cg.PollingComponent)

DEPENDENCIES = ['text_sensor']
AUTO_LOAD = ['sensor', 'text_sensor']

CONF_heltec_wifi_lora_NAME = "name"
received_msg_sensor = "received_msg_sensor"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(heltec_wifi_lora),    
    cv.Required(CONF_heltec_wifi_lora_NAME): cv.string,

    cv.Optional(received_msg_sensor): cv.use_id(text_sensor.TextSensor),
    cv.Optional(CONF_ID): cv.declare_id(heltec_wifi_lora),
    cv.Optional(CONF_heltec_wifi_lora_NAME): cv.string,

}).extend(cv.polling_component_schema('60s'))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    if CONF_heltec_wifi_lora_NAME in config:
        conf = config[CONF_heltec_wifi_lora_NAME]
   #     sens = yield sensor.new_sensor(conf)
   #     cg.add(var.set_heltec_wifi_lora_name_sensor(sens))
        # cg.add(var.set_internal(sens))
