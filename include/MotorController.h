#ifndef MotorController_h
#define MotorController_h


#define ENA D7
#define IN1 D6
#define IN2 D5


#define ENA D7
#define IN1 D6
#define IN2 D5

#define ssid = "Galaxy S22 UltraDC7F"
#define password = "oyyy2060"
#define  mqtt_server = "broker.emqx.io"
#define mqtt_topic = "/YNOV_BDX/servo"

#define BAUD 115200



#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MotorController {
    public:
        MotorController();
        void setup();
        void setup_wifi();
        void callback(char* topic, byte* payload, unsigned int length);
        void reconnect();
        void loop();
        void setDirection(String sens);
        void changeSpeed(int pwmValue);

    private:
        const char* ssid;
        const char* password;
        const char* mqtt_server;
        const char* mqtt_topic;

        const int frequency;
        const int pwm_channel;
        const int resolution;

        WiFiClient espClient;
        PubSubClient client;
};
#endif
