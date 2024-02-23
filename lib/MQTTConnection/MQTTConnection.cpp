#include "MQTTConnection.h"
#include "LCDDisplay.h"
#include "DHTSensor.h"

const char* MQTTConnection::mqttServer = "your_mqtt_server";
const int MQTTConnection::mqttPort = 1883;
const char* MQTTConnection::mqttUser = "your_mqtt_user";
const char* MQTTConnection::mqttPassword = "your_mqtt_password";
WiFiClient MQTTConnection::wifiClient;
PubSubClient MQTTConnection::mqttClient(MQTTConnection::wifiClient);

void MQTTConnection::setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword)) {
      mqttClient.subscribe("topic/temperature_humidity");
    } else {
      delay(5000);
    }
  }
}

void MQTTConnection::handleMQTT() {
  if (!mqttClient.connected()) {
    setupMQTT();
  }
  mqttClient.loop();
}

void MQTTConnection::callback(char* topic, byte* payload, unsigned int length) {
  // Code de gestion des messages MQTT reçus
  // ...

  //récupération des données du capteur DHT11
  if (strcmp(topic, "topic/temperature_humidity") == 0) {
    float temperature, humidity;
    // Lire les données du capteur DHT11
    DHTSensor::readData(temperature, humidity);
    // Envoyer les données via MQTT
    // ...
  }
}