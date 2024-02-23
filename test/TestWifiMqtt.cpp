#include <Arduino.h>

//Librairie necessaire pour MQTT
#include <SPI.h>
#include <ESP8266WiFi.h> //Importe cette librairie
#include <PubSubClient.h>

/* Nom et mot de passe du hotspot */
//WiFi Connection configuration
const char* ssid = "Galaxy S22 UltraDC7F"; //Nom du wifi
const char* password = "oyyy2060"; //Mot de passe du wifi

//mqtt server
char mqtt_server[] = "broker.emqx.io";  //adresse IP serveur/127.0.0.1 
#define MQTT_USER ""
#define MQTT_PASS ""

WiFiClient espClient;
PubSubClient MQTTclient(espClient);

//Fonctions mqtt envoyer de message
void MQTTsend() 
{//Permet de formater les donnée à envoyer en utilisant la commande MQTTclient.publish.

  static int cpt=0;
  cpt++;

  String reponse="test Mqtt n°"+(String)cpt;
  MQTTclient.publish("/YNOV_BDX/Ib", reponse.c_str());
  //Le premier paramètre est le Topic, second message a publier
}

void MQTTconnect() {

  while (!MQTTclient.connected()) 
  {//on vérifie la connexion au Broker, afin de se reconnecter si besoin par la fonction MQTTconnect().
  //Les clients se connectant au serveur Broker, doivent disposer d’un identifiant unique.
    Serial.print("Attente  MQTT connection...");

    String clientId = "TestClient-"; //ClienId a changer

    clientId += String(random(0xffff), HEX);

    // test connexion
    if (MQTTclient.connect(clientId.c_str(),"","")) 
    {// Permet de se connecter au broker

      Serial.println("connected");

      //Permet d'écouter le topic, afin de récupérer ces informations.
      MQTTclient.subscribe("/YNOV_BDX/Ib");

    } 
    else 
    {  // si echec affichage erreur

      Serial.print("ECHEC, rc=");
      Serial.print(MQTTclient.state());

      Serial.println(" nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

//Connectionn au wifi
void connectedWifi()
{
  WiFi.begin(ssid, password); //Les mots de connexion
  /*WiFi.begin("Wokwi-GUEST", "", 6); *///Pour le test de simulation du site Wokwi

  Serial.print("\n");
  Serial.print("Connexion en cour.");

  while (WiFi.status() != WL_CONNECTED) 
  {//Connexion du wifi
    Serial.print(".");
    delay(500);
  }

  //Si on est connecter au wifi
  Serial.println("\n");
  Serial.println("Connexion etablie !");
  Serial.print("Adresse IP: ");
  Serial.print( WiFi.localIP() );
}

//afin de récupérer cette information. Pour cela j’utilise la fonction : MQTTclient.subscribe(« byfeel/mqtt/RX »);
//Pour traiter les messages reçu sur ce topic , je créé la fonction MQTTcallback()
void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message MQTT [");
  Serial.print(topic);
  Serial.print("] ");

  payload[length] = '\0';
  String s = String((char*)payload);

  Serial.println("message reçu : "+s);
}

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200); //Initialise la communication entre le PC et Arduino
  Serial.print("\n"); //Saut de ligne

  //WiFi.persistent(false); //Permet de reinitialise la memoir flash

  connectedWifi(); //Connection au Wifi

  //on affecte l’adresse du serveur et le port , au client MQTT créé.
  MQTTclient.setServer(mqtt_server, 1883);
  MQTTclient.setCallback(MQTTcallback);
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