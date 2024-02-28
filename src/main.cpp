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
int addedCapteurNotificationTime = 0;

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
  // setupCapteursList(capteurs);
  Serial.println("Aucun capteur connecté");
  #ifdef OLED
  setupTFT();
  drawTextAt(0, 0, "No connected captors", WHITE);
  #endif
  #endif
}

void loop() {
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  #ifdef IHM

  if (!isCapteurSelected && ReadClick())
  {
    // Sélection du capteur
    Serial.println("Capteur sélectionné !");
    isCapteurSelected = !isCapteurSelected;
  }
  if (isCapteurSelected)
  {
    // Capteur sélectionné
    changeSeuilValueIfNecessary();
    flashSeuil();
    if (ReadClick())
    {
      // Désélection du capteur et envoie mqtt du nouveau seuil sur le topic du capteur
      Serial.println("Capteur désélectionné !");
      clearMsg(msg);
      snprintf(msg, NB_CHARS, "%f", capteurs[currentCapteurIndex].seuil);
      char topic[MSG_BUFFER_SIZE] = "/YNOV_BDX/Projet_IoT/Capteurs/";
      strcat(topic, capteurs[currentCapteurIndex].name);
      strcat(topic, "/seuil");
      client.publish(topic, msg);
      isCapteurSelected = !isCapteurSelected;
      printCapteur(capteurs[currentCapteurIndex]);
    }
  }
  else if (nbCapteurs > 0)
  {
    // Capteur non sélectionné
    changeCapteurIfNecessary();
  }
  // Clear notification after 3 second
  if (millis() - addedCapteurNotificationTime > 3000) {
    clearLine(9);
    clearLine(10);
  }
  delay(100);
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

  #ifdef IHM
  if (strstr(topic, "/YNOV_BDX/Projet_IoT/Capteurs/") != NULL && strstr(topic, "/mesure") != NULL)
  {
    char *capteurName = strstr(topic, "/YNOV_BDX/Projet_IoT/Capteurs/") + 30;
    strcpy(capteurName, strtok(capteurName, "/")); // remove what's after the captor name
    char *temperature = (char *)payload;
    char *humidity = (char *)payload + 5;
    // Check if captor already exists
    bool capteurExists = false;
    for (int i = 0; i < nbCapteurs; i++)
    {
      if (strcmp(capteurName, capteurs[i].name) == 0)
      {
        capteurs[i].temperature = atof(temperature);
        capteurs[i].humidity = atof(humidity);
        printCapteur(capteurs[i]);
        capteurExists = true;
      }
    }
    // Add captor if it doesn't exist
    if (!capteurExists)
    {
      addCapteur(capteurName, atof(temperature), atof(humidity));
      // Print first capteur on screen
      if (nbCapteurs == 1)
      {
        printCapteur(capteurs[0]);
      }
    }
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
    char name[NB_CHARS] = "Capteur ";
    name[8] = i+1 + 48;
    addCapteur(name, 0, 0);
    Serial.print("Capteur : ");
    Serial.print(capteurs[i].name);
    Serial.print(" Temperature : ");
    Serial.print(capteurs[i].temperature);
    Serial.print(" Humidite : ");
    Serial.println(capteurs[i].humidity);
  }
  nbCapteurs = NB_MAX_CAPTEURS;
}

void addCapteur(char *name, float temperature, float humidity)
{
  Serial.print("Ajout du capteur : ");
  Serial.println(name);
  if (nbCapteurs >= NB_MAX_CAPTEURS)
  {
    Serial.println("Nombre de capteurs maximum atteint");
    return;
  }
  
  strcpy(capteurs[nbCapteurs].name, name);
  capteurs[nbCapteurs].temperature = temperature;
  capteurs[nbCapteurs].humidity = humidity;
  nbCapteurs++;
  // Print notification on screen
  clearLine(9);
  clearLine(10);
  drawTextAt(0, 9, name, WHITE);
  drawTextAt(0, 10, "est connecte", WHITE);
  addedCapteurNotificationTime = millis();
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
  snprintf(msg, NB_CHARS, "Temperature : %f", capteur.temperature);
  drawTextAt(0, 2, msg, WHITE);
  clearMsg(msg);
  snprintf(msg, NB_CHARS, "Humidite    : %f", capteur.humidity);
  drawTextAt(0, 3, msg, WHITE);
  clearMsg(msg);
  snprintf(msg, NB_CHARS, "Seuil : %f", capteur.seuil);
  drawTextAt(0, 6, msg, WHITE);
}

void printSeuil(float seuil, uint16_t color = WHITE)
{
  clearLine(6);
  clearMsg(msg);
  if (color == WHITE)
  {
    tft.fillRect(0, 6 * CHAR_HEIGHT, SCREEN_WIDTH, CHAR_HEIGHT, BLACK);
  }
  else
  {
    tft.fillRect(0, 6 * CHAR_HEIGHT, SCREEN_WIDTH, CHAR_HEIGHT, WHITE);
  }
  snprintf(msg, NB_CHARS, "Seuil : %lf", seuil);
  drawTextAt(0, 6, msg, color);
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
    Serial.println("Baisse du seuil");
    capteurs[currentCapteurIndex].seuil -= 0.1;
    // refresh seuil on screen
    printSeuil(capteurs[currentCapteurIndex].seuil, BLACK);
  }
  if (ReturnDirection() == 2)
  {
    Serial.println("Augmentation du seuil");
    capteurs[currentCapteurIndex].seuil += 0.1;
    // refresh seuil on screen
    printSeuil(capteurs[currentCapteurIndex].seuil, BLACK);
  }
}

void flashSeuil()
{
  int last = 0;
  if (millis() - last > 500)
  {
    last = millis();
    printSeuil(capteurs[currentCapteurIndex].seuil, BLACK);
  }
}