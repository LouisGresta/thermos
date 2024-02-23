#include "DHTSensor.h"

const int DHTSensor::dhtPin = 2;
DHT DHTSensor::dht(DHTSensor::dhtPin, DHT11);

void DHTSensor::setupDHT() {
  dht.begin();
}

void DHTSensor::readData(float& temperature, float& humidity) {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
}