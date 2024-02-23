#include "WiFiConnection.h"
#include "MQTTConnection.h"
#ifdef IHM
#include "LCDDisplay.h"
#endif
#ifdef CAPTEUR
#include "DHTSensor.h"
#endif

void setup() {
  // Initialisation des différentes parties du système
  WiFiConnection::setupWiFi();
  MQTTConnection::setupMQTT();
  LCDDisplay::setupLCD();
  DHTSensor::setupDHT();
}

void loop() {
  // Boucle principale
  WiFiConnection::handleWiFi();
  MQTTConnection::handleMQTT();
  LCDDisplay::handleLCD();
}