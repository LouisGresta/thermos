#include <WiFiMqtt_H/WifiMqtt.h>

//mqtt server
char mqtt_server[] = "broker.emqx.io";  //adresse IP serveur/127.0.0.1 
#define MQTT_USER ""
#define MQTT_PASS ""
#define MQTT_PORT 1883 //Port du serveur MQTT

WiFiClient espClient;
PubSubClient MQTTclient(espClient);


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

//Partie setup du MQTT
void setup_mqtt(void (*MQTTcallback)(char *, byte *, unsigned int))
{
  MQTTclient.setServer(mqtt_server, mqtt_port);
  MQTTclient.setCallback(MQTTcallback);
}

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

      Serial.println("\nMQTT connected...\n");

      //Permet d'écouter le topic, afin de récupérer ces informations.
      MQTTclient.subscribe(Topic_Temp_MQTT);
      MQTTclient.subscribe(Topic_Hum_MQTT);

      MQTTclient.subscribe(Topic_Serv_MQTT);
      MQTTclient.subscribe(Topic_Led_Mqtt);

      MQTTclient.subscribe(Topic_Mod_Mqtt);
      MQTTclient.subscribe(Topic_Sens_Mqtt);
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
void MQTTsendTemp(float Temp) 
{//Permet de formater les donnée à envoyer en utilisant la commande MQTTclient.publish.


  String reponse = (String)Temp;
  MQTTclient.publish(Topic_Temp_MQTT, reponse.c_str());
  //Le premier paramètre est le Topic, second message a publier

}

//Fonctions mqtt envoyer de message
void MQTTsendHum(float Hum) 
{//Permet de formater les donnée à envoyer en utilisant la commande MQTTclient.publish.

  String reponse = (String)Hum;
  MQTTclient.publish(Topic_Hum_MQTT, reponse.c_str());
  //Le premier paramètre est le Topic, second message a publier
}

//Fonctions mqtt envoyer de message
void MQTTsendPos(int pos) 
{//Permet de formater les donnée à envoyer en utilisant la commande MQTTclient.publish.

  String reponse = (String)pos;
  MQTTclient.publish(Topic_Serv_MQTT, reponse.c_str());
  //Le premier paramètre est le Topic, second message a publier
}