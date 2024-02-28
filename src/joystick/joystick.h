#include <Arduino.h>

#define BOUTON 13
#define JOYSTICK A0

extern bool Etat_joystick;

void setupJoystick(void);

bool ReadClick(void);

void confirmAppuie(void);

int ReadValueJoystick();

short int ReturnDirection(); // 0 = gauche, 1 = milieu , 2 = droite
