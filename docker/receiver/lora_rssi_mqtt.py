import os
import serial
import paho.mqtt.client as mqtt

tty = os.environ.get("TTY_DEVICE", "/dev/ttyUSB0")
ser = serial.Serial(tty, 9600)
mqttc = mqtt.Client()
mqttc.connect("mosquitto", 1883, 60)

while True:
    line = ser.readline().decode(errors="replace").strip()
    if line:
        mqttc.publish("meshtastic/rssi", line)       
        import os
        import serial
        import paho.mqtt.client as mqtt
        
        tty = os.environ.get("TTY_DEVICE", "/dev/ttyUSB0")
        ser = serial.Serial(tty, 9600)
        mqttc = mqtt.Client()
        mqttc.connect("mosquitto", 1883, 60)
        
        while True:
            line = ser.readline().decode(errors="replace").strip()
            if line:
                mqttc.publish("meshtastic/rssi", line)

                import serial, paho.mqtt.client as mqtt
                ser = serial.Serial('/dev/ttyUSB0', 9600)
                mqttc = mqtt.Client()
                mqttc.connect("mosquitto", 1883, 60)
                while True:
                    line = ser.readline().decode().strip()
                    mqttc.publish("meshtastic/rssi", line)
