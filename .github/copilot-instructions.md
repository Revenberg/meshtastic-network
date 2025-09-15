# Meshtastic Network: AI Coding Agent Instructions

## Project Architecture & Major Components
- **Central Controller (Raspberry Pi 4):** Runs all management, dashboards, and OTA services in Docker containers. Key services: Node-RED (flows), Grafana (metrics), Mosquitto (MQTT), ESPHome (OTA/device mgmt), Webserver (dashboard).
- **Heltec WiFi LoRa 32 V3 (USB):** LoRa gateway for RPI4, connects via USB.
- **ESP32 Nodes:** Each runs ESPHome firmware, participates in mesh network, communicates via LoRa.
- **Game Logic:** Distributed, interactive, real-time communication between nodes and server.

## Developer Workflows
- **Setup:** Use `setup.sh` from project root for initial install and Docker stack deployment.
- **Build/Flash ESP32:** Use provided ESPHome templates; see `heltec_esp32_lora_v3_SR/boards/boards-ht_u.txt` for board config.
- **Docker Services:** Managed via `docker-compose.yml` (see `docker/` and `meshtastic-docker/`).
- **OTA Updates:** Use Node-RED flows or ESPHome web interface for device updates.
- **Dashboards:** Access via webserver (port 80), Grafana (port 3000), Node-RED (port 1880).

## Project-Specific Conventions & Patterns
- **Service Boundaries:** Each major function (MQTT, dashboards, OTA, game logic) is isolated in its own Docker container.
- **LoRa Communication:** All node-to-node and node-to-server comms use LoRa via Heltec boards; see `LoRaRadio.h/cpp` for protocol details.
- **Display/UI:** Node display logic in `heltec_display.cpp`; UI frames and overlays are modular functions.
- **Automation:** ESPHome custom components (see `docker/esphome/esp-config/components/`) use async Python for device logic.
- **Board Config:** Board-specific settings in `boards/boards-ht_u.txt`.

## Integration Points & External Dependencies
- **RadioLib, SSD1306 OLED, HotButton:** See `library.properties` for required Arduino/ESP32 libraries.
- **Docker Compose:** All RPI4 services are orchestrated via Docker Compose; see `setup.sh` and `docker-compose.yml`.
- **MQTT:** Mosquitto broker for all mesh messages; flows managed in Node-RED.
- **ESPHome:** Used for firmware, OTA, and device management.

## Examples & Patterns
- **Node Initialization:** See `radio.ino` for main setup, task creation, and interrupt handling.
- **Display Frame Pattern:** See `heltec_display.cpp` for modular frame callbacks and overlays.
- **Custom ESPHome Component:** See `docker/esphome/esp-config/components/lora_sx126x/switch/__init__.py` for async automation pattern.

## Coding Guidelines
- Follow existing code structure and naming conventions (see `radio.ino`, `heltec_display.cpp`).
- Document new features and changes inline.
- Prefer modular, reusable functions for hardware and service logic.
- When adding new services, update Docker Compose and `setup.sh`.
- For device code, ensure compatibility with board configs in `boards-ht_u.txt`.

---
For questions about unclear patterns or missing documentation, ask for feedback and iterate.
