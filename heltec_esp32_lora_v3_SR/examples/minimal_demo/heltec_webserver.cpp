#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "heltec_radio.h"

WebServer server(80);
DNSServer dnsServer;

String ssid;
const char* password = "12345678";

// extern String TXLoRaMsg; // Defined in heltec_radio.cpp

// ====== HTML Pagina ======
// Extern from radio module
extern String RXLoRaMsg;

String makePage() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body { font-family: Arial; margin: 20px; }";
  html += "input[type=text]{ width: 80%; padding: 8px; }";
  html += "input[type=submit]{ padding: 8px 16px; }";
  html += "div.box{ margin-top:20px; padding:10px; border:1px solid #333; border-radius:8px;}";
  html += "</style></head><body>";

  html += "<h2>Heltec ESP32 Webserver</h2>";
  html += "<form action='/' method='POST'>";
  html += "<input type='text' name='msg' placeholder='Typ bericht...'>";
  html += "<input type='submit' value='Verzend'>";
  html += "</form>";

  html += "<div class='box'><b>Laatste bericht:</b><br>" + TXLoRaMsg + "</div>";
  html += "<div class='box'><b>Laatste LoRa RX:</b><br>" + RXLoRaMsg + "</div>";

  html += "<h3>LoRa Message Queue</h3>";
  html += "<iframe src='/messages' width='100%' height='300' style='border:1px solid #333;'></iframe>";

  html += "</body></html>";
  return html;
}

// ====== Handlers ======
void handleRoot() {
  Serial.println("[INFO] Client opende root pagina");
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handlePost() {
  if (server.hasArg("msg")) {
    String message = server.arg("msg");
    heltec_send_lora(message);
    Serial.println("Web->ESP: " + message);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleCaptivePortal() {
  server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString() + "/", true);
  server.send(302, "text/plain", "");
}

void heltec_webserver_setup() {
  Serial.begin(115200);
  Serial.println("[INFO] Start WiFi AP...");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192,168,4,1),
                    IPAddress(192,168,4,1),
                    IPAddress(255,255,255,0));
  // Get MAC address and build SSID
  String mac = WiFi.softAPmacAddress();
  mac.replace(":", "");
  ssid = "Heltec_AP_" + mac;
  WiFi.softAP(ssid.c_str(), password);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("[INFO] AP SSID: ");
  Serial.println(ssid);
  Serial.print("[INFO] AP IP adres: ");
  Serial.println(ip);

  // DNS server → captive portal
  dnsServer.start(53, "*", ip);

  // Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/", HTTP_POST, handlePost);

  // Captive portal detectie (Android, iOS, Windows)
  server.on("/generate_204", handleCaptivePortal);
  server.on("/fwlink", handleCaptivePortal);
  server.on("/hotspot-detect.html", handleCaptivePortal);
  server.onNotFound(handleCaptivePortal);

  server.begin();
  Serial.println("[INFO] Webserver gestart.");
}

void heltec_webserver_loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
