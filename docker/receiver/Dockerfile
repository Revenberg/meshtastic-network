FROM python:3.11-slim
WORKDIR /app
COPY lora_rssi_mqtt.py .
RUN pip install paho-mqtt pyserial
CMD ["python", "lora_rssi_mqtt.py"]
