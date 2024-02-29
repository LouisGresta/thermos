#include <Arduino.h>
#include "wifi_mqtt/wifi_mqtt.h"
#ifdef IHM
#include "joystick/joystick.h"
#include "afficheur/afficheur.h"
#endif

#define CAPTEURS_TOPIC "/YNOV_BDX/Projet_IoT/Capteurs/"
#define ACTIONNEURS_TOPIC "/YNOV_BDX/Projet_IoT/Actionneurs/"
#define NB_MAX_CAPTEURS 5
#define SEUIL_STEP 0.2
int nbCapteurs = 0;
int currentCapteurIndex = 0;
bool isCapteurSelected = false;
bool isActionneurSelected = false;

int addedCapteurNotificationTime = 0;
int addedActionneurNotificationTime = 0;

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;

#define NB_CHAR_FLOAT 4
struct CapteurData
{
  char name[NB_CHARS];
  char actionneurName[NB_CHARS];
  float temperature;
  float humidity;
  float seuil;
};

char actionneurNames[NB_MAX_CAPTEURS][NB_CHARS];
int nbActionneurs = 0;
int currentActionneurIndex = 0;

CapteurData capteurs[NB_MAX_CAPTEURS];


// Function prototypes
// Capteur
void setupCapteursList(CapteurData *capteurs);
void addCapteur(char *name, float temperature, float humidity);
void printCapteur(CapteurData capteur, bool clearAll = true);
void changeCapteurIfNecessary();
// Actionneur
void addActionneur(char *name);
void changeActionneurIfNecessary();
void flashActionneur();
void printActionneur(char* actionneurName, uint16_t color = WHITE);
// Seuil
void changeSeuilValueIfNecessary();
void flashSeuil();
void printSeuil(float seuil, uint16_t color = WHITE);

void callback(char *topic, byte *payload, unsigned int length);
void clearMsg(char* msg, int start = 0, int size = MSG_BUFFER_SIZE);

void setup() {
  Serial.begin(115200);
  #ifdef IHM
  setupJoystick();
  //setupCapteursList(capteurs);
  Serial.println("Initialisation...");
  #ifdef OLED
  setupTFT();
  drawTextAt(0, 0, "Initialisation...", WHITE);
  #endif
  #endif
  setup_wifi();
  setup_mqtt(callback);
  #ifdef OLED
  clearLine(0);
  drawTextAt(0, 0, "No connected sensors", WHITE);
  #endif
}

void loop() {
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  #ifdef IHM

  if (!isCapteurSelected && nbCapteurs > 0)
  {
    // Capteur non sélectionné
    changeCapteurIfNecessary();
    if (ReadClick())
    {
      // Sélection du capteur
      Serial.println("Capteur sélectionné !");
      isCapteurSelected = !isCapteurSelected;
    }
  }
  if (isCapteurSelected && !isActionneurSelected) {
    // Sélection de l'actionneur
    changeActionneurIfNecessary();
    flashActionneur();
    if (ReadClick())
    {
      Serial.println("Actionneur sélectionné !");
      isActionneurSelected = !isActionneurSelected;
      strcpy(capteurs[currentCapteurIndex].actionneurName, actionneurNames[currentActionneurIndex]);
      printActionneur(actionneurNames[currentActionneurIndex]);
    }
  }
  if (isCapteurSelected && isActionneurSelected)
  {
    // Sélection du seuil
    changeSeuilValueIfNecessary();
    flashSeuil();
    if (ReadClick())
    {
      Serial.println("Seuil validé !");
      // Désélection du capteur et envoie mqtt du nouveau seuil sur le topic du capteur
      clearMsg(msg);
      snprintf(msg, NB_CHARS, "%f", capteurs[currentCapteurIndex].seuil);
      char topic[MSG_BUFFER_SIZE] = ACTIONNEURS_TOPIC;
      strcat(topic, capteurs[currentCapteurIndex].name);
      strcat(topic, "/seuil");
      client.publish(topic, msg);
      Serial.print("Publish seuil on topic : ");
      Serial.println(topic);
      isCapteurSelected = !isCapteurSelected;
      isActionneurSelected = !isActionneurSelected;
      printCapteur(capteurs[currentCapteurIndex]);
    }
  }
  // Clear notification after 3 second
  if (millis() - addedCapteurNotificationTime > 3000) {
    clearLine(9);
    clearLine(10);
  }
  if (millis() - addedActionneurNotificationTime > 3000) {
    clearLine(11);
    clearLine(12);
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

  #ifdef IHM
  if (strstr(topic, CAPTEURS_TOPIC) != NULL && strstr(topic, "/tempHum") != NULL)
  {
    char *capteurName = strstr(topic, CAPTEURS_TOPIC) + strlen(CAPTEURS_TOPIC);
    strcpy(capteurName, strtok(capteurName, "/")); // remove what's after the sensor name
    char *temperature = (char *)malloc(NB_CHARS);
    char *humidity = (char *)malloc(NB_CHARS);
    if (strstr((char *) payload, "nan") != NULL)
    {
      Serial.println("Mesure non acceptée");
      strcpy(temperature, "0.0");
      strcpy(humidity, "0.0");
    }
    else 
    {
      strcpy(temperature, strtok((char *)payload, " "));
      strcpy(humidity, strtok(NULL, " "));
    }
    // Check if sensor already exists
    bool capteurExists = false;
    for (int i = 0; i < nbCapteurs; i++)
    {
      if (strcmp(capteurName, capteurs[i].name) == 0)
      {
        capteurExists = true;
        capteurs[i].temperature = atof(temperature);
        capteurs[i].humidity = atof(humidity);
        printCapteur(capteurs[i], false);
      }
    }
    // Add sensor if it doesn't exist
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
  else if (strstr(topic, ACTIONNEURS_TOPIC) != NULL)
  {
    char *actionneurName = strstr(topic, ACTIONNEURS_TOPIC) + strlen(ACTIONNEURS_TOPIC);
    strcpy(actionneurName, strtok(actionneurName, "/")); // remove what's after the actionneur name
    bool actionneurExists = false;
    for (int i = 0; i < nbActionneurs; i++)
    {
      if (strcmp(actionneurName, actionneurNames[i]) == 0) actionneurExists = true;
    }
    if (!actionneurExists) addActionneur(actionneurName);
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
    addCapteur(name, 0.0, 0.0);
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

void addActionneur(char *name)
{
  Serial.print("Ajout de l'actionneur : ");
  Serial.println(name);
  if (nbActionneurs >= NB_MAX_CAPTEURS)
  {
    Serial.println("Nombre d'actionneurs maximum atteint");
    return;
  }
  strcpy(actionneurNames[nbActionneurs], name);
  nbActionneurs++;
  // Print notification on screen
  clearLine(11);
  clearLine(12);
  drawTextAt(0, 11, name, WHITE);
  drawTextAt(0, 12, "est connecte", WHITE);
  addedActionneurNotificationTime = millis();
}

void printCapteur(CapteurData capteur, bool clearAll)
{
  Serial.print("Capteur : ");
  Serial.print(capteur.name);
  Serial.print(" Temperature : ");
  Serial.print(capteur.temperature);
  Serial.print(" Humidite : ");
  Serial.println(capteur.humidity);
  // Remove previous capteur on screen
  if (clearAll) tft.fillScreen(BLACK);
  else tft.fillRect(0, 0, SCREEN_WIDTH, 4 * CHAR_HEIGHT, BLACK); // clear only capteur data
  // Print capteur on screen
  drawTextAt(0, 0, capteur.name, WHITE);
  clearMsg(msg);
  snprintf(msg, NB_CHARS, "Temperature : %f", capteur.temperature);
  drawTextAt(0, 2, msg, WHITE);
  clearMsg(msg);
  snprintf(msg, NB_CHARS, "Humidite    : %f", capteur.humidity);
  drawTextAt(0, 3, msg, WHITE);
  if (clearAll) {
    clearMsg(msg);
    strcpy(msg, "Actionneur : ");
    drawTextAt(0, 6, msg, WHITE);
    clearMsg(msg);
    snprintf(msg, NB_CHARS, "%s", capteur.actionneurName);
    drawTextAt(0, 7, msg, WHITE);
    clearMsg(msg);
    snprintf(msg, NB_CHARS, "Seuil : %f", capteur.seuil);
    drawTextAt(0, 8, msg, WHITE);
  }
}

void printSeuil(float seuil, uint16_t color)
{
  static int seuilLine = 8;
  clearLine(seuilLine);
  clearMsg(msg);
  if (color == WHITE)
  {
    tft.fillRect(0, seuilLine * CHAR_HEIGHT, SCREEN_WIDTH, CHAR_HEIGHT, BLACK);
  }
  else
  {
    tft.fillRect(0, seuilLine * CHAR_HEIGHT, SCREEN_WIDTH, CHAR_HEIGHT, WHITE);
  }
  snprintf(msg, NB_CHARS, "Seuil : %f", seuil);
  drawTextAt(0, seuilLine, msg, color);
}

void printActionneur(char* actionneurName, uint16_t color)
{
  static int actionneurLine = 6;
  clearLine(actionneurLine);
  clearLine(actionneurLine + 1);
  clearMsg(msg);
  if (color == WHITE)
  {
    tft.fillRect(0, actionneurLine * CHAR_HEIGHT, SCREEN_WIDTH, 2 * CHAR_HEIGHT, BLACK);
  }
  else
  {
    tft.fillRect(0, actionneurLine * CHAR_HEIGHT, SCREEN_WIDTH, 2 * CHAR_HEIGHT, WHITE);
  }
  strcpy(msg, "Actionneur : ");
  drawTextAt(0, actionneurLine, msg, color);
  clearMsg(msg);
  snprintf(msg, NB_CHARS, "%s", actionneurName);
  drawTextAt(0, actionneurLine + 1, msg, color);
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

void changeActionneurIfNecessary()
{
  if (ReturnDirection() == 0)
  {
    currentActionneurIndex--;
    if (currentActionneurIndex < 0)
    {
      currentActionneurIndex = nbActionneurs - 1;
    }
    printActionneur(actionneurNames[currentActionneurIndex]);
  }
  if (ReturnDirection() == 2)
  {
    currentActionneurIndex++;
    if (currentActionneurIndex >= nbActionneurs)
    {
      currentActionneurIndex = 0;
    }
    printActionneur(actionneurNames[currentActionneurIndex]);
  }
}

void changeSeuilValueIfNecessary()
{
  if (ReturnDirection() == 0)
  {
    Serial.println("Baisse du seuil");
    capteurs[currentCapteurIndex].seuil -= SEUIL_STEP;
    // refresh seuil on screen
    printSeuil(capteurs[currentCapteurIndex].seuil, BLACK);
  }
  if (ReturnDirection() == 2)
  {
    Serial.println("Augmentation du seuil");
    capteurs[currentCapteurIndex].seuil += SEUIL_STEP;
    // refresh seuil on screen
    printSeuil(capteurs[currentCapteurIndex].seuil, BLACK);
  }
}

void flashActionneur()
{
  int last = 0;
  if (millis() - last > 500)
  {
    last = millis();
    printActionneur(actionneurNames[currentActionneurIndex], BLACK);
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