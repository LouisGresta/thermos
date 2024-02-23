#ifndef WIFI_CONNECTION_H
    #define WIFI_CONNECTION_H

    #include <ESP8266WiFi.h>

    class WiFiConnection {
    public:
    static void setupWiFi();
    static void handleWiFi();

    private:
    static const char* ssid;
    static const char* password;
    };

#endif