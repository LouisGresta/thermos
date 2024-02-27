#include <Arduino.h>

#include <ESP8266WiFi.h> //Importe cette librairie

/* Nom et mot de passe du hotspot */ 
const char* ssid = "NomWifi"; //Nom du wifi
const char* password = "MDP"; //Mot de passe du wifi

/* Nom et mot de passe du hotspot pour creer un wifi avec ESP32*/ 
//const char* ssid = "ESP32-Wifi"; //Nom du wifi
//const char* password = "Esp32-1234"; //Mot de passe du wifi

void creatWifi()
{
  /*Creation du wifi avec ESP32*/
  Serial.println("Creation du point d'acces...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.print("Adresse IP: ");
  Serial.print(WiFi.softAPIP());
}

//Connectionn aussi wifi
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

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200); //Initialise la communication entre le PC et Arduino
  Serial.print("\n"); //Saut de ligne

  //WiFi.persistent(false); //Permet de reinitialise la memoir flash

  //creatWifi(); //Creation du wifi avec ESP32
  connectedWifi(); //Connection au Wifi

}

void loop() {
  // put your main code here, to run repeatedly:

}