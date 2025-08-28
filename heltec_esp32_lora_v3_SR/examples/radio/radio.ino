#include <RadioLib.h>

// Heltec V3 LoRa pin mapping
#define LORA_CS    8
#define LORA_RST   12
#define LORA_BUSY  13
#define LORA_DIO1  14

Module loraModule(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY);
SX1262 radio(&loraModule);

void heltec_radio_setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("[PING] Initializing LoRa...");
  int state = radio.begin(868.0, 125.0, 9, 7, 0x12);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("[PING] LoRa init OK!");
  } else {
    Serial.print("[PING] init failed, code: ");
    Serial.println(state);
    while (true);
  }
}

void heltec_radio_update() {
  static unsigned long lastPing = 0;
  if (millis() - lastPing < 3000) return;
  lastPing = millis();

  // Stuur PING
  Serial.println("[PING] Sending packet...");
  int state = radio.transmit("PING");

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("[PING] Sent OK");
  } else {
    Serial.print("[PING] TX failed, code ");
    Serial.println(state);
  }

  // Wacht op antwoord
  String str;
  state = radio.receive(str);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.print("[PING] Got reply: ");
    Serial.println(str);
  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println("[PING] No reply");
  } else {
    Serial.print("[PING] RX error, code ");
    Serial.println(state);
  }
}
