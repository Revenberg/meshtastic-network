#pragma once
#include <WebServer.h>

// Deze drie variabelen bestaan echt in de .cpp
extern WebServer server;
extern String ssid;
extern String inputText;
extern String TXLoRaMsg;

// Functies
void heltec_webserver_loop();
void heltec_webserver_setup();
void handleRoot();
