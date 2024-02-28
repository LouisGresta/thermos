#ifndef WIFI_MQTT_H
#define WIFI_MQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE (100)

extern PubSubClient client;
extern const char *topicOut;
extern const char *topicIn;

// WiFi credentials
const char *ssid = "Projet_IoT";
const char *password = "MdpTresSafe01";

// Mqtt connection
const char *mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char *topicIn = "/YNOV_BDX/Capteurs/";  // celui que l'on reçoit
const char *topicOut = "/YNOV_BDX/Theo/TestUnit"; // celui que l'on envoie

void setup_wifi();
void setup_mqtt(void (*callback)(char *, byte *, unsigned int));
void reconnect();

#endif // WIFI_MQTT_H