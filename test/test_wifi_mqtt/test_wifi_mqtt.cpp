/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <unity.h>
#include <string.h>

// Update these with values suitable for your network.

const char *ssid = "theo";
const char *password = "testWifi";
const char *mqtt_server = "broker.emqx.io";
const char *topicIn = "/YNOV_BDX/Theo/TestUnit";  // celui que l'on reçoit
const char *topicOut = "/YNOV_BDX/Theo/TestUnit"; // celui que l'on envoie
char *msgToSend = "test";                         // message à envoyer => modifiable dans le code
char *topicReaded = "none";
char *msgReaded = "none";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void setup_wifi()
{

    delay(10);
    // We start by connecting to a WiFi network
    UnityPrint("");
    UnityPrint("Connecting to ");
    UnityPrint(ssid);

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
    strcpy(topicReaded, topic);
    TEST_MESSAGE(topicReaded);

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        strcat(msgReaded, (char *)payload[i]);
    }
    Serial.println();

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
        digitalWrite(BUILTIN_LED, LOW); // Turn the LED on (Note that LOW is the voltage level
                                        // but actually the LED is on; this is because
                                        // it is active low on the ESP-01)
    }
    else
    {
        digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off by making the voltage HIGH
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

// FONCTION TEST UNITAIRE

void test_mqtt(void)
{
    // envoie de "test"
    client.publish(topicOut, msgToSend);
    // lecture, égal à true si = test
    delay(2000);
    bool result;
    // verification du fonctionnement

    if ((strcmp(topicReaded, topicIn) == 0) /*&& (strcmp(msgReaded, msgToSend) == 0)*/)
    {
        result = true;
    }
    else
        result = false;
    // verification test
    TEST_ASSERT_EQUAL(true, result);
}

void setup()
{
    delay(2000);
    pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    UNITY_BEGIN();
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
        client.publish("/YNOV_BDX/Theo", msg); // Fonction qui envoie au MQTT
        RUN_TEST(test_mqtt);
    }
}