#include <Arduino.h>
#include <DHTesp.h>  //Librairie du Capteur temp DHT11

// put function declarations here:
int myFunction(int, int);

#define DHT_SENSOR_PIN 9 // The ESP8266 pin D7 connected to DHT11 sensor 
#define DHT_SENSOR_TYPE DHT11

DHTesp dht;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  String thisBoard= ARDUINO_BOARD;

  Serial.println(thisBoard);

  dht.setup(DHT_SENSOR_PIN, DHTesp::DHT_SENSOR_TYPE); // Connect DHT sensor to GPIO 17
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");

  Serial.print(humidity, 1);
  Serial.print("\t\t");

  Serial.print(temperature, 1);
  Serial.print("\t\t");

  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");

  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");

  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
  delay(2000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}