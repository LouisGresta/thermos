#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <DHT.h>

class DHTSensor {
public:
  static void setupDHT();
  static void readData(float& temperature, float& humidity);

private:
  static const int dhtPin;
  static DHT dht;
};

#endif