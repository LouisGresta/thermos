#include <Arduino.h>
#include "wifi_mqtt/wifi_mqtt.h"
#ifdef IHM        
#include "afficheur/afficheur.h"
#endif

extern PubSubClient client;
extern const char *topicOut;
extern const char *topicIn;

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;

// Function prototypes
void callback(char *topic, byte *payload, unsigned int length);

void setup() {
  Serial.begin(115200);
  setupLCD();

  uint16_t time = millis();
  time = millis() - time;
  delay(500);
  
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  setup_mqtt(callback);
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
    lastMsg = now;
    ++value;
    snprintf(msg, MSG_BUFFER_SIZE, "envoie #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(topicOut, msg); // Fonction qui envoie au MQTT
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
//  strcpy(topicReaded, topic);
//  Serial.println(topicReaded);

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
//    strcat(msgReaded, (char *)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1')
  {
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because
                                    // it is active low on the ESP-01)
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  }
  #ifdef IHM
    Serial.println("Hello");
    displayMessage("Bonjour");
  #endif
}

