#include "esphome.h"
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>

class WebMqttComponent : public Component {
public:
WebMqttComponent(const std::string &node_id)
: node_id_(node_id),
server_(80),
mqttClient_(wifiClient_) {}

void setup() override {
// MQTT instellingen
mqttClient_.setServer("mosquitto", 1883);
mqttClient_.setCallback([this](char* topic, byte* payload, unsigned int len){
this->on_mqtt_message(topic, payload, len);
});
this->mqtt_connect();

// HTML templates
login_html_ = "<html><body>"
"<h2>Login Vereist (" + node_id_ + ")</h2>"
"<form method='POST' action='/set_cookie'>"
" Sessie-ID: <input name='session'><br>"
" <input type='submit' value='Login'>"
"</form></body></html>";
welcome_html_ = "<html><body><h2>Welkom Node " + node_id_ + "!</h2>"
"<p id='msg'>-- geen bericht --</p>"
"</body></html>";

current_page_ = login_html_;

// Webserver endpoints
server_.on("/", HTTP_GET, [this](AsyncWebServerRequest* req){
req->send(200, "text/html", current_page_);
});

server_.on("/set_cookie", HTTP_POST, [this](AsyncWebServerRequest* req){
if (req->hasParam("session", true)) {
session_id_ = req->getParam("session", true)->value();
String payload = node_id_.c_str(); payload += ","; payload += session_id_.c_str();
this->mqtt_publish("heltec/session_check", payload.c_str());
req->send(200, "text/html", "<html><body><p>Controle sessie...</p></body></html>");
} else {
req->send(400, "text/plain", "Sessie-ID mist");
}
});

server_.begin();
}

void loop() override {
if (!mqttClient_.connected()) this->mqtt_connect();
mqttClient_.loop();
}

private:
std::string node_id_, session_id_;
std::string login_html_, welcome_html_, current_page_;
AsyncWebServer server_;
WiFiClient wifiClient_;
PubSubClient mqttClient_;

void mqtt_connect() {
if (mqttClient_.connect(node_id_.c_str())) {
std::string topic = "rpi/session_reply/" + node_id_;
mqttClient_.subscribe(topic.c_str());
}
}

void mqtt_publish(const char* topic, const char* payload) {
if (!mqttClient_.connected()) this->mqtt_connect();
mqttClient_.publish(topic, payload);
}

void on_mqtt_message(char* topic, byte* payload, unsigned int len) {
String msg = String((char*)payload).substring(0, len);
if (msg.startsWith("OK:")) {
String info = msg.substring(3);
// pas welcome_html_ aan met ontvangen tekst
welcome_html_ = "<html><body><h2>Welkom Node " +
String(node_id_.c_str()) +
"!</h2><p>" + info + "</p></body></html>";
current_page_ = welcome_html_;
} else {
current_page_ = login_html_;
}
}
};
