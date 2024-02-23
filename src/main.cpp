#include <Arduino.h>
#include <function.h>

#include <PubSubClient.h>
#include <ESP8266WiFi.h> //Importe cette librairie
#include <SPI.h>

#include <Servo.h> //Servo motor
Servo myservo;

/* Nom et mot de passe du hotspot */
//WiFi Connection configuration
const char* ssid = "Galaxy S22 UltraDC7F"; //Nom du wifi
const char* password = "oyyy2060"; //Mot de passe du wifi

//mqtt server
char mqtt_server[] = "broker.emqx.io";  //adresse IP serveur/127.0.0.1 
#define MQTT_USER ""
#define MQTT_PASS ""
#define MQTT_PORT 1883 //Port du serveur MQTT

// Les parametre de MQTT Id client et Topic
#define IDClient_MQTT "TestClient-" //Id client de MQTT unique
//char Topic_MQTT [] = "/YNOV_BDX/Ib"; //le nom du topic
#define Topic_Hum_MQTT "/YNOV_BDX/Dh11-Hum"
#define Topic_Temp_MQTT "/YNOV_BDX/Dh11-Temp"

WiFiClient espClient;
PubSubClient MQTTclient(espClient);

//Capteur temperature
#define DHT_SENSOR_PIN 9 // The ESP8266 pin D7 connected to DHT11 sensor 

/***********Variable global*******/
float TabCpt;

//Connexion au MQTT
void MQTTconnect() 
{

  while (!MQTTclient.connected()) 
  {//on vérifie la connexion au Broker, afin de se reconnecter si besoin par la fonction MQTTconnect().
  //Les clients se connectant au serveur Broker, doivent disposer d’un identifiant unique.
    Serial.print("Attente  MQTT connection...");

    String clientId = IDClient_MQTT; //ClienId a changer

    clientId += String(random(0xffff), HEX);

    // test connexion
    if (MQTTclient.connect(clientId.c_str(),"","")) 
    {// Permet de se connecter au broker

      Serial.println("\nMQTT connected\n");

      //Permet d'écouter le topic, afin de récupérer ces informations.
      MQTTclient.subscribe(Topic_Temp_MQTT);
      MQTTclient.subscribe(Topic_Hum_MQTT);
      MQTTclient.subscribe("/YNOV_BDX/servo");
    } 
    else 
    {  // si echec affichage erreur

      Serial.print("\nECHEC de connexion MQTT, rc=");
      Serial.print(MQTTclient.state());

      Serial.println("\nNouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

//Fonctions mqtt envoyer de message
void MQTTsend() 
{//Permet de formater les donnée à envoyer en utilisant la commande MQTTclient.publish.

  float TabCpt_Temp = CptTempHum(DHT_SENSOR_PIN, 1);
  float TabCpt_Hum = CptTempHum(DHT_SENSOR_PIN, 0);

  String reponseT = (String)TabCpt_Temp;
  MQTTclient.publish(Topic_Temp_MQTT, reponseT.c_str());
  //Le premier paramètre est le Topic, second message a publier

  String reponseH = (String)TabCpt_Hum;
  MQTTclient.publish(Topic_Hum_MQTT, reponseH.c_str());
  //Le premier paramètre est le Topic, second message a publier
}

void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  String string1 = "";

  Serial.print("Message MQTT [");
  Serial.print(topic);
  Serial.print("] ");

  payload[length] = '\0';
  String s = String((char*)payload);

  Serial.println("message reçu : "+s);

  if ( topic == "/YNOV_BDX/servo"){
    int status = string1.toInt();
    int pos = map(status, 1, 100, 0, 180);
    Serial.println(pos);
    myservo.write(pos);
    delay(10);
  }
}

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200); //Initialise la communication entre le PC et Arduino
  Serial.print("\n"); //Saut de ligne

  connectedWifi(ssid, password); //Connection au Wifi
  //MQTTconctedServeurParam(mqtt_server, MQTT_PORT); //Parametre Connexion au serveur MQTT

  MQTTclient.setServer(mqtt_server, 1883);
  MQTTclient.setCallback(MQTTcallback);

  // Servomottor setup 
    myservo.attach(13);
    myservo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:

  static uint32_t  lastTimeMqtt= 0;

  // connect serveur MQTT
  if (!MQTTclient.connected()) 
  {
      MQTTconnect();
  }

  if (millis() - lastTimeMqtt >= 10000)  // toutes les 10 secondes
  {//Réalisé une boucle qui publie toutes les 10 secondes 
      lastTimeMqtt = millis();
      MQTTsend();
  }

  //Cette dernière a pour but de surveiller les données entrantes et de rediriger vers la fonction définie par setcallback ( ici MQTTcallback).
  MQTTclient.loop();
}