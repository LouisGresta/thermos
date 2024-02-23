
void connectedWifi(const char* ssid, const char* password); //Connectionn au wifi

/** MQTT ****/

//void MQTTconnect(const char* IdClient, const char* TopicSub); //Connexion au MQTT
//void MQTTsend(); //Fonctions mqtt publication des messages
//void MQTTconctedServeurParam (char* mqtt_server, unsigned int MQTT_PORT); //Parametre Connexion au serveur MQTT
//void conectedServeurMQTT(const char* IdClient_R, char* TopicSub_R, char* TopicPub_R, float Poyload_Pub_R); //Connexion au serveur de MQTT
float CptTempHum(int Pin, int index); //Capteur temperature