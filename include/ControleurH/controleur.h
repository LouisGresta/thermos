#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <Servo.h>

#define POS_INITIALE 0
#define POS_FINALE   180


#define Topic_SeuilTemp_Mqtt "/YNOV_BDX/Projet_IoT/Actionneurs/SOUK03"

#define PinSevo 5

#define TEN 10
#define TRUE 0
#define FALSE 1


void setup_servo(Servo *monServo);
int compareTopics(const char* topic, const char* Topic_C);

int compareTopicsSeuilTemp(const char* topic);
int SetservoMoteur(int flag, String s,  Servo *monServo);


#endif // CONTROLEUR_H