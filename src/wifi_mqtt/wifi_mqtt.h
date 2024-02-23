#ifndef WIFI_MQTT_H
#define WIFI_MQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE (50)

void setup_wifi();
void setup_mqtt(void (*callback)(char *, byte *, unsigned int));
void reconnect();

#endif // WIFI_MQTT_H