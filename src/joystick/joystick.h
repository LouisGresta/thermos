#include <Arduino.h>

#define BOUTON 12
#define JOYSTICK A0

extern bool Etat_joystick;

void setupJoystick(void);

bool ReadClick(void);

void confirmAppuie(void);

int ReadValueJoystick();

short int ReturnDirection();
