import paho.mqtt.client as mqtt
def on_message(client, userdata, msg):
    print(f"[{msg.topic}] {msg.payload.decode()}")
client = mqtt.Client()
client.on_message = on_message
client.connect("mosquitto", 1883)
client.subscribe("meshtastic/#")
client.loop_forever()
