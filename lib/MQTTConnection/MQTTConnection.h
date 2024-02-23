#ifndef MQTT_CONNECTION_H
#define MQTT_CONNECTION_H

    #include <ESP8266WiFi.h>
    #include <PubSubClient.h>

    class MQTTConnection {
    public:
    static void setupMQTT();
    static void handleMQTT();

    private:
    static const char* mqttServer;
    static const int mqttPort;
    static const char* mqttUser;
    static const char* mqttPassword;
    static WiFiClient wifiClient;
    static PubSubClient mqttClient;
    static void callback(char* topic, byte* payload, unsigned int length);
    };

#endif