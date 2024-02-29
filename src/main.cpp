#include <Arduino.h>
#include <Servo.h>

#include "WiFiMqtt_H/WifiMqtt.h"
#include "CapteurH/capteur.h"
#include "ControleurH/controleur.h"
#include "MoteurH/moteur.h"

/**Capteur DHT11*****/
DHTesp dht;

Servo monServo;

/* Nom et mot de passe du hotspot */
//WiFi Connection configuration
const char* ssid = "Galaxy S22 UltraDC7F"; //Nom du wifi
const char* password = "oyyy2060"; //Mot de passe du wifi

extern PubSubClient MQTTclient; 

/***********Variable global*******/
int pos = NULL;

void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  String string1 = "";

  Serial.print("Message MQTT [");
  Serial.print(topic);
  Serial.print("] ");

  payload[length] = '\0';
  String DataMqtt = String((char*)payload);

  Serial.println("message reçu : "+ DataMqtt);

  /*******Servo-moteur************/
  int flag = compareTopics(topic, Topic_Hum_MQTT);
  pos = SetservoMoteur(flag, DataMqtt, &monServo);

  /***Pour la led******/
  int flagLed = compareTopicsM(topic);
  MoteurLed(flagLed, DataMqtt);

  /****Pour le mode*****/
  int flagMod = compareTopicsMod(topic);
  MoteurMode(flagMod, DataMqtt);

  /*****Sens******/
  int flagSens = compareTopicsSens(topic);
  MoteurSens(flagSens, DataMqtt);

}

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200); //Initialise la communication entre le PC et Arduino
  Serial.print("\n"); //Saut de ligne

  connectedWifi(ssid, password); //Connection au Wifi
  setup_mqtt(MQTTcallback);

  setup_Capt(&dht);
  setup_servo(&monServo);
}

void loop() {
  // put your main code here, to run repeatedly:

  float sensorValues[2];
  CptTempHum(sensorValues, &dht);

  static uint32_t  lastTimeMqtt= 0;

  // connect serveur MQTT
  if (!MQTTclient.connected()) 
  {
      MQTTconnect();
  }

  if (millis() - lastTimeMqtt >= 10000)  // toutes les 10 secondes
  {//Réalisé une boucle qui publie toutes les 10 secondes

    lastTimeMqtt = millis();
    MQTTsendHum(sensorValues[0]);
    delay(500);

    MQTTsendTemp(sensorValues[1]);
    delay(600);

    MQTTsendPos(pos);
    delay(700);
  }

  //Cette dernière a pour but de surveiller les données entrantes et de rediriger vers la fonction définie par setcallback ( ici MQTTcallback).
  MQTTclient.loop();
}