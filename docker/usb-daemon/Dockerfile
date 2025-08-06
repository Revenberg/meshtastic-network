FROM python:3.11-slim
WORKDIR /app
COPY usb_rssi_mqtt.py .
RUN pip install paho-mqtt pyserial
CMD ["python", "usb_rssi_mqtt.py"]
