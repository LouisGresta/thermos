#include <Arduino.h>
#include "wifi_mqtt/wifi_mqtt.h"
#ifdef OLED
#include "afficheur/afficheur.h"
#endif

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;

// Function prototypes
void callback(char *topic, byte *payload, unsigned int length);
void clearMsg(int start = 0, int size = MSG_BUFFER_SIZE);

void setup() {
  Serial.begin(115200);

  uint16_t time = millis();
  time = millis() - time;
  delay(500);
  
  Serial.begin(115200);
  setup_wifi();
  setup_mqtt(callback);
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

  unsigned long now = millis();
  if (now - lastMsg > 2000)
  {
    // lastMsg = now;
    // ++value;
    // snprintf(msg, MSG_BUFFER_SIZE, "envoie #%ld", value);
    // Serial.print("Publish message: ");
    // Serial.println(msg);
    // client.publish(topicOut, msg); // Fonction qui envoie au MQTT
  }
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
  // Print message on OLED Screen
  tft.fillScreen(BLACK);
  delay(500); // le temps que l'écran se vide
  snprintf(msg, MSG_BUFFER_SIZE, "%s", (char *) payload);
  Serial.print("Message printed on OLED : ");
  Serial.println(msg);
  clearMsg(length);
  drawTextAt(0, 0, msg, WHITE);
  #endif
}

void clearMsg(int start, int size)
{
  for (int i = start; i < MSG_BUFFER_SIZE; i++)
  {
    msg[i] = '\0';
  }
}

