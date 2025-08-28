#include <WebServer.h> // Only needed if not already included elsewhere
extern WebServer server; // Assume server is defined elsewhere

#include <Arduino.h>
#include <RadioLib.h>
#include <WiFi.h>

// Heltec V3 LoRa pin mapping
#define LORA_CS 8
#define LORA_RST 12
#define LORA_BUSY 13
#define LORA_DIO1 14

Module loraModule(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY);
SX1262 radio(&loraModule);

#define MAX_MESSAGES 10

struct LoRaMessage {
    uint32_t id;
    String text;
    bool awaitingAck;
    unsigned long lastSend;
    int sendCount;
};

LoRaMessage messageList[MAX_MESSAGES];
int messageCount = 0;
uint32_t nextMsgId = 1;

void handleMessages() {
    String html = "<html><head><title>LoRa Message Queue</title></head><body>";
    html += "<h2>LoRa Message Queue</h2><table border='1'><tr><th>ID</th><th>Text</th><th>Awaiting Ack</th><th>Send Count</th><th>Last Send (ms)</th></tr>";
    for (int i = 0; i < messageCount; ++i) {
        html += "<tr>";
        html += "<td>" + String(messageList[i].id) + "</td>";
        html += "<td>" + messageList[i].text + "</td>";
        html += "<td>" + String(messageList[i].awaitingAck ? "true" : "false") + "</td>";
        html += "<td>" + String(messageList[i].sendCount) + "</td>";
        html += "<td>" + String(messageList[i].lastSend) + "</td>";
        html += "</tr>";
    }
    html += "</table></body></html>";
    server.send(200, "text/html", html);
}

void queue_lora_message(const String& text) {
    if (messageCount >= MAX_MESSAGES) {
        Serial.println("[QUEUE] Vol, kan geen nieuw bericht toevoegen!");
        return;
    }
    LoRaMessage msg;
    msg.id = nextMsgId++;
    msg.text = text;
    msg.awaitingAck = true;
    msg.lastSend = 0;
    msg.sendCount = 3;
    messageList[messageCount++] = msg;
    Serial.print("[QUEUE] Added message id ");
    Serial.print(msg.id);
    Serial.print(", text: ");
    Serial.println(text);
}

void send_lora_message(LoRaMessage& msg) {
    String mac = WiFi.softAPmacAddress();
    String payload = String(msg.id) + ":" + msg.text + " - " + mac;
    int state = radio.transmit(payload);
    msg.lastSend = millis();
    msg.sendCount--;
    Serial.print("["); Serial.print(millis()); Serial.print("] [SEND] id "); Serial.print(msg.id); Serial.print(", text: "); Serial.println(msg.text);
    Serial.print("[STATUS] Onbevestigde berichten: "); Serial.println(messageCount);
    if (state != RADIOLIB_ERR_NONE) {
        Serial.print("["); Serial.print(millis()); Serial.print("] [SEND] TX failed, code "); Serial.println(state);
    }
}

void process_ack(uint32_t ackId) {
    for (int i = 0; i < messageCount; ++i) {
        if (messageList[i].id == ackId) {
            Serial.print("[ACK] Received for id "); Serial.println(ackId);
            // Verwijder bericht door alles te shiften
            for (int j = i; j < messageCount - 1; ++j) {
                messageList[j] = messageList[j + 1];
            }
            messageCount--;
            Serial.print("[STATUS] Onbevestigde berichten: "); Serial.println(messageCount);
            break;
        }
    }
}


String RXLoRaMsg;
String TXLoRaMsg;

void heltec_radio_setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("[PING] Initializing LoRa...");
    int state = radio.begin(868.0, 125.0, 9, 7, 0x12);

    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("[PING] LoRa init OK!");
    }
    else
    {
        Serial.print("[PING] init failed, code: ");
        Serial.println(state);
        while (true)
            ;
    }

    // Register the /messages endpoint
    server.on("/messages", handleMessages);

}

static int sendCount = 0;
static unsigned long lastSend = 0;

void heltec_send_lora(const String &inputText)
{
    if (TXLoRaMsg != inputText)
    {
        TXLoRaMsg = inputText;
        sendCount = 3;
    }
}

void heltec_send_lora_msg(const String &inputText)
{
    String mac = WiFi.softAPmacAddress();

    int state = radio.transmit(inputText + " - " + mac);
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] [PING] Sending packet... ");
    Serial.println(inputText + " - " + mac);


    if (state == RADIOLIB_ERR_NONE)
    {
        //        Serial.println("[PING] Sent OK");
    }
    else
    {
        Serial.print("[");
        Serial.print(millis());
        Serial.print("] [PING] TX failed, code ");
        Serial.println(state);
    }
}

void heltec_receive_lora()
{
    // Wacht op antwoord
    String str;
    int state = radio.receive(str);

    if (state == RADIOLIB_ERR_NONE) {
        Serial.print("["); Serial.print(millis()); Serial.print("] [RX] "); Serial.println(str);
        // Check for ACK or normal message
        int sep = str.indexOf(":");
        if (sep > 0) {
            String idStr = str.substring(0, sep);
            String msgText = str.substring(sep + 1);
            if (msgText.startsWith("ACK")) {
                uint32_t ackId = idStr.toInt();
                process_ack(ackId);
            } else {
                // Normal message, send 3x ACK
                uint32_t msgId = idStr.toInt();
                for (int i = 0; i < 3; ++i) {
                    String ackPayload = String(msgId) + ":ACK";
                    int ackState = radio.transmit(ackPayload);
                    Serial.print("["); Serial.print(millis()); Serial.print("] [ACK] Sent for id "); Serial.println(msgId);
                }
            }
        }
        RXLoRaMsg = str;
    } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
        // No reply
    } else {
        Serial.print("["); Serial.print(millis()); Serial.print("] [RX] error, code "); Serial.println(state);
    }
}

void heltec_radio_update()
{
    heltec_receive_lora();

    // Send all messages in the list (may be out of order)
    for (int i = 0; i < messageCount; ++i) {
        LoRaMessage& msg = messageList[i];
        if (msg.awaitingAck && msg.sendCount > 0 && millis() - msg.lastSend > 200) {
            send_lora_message(msg);
        }
    }

    // Example: add keep-alive every minute
    static unsigned long lastKeepAlive = 0;
    if (millis() - lastKeepAlive > 60000) {
        lastKeepAlive = millis();
        queue_lora_message("keep-alive message");
    }

}