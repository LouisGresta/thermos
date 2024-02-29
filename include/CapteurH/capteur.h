#ifndef CAPTEUR_H
#define CAPTEUR_H

#include <DHTesp.h>


#define DHT_SENSOR_TYPE DHT11
#define SEUIL_TEMP 40
#define PIN  9

void setup_Capt(DHTesp *dht);
void CptTempHum(float sensorValues[], DHTesp *dht);




#endif // CAPTEUR_H