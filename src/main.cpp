#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <Arduino.h>
#include "joystick.h"
#include <Wire.h>

// Update these with values suitable for your network.

const char *ssid = "Projet_IoT";
const char *password = "MdpTresSafe01";
const char *mqtt_server = "broker.emqx.io";
const char *topicIn = "/YNOV_BDX/Theo/TestJ";  // celui que l'on reçoit
const char *topicOut = "/YNOV_BDX/Theo/TestJ"; // celui que l'on envoie

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value_m = 0;

void setUp(void)
{
  // set stuff up here
}

void loop()
{
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

void drawTextAt(int x, int y, const char *text, uint16_t color)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.print(text);
}

#ifdef TEST
void tftPrintTest()
{
  tft.fillScreen(BLACK);
  tft.setCursor(0, 5);
  tft.setTextColor(RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(BLUE);
  tft.setTextSize(3);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 5);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(GREEN);
  tft.print("3.14");
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(WHITE);
  tft.print(" seconds.");
}
#endif

void testlines(uint16_t color)
{
  tft.fillScreen(BLACK);
  for (uint16_t x = 0; x < tft.width() - 1; x += 6)
  {
    tft.drawLine(0, 0, x, tft.height() - 1, color);
  }
  for (uint16_t y = 0; y < tft.height() - 1; y += 6)
  {
    tft.drawLine(0, 0, tft.width() - 1, y, color);
  }

  tft.fillScreen(BLACK);
  for (uint16_t x = 0; x < tft.width() - 1; x += 6)
  {
    tft.drawLine(tft.width() - 1, 0, x, tft.height() - 1, color);
  }
  for (uint16_t y = 0; y < tft.height() - 1; y += 6)
  {
    tft.drawLine(tft.width() - 1, 0, 0, y, color);
  }

  tft.fillScreen(BLACK);
  for (uint16_t x = 0; x < tft.width() - 1; x += 6)
  {
    tft.drawLine(0, tft.height() - 1, x, 0, color);
  }
  for (uint16_t y = 0; y < tft.height() - 1; y += 6)
  {
    tft.drawLine(0, tft.height() - 1, tft.width() - 1, y, color);
  }

  void setup_wifi()
  {

    delay(10);
    // We start by connecting to a WiFi network
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
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
  }

  void reconnect()
  {
    // Loop until we're reconnected
    while (!client.connected())
    {
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str()))
      {
        Serial.println("connected");
        // Once connected, publish an announcement...
        client.publish(topicOut, "Début test envoie");
        // ... and resubscribe
        client.subscribe(topicIn);
      }
      else
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }

  void setup()
  {
    setupJoystick();
    delay(2000);
    pinMode(LED_BUILTIN, OUTPUT); // Initialize the BUILTIN_LED pin as an output
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
  }

  void loop()
  {

    if (!client.connected())
    {
      reconnect();
    }
    client.loop();

    unsigned long now = millis();
    if (now - lastMsg > 2000)
    {
      lastMsg = now;
      ++value_m;
      snprintf(msg, MSG_BUFFER_SIZE, "value : %d", ReturnDirection());
      Serial.print("Publish message:");
      bool valeur = ReadClick();
      Serial.println(valeur);
      if (valeur == true) // il y a eu un appuie
      {
        client.publish(topicOut, "il y a eu un appuie"); // Fonction qui envoie au MQTT
      }

      client.publish(topicOut, msg); // Fonction qui envoie au MQTT
    }
  }