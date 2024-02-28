#include <Arduino.h>
#include "wifi_mqtt/wifi_mqtt.h"
#ifdef IHM
#include "joystick/joystick.h"
#endif
#ifdef OLED
#include "afficheur/afficheur.h"
#endif

#define NB_MAX_CAPTEURS 5
int nbCapteurs = 0;
int currentCapteurIndex = 0;
bool isCapteurSelected = false;

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;

struct CapteurData
{
  char name[NB_CHARS];
  float temperature;
  float humidity;
  float seuil;
};

CapteurData capteurs[NB_MAX_CAPTEURS];


// Function prototypes
void setupCapteursList(CapteurData *capteurs);
void addCapteur(char *name, float temperature, float humidity);
void printCapteur(CapteurData capteur);
void changeCapteurIfNecessary();
void changeSeuilValueIfNecessary();
void flashSeuil();

void callback(char *topic, byte *payload, unsigned int length);
void clearMsg(char* msg, int start = 0, int size = MSG_BUFFER_SIZE);

void setup() {
  Serial.begin(115200);

  uint16_t time = millis();
  time = millis() - time;
  delay(500);

  setup_wifi();
  setup_mqtt(callback);
  #ifdef IHM
  setupJoystick();
  setupCapteursList(capteurs);
  #endif
  #ifdef OLED
  setupTFT();
  #endif
}

void loop() {
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  #ifdef IHM
  changeCapteurIfNecessary();

  if (!isCapteurSelected && ReadClick())
  {
    Serial.println("Capteur sélectionné !");
    isCapteurSelected = !isCapteurSelected;
  }
  if (isCapteurSelected)
  {
    changeSeuilValueIfNecessary();
    flashSeuil();
    if (ReadClick())
    {
      // Send new seuil to MQTT
      clearMsg(msg);
      snprintf(msg, NB_CHARS, "%lf", capteurs[currentCapteurIndex].seuil);
      client.publish("/YNOV_BDX/Projet_IoT/Capteurs/seuil", msg);
      // unselect capteur and print it
      isCapteurSelected = !isCapteurSelected;
      printCapteur(capteurs[currentCapteurIndex]);
    }
  }
  
  #endif
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  #ifdef OLED
  if (strcmp(topic, "/YNOV_BDX/Projet_IoT/Afficheur/temperature") == 0)
  {
    // Remove previous temperature on screen
    clearLine(2);
    delay(100);
    // Print message on screen
    snprintf(msg, MSG_BUFFER_SIZE, "Temperature : %s", (char *) payload);
    clearMsg(msg, length + 14); // 14 = length of "Temperature : "
    
    drawTextAt(0, 2, msg, WHITE);
  }
  if (strcmp(topic, "/YNOV_BDX/Projet_IoT/Afficheur/humidite") == 0)
  {
    // Remove previous humidity on screen
    clearLine(3);
    delay(100);
    // Print message on screen
    snprintf(msg, MSG_BUFFER_SIZE, "Humidite : %s", (char *) payload);
    clearMsg(msg, length + 11); // 11 = length of "Humidite : "
    drawTextAt(0, 3, msg, WHITE);
  }


  #endif
}

void clearMsg(char* msg, int start, int size)
{
  for (int i = start; i < MSG_BUFFER_SIZE; i++)
  {
    msg[i] = '\0';
  }
}

void setupCapteursList(CapteurData *capteurs)
{
  for (int i = 0; i < NB_MAX_CAPTEURS; i++)
  {
    char *name = (char *)malloc(NB_CHARS * sizeof(char));
    snprintf(name, NB_CHARS, "Capteur %d", i + 1);
    strcpy(capteurs[i].name, name);
    capteurs[i].temperature = 0;
    capteurs[i].humidity = 0;
    capteurs[i].seuil = 0;
  }
}

void addCapteur(char *name, float temperature, float humidity)
{
  if (nbCapteurs >= NB_MAX_CAPTEURS)
  {
    Serial.println("Nombre de capteurs maximum atteint");
    return;
  }
  
  strcpy(capteurs[nbCapteurs].name, name);
  capteurs[nbCapteurs].temperature = temperature;
  capteurs[nbCapteurs].humidity = humidity;
  nbCapteurs++;
}

void printCapteur(CapteurData capteur)
{
  Serial.print("Capteur : ");
  Serial.print(capteur.name);
  Serial.print(" Temperature : ");
  Serial.print(capteur.temperature);
  Serial.print(" Humidite : ");
  Serial.println(capteur.humidity);
  // Remove previous capteur on screen
  tft.fillScreen(BLACK);
  delay(250);
  // Print capteur on screen
  drawTextAt(0, 0, capteur.name, WHITE);
  clearMsg(msg);
  snprintf(msg, NB_CHARS, "Temperature : %lf", capteur.temperature);
  drawTextAt(0, 2, msg, WHITE);
  clearMsg(msg);
  snprintf(msg, NB_CHARS, "Humidite : %lf", capteur.humidity);
  drawTextAt(0, 3, msg, WHITE);
  clearMsg(msg);
  snprintf(msg, NB_CHARS, "Seuil : %lf", capteur.seuil);
  drawTextAt(0, 6, msg, WHITE);
}

void printSeuil(float seuil, uint16_t color = WHITE)
{
  clearLine(6);
  clearMsg(msg);
  if (color == WHITE)
  {
    tft.setTextColor(BLACK);
  }
  else
  {
    tft.setTextColor(WHITE);
  }
  snprintf(msg, NB_CHARS, "Seuil : %lf", seuil);
  drawTextAt(0, 6, msg, WHITE);
}

void changeCapteurIfNecessary()
{
  if (ReturnDirection() == 0)
  {
    currentCapteurIndex--;
    if (currentCapteurIndex < 0)
    {
      currentCapteurIndex = nbCapteurs - 1;
    }
    printCapteur(capteurs[currentCapteurIndex]);
  }
  if (ReturnDirection() == 2)
  {
    currentCapteurIndex++;
    if (currentCapteurIndex >= nbCapteurs)
    {
      currentCapteurIndex = 0;
    }
    printCapteur(capteurs[currentCapteurIndex]);
  }
}

void changeSeuilValueIfNecessary()
{
  if (ReturnDirection() == 0)
  {
    capteurs[currentCapteurIndex].seuil -= 0.1;
    // refresh seuil on screen
    printSeuil(capteurs[currentCapteurIndex].seuil);
  }
  if (ReturnDirection() == 2)
  {
    capteurs[currentCapteurIndex].seuil += 0.1;
    // refresh seuil on screen
    printSeuil(capteurs[currentCapteurIndex].seuil);
  }
}

void flashSeuil()
{
  static bool isSeuilWhite = false;
  if (millis() % 1000 < 500)
  {
    printSeuil(capteurs[currentCapteurIndex].seuil, WHITE);
  }
  else
  {
    printSeuil(capteurs[currentCapteurIndex].seuil, BLACK);
  }
}