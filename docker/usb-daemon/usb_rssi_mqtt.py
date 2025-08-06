import serial, paho.mqtt.client as mqtt
ser = serial.Serial('/dev/ttyUSB1', 9600)
mqttc = mqtt.Client()
mqttc.connect("mosquitto", 1883, 60)
while True:
    line = ser.readline().decode().strip()
    mqttc.publish("meshtastic/usb", line)
