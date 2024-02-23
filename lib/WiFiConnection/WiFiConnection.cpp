#include "WiFiConnection.h"

const char* WiFiConnection::ssid = "YourWiFiSSID";
const char* WiFiConnection::password = "YourWiFiPassword";

void WiFiConnection::setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void WiFiConnection::handleWiFi() {
  // Gestion de la connexion WiFi
  // ...
}