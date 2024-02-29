
#include "MoteurH/moteur.h"


// Pour la partie node-red 
int compareTopicsM(const char* topic) {
    int result = strcmp(topic, Topic_Led_Mqtt);
    
    if (result == 0) {
        return true;
    } else {
        return false;
    }
}

// Pour la partie node-red 
int compareTopicsMod(const char* topic) {
    int result = strcmp(topic, Topic_Mod_Mqtt);
    
    if (result == 0) {
        return true;
    } else {
        return false;
    }
}

// Pour la partie node-red 
int compareTopicsSens(const char* topic) {
    int result = strcmp(topic, Topic_Sens_Mqtt);
    
    if (result == 0) {
        return true;
    } else {
        return false;
    }
}

void setupLed()
{
    pinMode(LedMoteur, OUTPUT);
}

//Pour la led
void MoteurLed(int flag, String data)
{

    if (flag == 1) 
    {
        /*Serial.print("dataLed : ");
        Serial.println(data);*/

        if (data == "true") 
        {
            Serial.println("LED");
            //digitalWrite(LedMoteur, HIGH);
        }
        else 
        {
            Serial.println("LED Non");
            //digitalWrite(LedMoteur, LOW);
        }
    }
}

//Pour le mode
void MoteurMode(int flag, String data)
{
    if (flag == 1) 
    {
        /*Serial.print("dataLed : ");
        Serial.println(data);*/

        if (data == "true") 
        {
            Serial.println("Mode Auto");
        }
        else 
        {
            Serial.println("Mode Manu");
        }
    }
}

//Pour le sens
void MoteurSens(int flag, String data)
{
    if (flag == 1) 
    {
        /*Serial.print("dataLed : ");
        Serial.println(data);*/

        if (data == "true") 
        {
            Serial.println("sens normal");
        }
        else 
        {
            Serial.println("sens inverse");
        }
    }
}