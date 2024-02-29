#include "CapteurH/capteur.h"


void setup_Capt(DHTesp *dht)
{
  dht->setup(PIN, DHTesp::DHT_SENSOR_TYPE);
}

void CptTempHum(float sensorValues[], DHTesp *dht) {
  delay(dht->getMinimumSamplingPeriod());

  float humidity = dht->getHumidity();
  float temperature = dht->getTemperature();
 
  Serial.print("Humidity : ");
  Serial.println(humidity, 1);
  sensorValues[0] = humidity;

  Serial.print("Temperature : ");
  Serial.println(temperature, 1);
  sensorValues[1] = temperature;
  Serial.print("\n");

}
