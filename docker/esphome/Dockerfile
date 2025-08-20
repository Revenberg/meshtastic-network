FROM esphome/esphome

# Voeg het script toe aan de container
COPY generate_nodes.sh /usr/src/app/generate_nodes.sh

CMD ["pip", "install", "--upgrade", "protobuf"]

# Maak het script uitvoerbaar
RUN chmod +x /usr/src/app/generate_nodes.sh