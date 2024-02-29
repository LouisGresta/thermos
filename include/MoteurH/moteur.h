#include <Arduino.h>

#define Topic_Led_Mqtt "/YNOV_BDX/led"
#define Topic_Mod_Mqtt "/YNOV_BDX/mode"
#define Topic_Sens_Mqtt "/YNOV_BDX/sens"

int compareTopicsM(const char* topic);
void MoteurLed(int flag, String data);

int compareTopicsMod(const char* topic);
void MoteurMode(int flag, String data);

int compareTopicsSens(const char* topic);
void MoteurSens(int flag, String data);