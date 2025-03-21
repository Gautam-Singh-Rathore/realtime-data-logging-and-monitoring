#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "gautam";
const char* password = "12345678";
const char* mqtt_server = "192.168.137.1";
const int mqtt_port = 1883;

const char* send_topic = "send_data";
const char* receive_topic = "receive_data";

const int ledPin = D0; // D2 = GPIO4

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to Wi-Fi");
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload, length);

    if (error) {
        Serial.println("Failed to parse JSON");
        return;
    }

    String task = doc["task"];
    String id = doc["id"];

    if (id == "1") { // Change this ID based on ESP's machine ID
        if (task == "start") {
            digitalWrite(ledPin, HIGH);
            Serial.println("LED turned ON");
        } else if (task == "stop") {
            digitalWrite(ledPin, LOW);
            Serial.println("LED turned OFF");
        }
    }
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP8266Client")) {
            Serial.println("Connected");
            client.subscribe(receive_topic);
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" trying again in 5 seconds");
            delay(5000);
        }
    }
}

void sendJsonMessage() {
    StaticJsonDocument<200> doc;
    doc["id"] = "1";
    doc["message"] = "Hello!!";
    doc["date"] = "04/02/2025";
    doc["time"] = "14:30";

    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);
    client.publish(send_topic, jsonBuffer);
    Serial.print("Data published: ");
    Serial.println(jsonBuffer);
}

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) reconnect();
    client.loop();

    static unsigned long lastMsg = 0;
    if (millis() - lastMsg > 10000) {
        lastMsg = millis();
        sendJsonMessage();
    }
}
