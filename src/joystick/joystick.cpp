#include "joystick.h"

bool Etat_joystick = false;

void setupJoystick()
{
    pinMode(JOYSTICK, INPUT);
    pinMode(BOUTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BOUTON), confirmAppuie, FALLING);
}

ICACHE_RAM_ATTR void confirmAppuie()
{
    Etat_joystick = true;
}

bool ReadClick()
{
    bool value = Etat_joystick;
    if (Etat_joystick == true)
        Etat_joystick = false;
    return value;
}

int ReadValueJoystick()
{
    return analogRead(JOYSTICK);
}

short int ReturnDirection()
{
    short int direction = 1;
    if (ReadValueJoystick() <= 200)
        direction = 0;
    else if (ReadValueJoystick() >= 800)
        direction = 2;

    return direction;
}