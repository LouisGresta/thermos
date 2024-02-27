#include "MotorController.h"


MotorController::MotorController() :
  ssid(ssid),
  password(password),
  mqtt_server(mqtt_server),
  mqtt_topic(mqtt_topic),
  frequency(500),
  pwm_channel(0),
  resolution(8),
  client(espClient)
{
}

void MotorController::setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  Serial.begin(BAUD);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  ledcSetup(pwm_channel, frequency, resolution);
  ledcAttachPin(ENA, pwm_channel);
}

void MotorController::setup_wifi() {
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

void MotorController::callback(char* topic, byte* payload, unsigned int length) {
  String receivedPayload = "";
  for (int i = 0; i < length; i++) {
    receivedPayload += (char)payload[i];
  }

  int pwm_value = strcmp(topic, mqtt_topic);
  if(pwm_value == 0){
    int pwmValue = receivedPayload.toInt();
    changeSpeed(pwmValue);
  }
}

void MotorController::reconnect() {
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

void MotorController::loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void MotorController::setDirection(int sens) {
  if (sens == 0) {
    ledcWrite(pwm_channel, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (sens == 1) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}

void MotorController::changeSpeed(int pwmValue) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  ledcWrite(pwm_channel, pwmValue);
}


