#include <Arduino.h>
#include <SPI.h>

#include <ESP8266WiFi.h> //Importe cette librairie
#include <PubSubClient.h>

#include <DHTesp.h>  //Librairie du Capteur temp DHT11
#define DHT_SENSOR_TYPE DHT11


//WiFiClient espClient;
//PubSubClient MQTTclient(espClient);


//Connectionn au wifi
void connectedWifi(const char* ssid, const char* password)
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
    Serial.print("\n\n");
}

/*** Partie MQTT***/

//Connexion au MQTT
/*void MQTTconnect(const char* IdClient, char* TopicSub) 
{

  while (!MQTTclient.connected()) 
  {//on vérifie la connexion au Broker, afin de se reconnecter si besoin par la fonction MQTTconnect().
  //Les clients se connectant au serveur Broker, doivent disposer d’un identifiant unique.
    Serial.print("Attente  MQTT connection...");

    String clientId = IdClient; //ClienId a changer

    clientId += String(random(0xffff), HEX);

    // test connexion
    if (MQTTclient.connect(clientId.c_str(),"","")) 
    {// Permet de se connecter au broker

      Serial.println("\nMQTT connected\n");

      //Permet d'écouter le topic, afin de récupérer ces informations.
      MQTTclient.subscribe(TopicSub);
    } 
    else 
    {  // si echec affichage erreur

      Serial.print("\nECHEC de connexion MQTT, rc=");
      Serial.print(MQTTclient.state());

      Serial.println("\nNouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}*/

//Fonctions mqtt envoyer de message
/*void MQTTsend(char* TopicPub, float Poyload_Pub) 
{//Permet de formater les donnée à envoyer en utilisant la commande MQTTclient.publish.

  //float reponsR = (float) Poyload_Pub;
  String reponse = (String) Poyload_Pub;
  MQTTclient.publish(TopicPub, reponse.c_str());
  //Le premier paramètre est le Topic, second message a publier
}*/

//afin de récupérer cette information. Pour cela j’utilise la fonction : MQTTclient.subscribe(« byfeel/mqtt/RX »);
//Pour traiter les messages reçu sur ce topic , je créé la fonction MQTTcallback()
/*void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message MQTT [");
  Serial.print(topic);
  Serial.print("] ");

  payload[length] = '\0';
  String s = String((char*)payload);

  Serial.println("message reçu : "+s);
}*/

//Parametre Connexion au serveur MQTT
/*void MQTTconctedServeurParam (char* mqtt_server, unsigned int MQTT_PORT)
{
    //on affecte l’adresse du serveur et le port , au client MQTT créé.
    MQTTclient.setServer(mqtt_server, MQTT_PORT);
    MQTTclient.setCallback(MQTTcallback);
}*/

//Connexion au serveur de MQTT
/*void conectedServeurMQTT(const char* IdClient_R, char* TopicSub_R, char* TopicPub_R, float Poyload_Pub_R)
{
  static uint32_t  lastTimeMqtt= 0;

  // connect serveur MQTT
  if (!MQTTclient.connected()) 
  {
      MQTTconnect(IdClient_R, TopicSub_R);
  }

  if (millis() - lastTimeMqtt >= 10000)  // toutes les 10 secondes
  {//Réalisé une boucle qui publie toutes les 10 secondes 
      lastTimeMqtt = millis();
      MQTTsend(TopicPub_R, Poyload_Pub_R);
  }

  //Cette dernière a pour but de surveiller les données entrantes et de rediriger vers la fonction définie par setcallback ( ici MQTTcallback).
  MQTTclient.loop();
}*/

//Capteur Temperature
float CptTempHum(int Pin, int index)
{
  DHTesp dht;
  dht.setup(Pin, DHTesp::DHT_SENSOR_TYPE); // Connect DHT sensor to GPIO 17

  float buffer[2];

  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print("Humidity : ");
  Serial.println(humidity, 1);
  buffer[0] = humidity;
  //Serial.print("\n");

  Serial.print("Temperature : ");
  Serial.println(temperature, 1);
  buffer[1] = temperature;
  Serial.print("\n");

  delay(2000);

  Serial.print("buffer : ");
  Serial.println(buffer[0]);

  return buffer[index];
}