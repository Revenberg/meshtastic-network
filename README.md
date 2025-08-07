# 📡 Meshtastic Network

An interactive game platform for ESP32-based mesh networks, managed by a central Raspberry Pi 4 (RPI4).

## Project Overview

This project enables a scalable, wireless mesh network using Heltec WiFi LoRa 32 V3 nodes, each equipped with ESP32 and LoRa communication. The central controller is a Raspberry Pi 4, which connects to a Heltec WiFi LoRa 32 V3 via USB for communication with the distributed nodes. Nodes are typically placed 2-3 km apart, forming a robust mesh network for interactive gaming and data exchange.

All software on the Raspberry Pi runs in isolated Docker containers for easy management, scalability, and reliability.

## Features
- **Central Control:** The RPI4 manages the network, collects data, and provides dashboards and OTA updates via Dockerized services (Node-RED, Grafana, ESPHome, etc.).
- **Mesh Networking:** Nodes communicate over LoRa, enabling long-range, low-power wireless connections.
- **Scalability:** Start with 2 nodes and expand to an unlimited number of nodes as needed.
- **Interactive Game Logic:** The system is designed for interactive, distributed games or applications, with real-time communication between nodes and the central server.

## Architecture
- **Raspberry Pi 4:** Runs all management and dashboard services in Docker containers.
- **Heltec WiFi LoRa 32 V3 (USB):** Acts as the LoRa gateway for the RPI4.
- **ESP32 Nodes:** Each node runs ESPHome firmware and participates in the mesh network.

## Getting Started
1. Flash ESPHome firmware to each node using the provided templates.
2. Connect the Heltec LoRa device to the RPI4 via USB.
3. Deploy the Docker stack on the RPI4 for dashboards, MQTT, and OTA management.
4. Monitor and manage the network via the web dashboard.

## Example Use Cases
- Outdoor games and treasure hunts
- Sensor networks with real-time monitoring
- Distributed IoT experiments

---

Voor gedetailleerde installatie-instructies, zie de onderstaande secties.

## 🔧 Installatie
```bash
curl -H "Cache-Control: no-cache" -O https://raw.githubusercontent.com/Revenberg/meshtastic-network/main/setup.sh;chmod +x setup.sh;./setup.sh
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
