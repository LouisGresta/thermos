
#ifndef WIFI_MQTT_H
#define WIFI_MQTT_H

#include <Arduino.h>
#include <SPI.h>

#include <ESP8266WiFi.h> //Importe cette librairie
#include <PubSubClient.h>


/*** Les parametre de MQTT Id client et Topic****/
#define IDClient_MQTT "TestClient-" //Id client de MQTT unique

/******Les Topic du MQTT*********/
#define Topic_Hum_MQTT "/YNOV_BDX/Dh11-Hum"
#define Topic_Temp_MQTT "/YNOV_BDX/Dh11-Temp"

#define Topic_Serv_MQTT "/YNOV_BDX/servo"
#define Topic_Led_Mqtt "/YNOV_BDX/led"

#define Topic_Mod_Mqtt "/YNOV_BDX/mode"
#define Topic_Sens_Mqtt "/YNOV_BDX/sens"

const int mqtt_port = 1883;

/***********Les appelles de fonction**********/
void connectedWifi(const char* ssid, const char* password);
void setup_mqtt(void (*MQTTcallback)(char *, byte *, unsigned int));
void MQTTconnect();
void MQTTsendTemp(float Temp);
void MQTTsendHum(float Hum);
void MQTTsendPos(int pos);

#endif // WIFI_MQTT_H