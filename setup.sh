#!/bin/bash
set -e
sudo apt update && sudo apt install -y git curl python3-pip python3-venv ca-certificates gnupg lsb-release
curl -fsSL https://get.docker.com | sudo bash
sudo usermod -aG docker $USER
sudo pip3 install docker-compose

git clone https://github.com/<jouw-gebruikersnaam>/meshtastic-network.git
cd meshtastic-network

TTY_DEV=$(ls /dev/ttyUSB* | head -n 1 || echo "/dev/ttyUSB0")
echo "TTY_DEVICE=$TTY_DEV" > .env

mkdir -p docker/webserver
cat <<EOF2 > docker/webserver/index.html
<html><body><h2>📡 Services</h2><ul>
<li><a href="http://localhost:1880">Node-RED</a></li>
<li><a href="http://localhost:3000">Grafana</a></li>
<li><a href="http://localhost:6052">ESPHome</a></li>
</ul></body></html>
EOF2

cd docker && docker-compose up -d --build
echo "✅ Installatie voltooid"
