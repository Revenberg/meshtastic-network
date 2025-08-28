#include <Wire.h>
#include "SSD1306Wire.h"

#define VEXT GPIO_NUM_36

SSD1306Wire display(0x3c, 17, 18, GEOMETRY_128_64);

void setup() {
  pinMode(VEXT, OUTPUT);
  digitalWrite(VEXT, LOW);   // OLED aan
  delay(100);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Hello World!");
  display.display();
}

void loop() {
}

// #include <heltec_h_sr.h>
// #include "heltec_webserver.h"
// #include "heltec_display.h"

/*void setup1() {
  Serial.begin(115200);
  heltec_setup();
  heltec_led(100); 
  delay(300); 
  heltec_led(0);
  // heltec_webserver_setup();
  heltec_display_setup();
}


void loop1() {
  heltec_loop();
  //server.handleClient();
  //int16_t len = radio.receive(loraBuf, sizeof(loraBuf));
  //if(len > 0){
  //  lastLoRaMsg = "";
  //  for(int i=0;i<len;i++) lastLoRaMsg += (char)loraBuf[i];
  //}
  heltec_display_update();
}
*/