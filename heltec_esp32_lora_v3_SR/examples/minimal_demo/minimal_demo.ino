#include "heltec_display.h"
#include "heltec_webserver.h"
#include "heltec_radio.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("[INFO] minimal_demo v1.0.0");

    heltec_display_setup();
    heltec_webserver_setup();
    heltec_radio_setup();
}

void loop() {
    heltec_display_update();
    heltec_webserver_loop();
    heltec_radio_update();
}
