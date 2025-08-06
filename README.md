# 📡 Meshtastic Network

Volledige IoT-stack voor ESP32 mesh-netwerken met OTA, MQTT, dashboards en meer.

## 🔧 Installatie
```bash
curl -O https://raw.githubusercontent.com/Revenberg/meshtastic-network/main/setup.sh;chmod +x setup.sh;./setup.sh
```

🌐 Services

| Service   | Poort | Beschrijving           |
|-----------|-------|-----------------------|
| Node-RED  | 1880  | Flow-based visualisatie |
| Grafana   | 3000  | Metrics & dashboards   |
| Mosquitto | 1883  | MQTT broker            |
| Webserver | 80    | Link-dashboard         |
| ESPHome   | 6052  | OTA & device management|

📊 Dashboards

• Signal Strength  
• Battery Status  
• Node Metrics

🔄 OTA Updates

Via Node-RED flows (flows.json) of ESPHome webinterface.
