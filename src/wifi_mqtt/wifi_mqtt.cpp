#include "wifi_mqtt.h"

// WiFi credentials
const char *ssid = "Projet_IoT";
const char *password = "MdpTresSafe01";

// Mqtt connection
const char *mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char *topicIn = "/YNOV_BDX/Projet_IoT/#";  // celui que l'on reçoit
const char *topicOut = "/YNOV_BDX/Theo/TestUnit"; // celui que l'on envoie

char *msgToSend = "test"; // message à envoyer => modifiable dans le code
char *topicReaded = "none";
char *msgReaded = "none";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup_mqtt(void (*callback)(char *, byte *, unsigned int))
{
    randomSeed(micros());
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish(topicOut, "hello world");
            // ... and resubscribe
            client.subscribe(topicIn);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}