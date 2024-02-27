#include <Arduino.h>
#include <ESP8266WiFi.h> //Importe cette librairie
#include <PubSubClient.h>
#include <string.h>

#define ENA D7
#define IN1 D6
#define IN2 D5

const char* ssid = "Galaxy S22 UltraDC7F";
const char* password = "oyyy2060";
const char* mqtt_server = "broker.emqx.io";
const char* mqtt_topic = "/YNOV_BDX/servo";

const int frequency = 500;
const int pwm_channel = 0;
const int resolution = 8;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  ledcSetup(pwm_channel, frequency, resolution);
  ledcAttachPin(ENA, pwm_channel);
  motor.setup();
}


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String receivedPayload = "";
  for (int i = 0; i < length; i++) {
    receivedPayload += (char)payload[i];
  }
  //récupére les informations depuis node-red
  int pwm_value = strcmp(topic,"/YNOV_BDX/servo" );
  int pwm_sens = strcmp(topic, "/YNOV_BDX/servos_sens");
  if(pwm_value == 0){
    int pwmValue = receivedPayload.toInt();
    changeSpeed(pwmValue);
  }
  if (pwm_sens ==  0)
  {
    setDirection(pwm_sens);
  }

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //motor.loop();
}

void setDirection(int sens) {
  if (sens == 0) {
    ledcWrite(pwm_channel, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (sens ==1) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}

void changeSpeed(int pwmValue) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  ledcWrite(pwm_channel, pwmValue);
}
