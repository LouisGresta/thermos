#include "ControleurH/controleur.h"

extern int seuil_Temp = 0;

void setup_servo(Servo *monServo)
{
    // Servomottor setup 
  monServo -> attach(PinSevo);
  monServo -> write(POS_INITIALE);
}

//Fonction qui permet de retourner de comparer les topics 
// Pour la partie node-red 
int compareTopics(const char* topic, const char* Topic_C) {
    int result = strcmp(topic, Topic_C);
    
    if (result == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// Comparaison pour la seuil
int compareTopicsSeuilTemp(const char* topic) {
    int result = strcmp(topic, Topic_SeuilTemp_Mqtt);
    
    if (result == 0) {
        return true;
    } else {
        return false;
    }
}

int recupSeuil(const char* topic, String s)
{
    int result = strcmp(topic, Topic_SeuilTemp_Mqtt);

    if (result == 0) 
    {
        int status = s.toInt();
        seuil_Temp = status;
    } 
}


//Fonction qui permet de set le servo-moteur en fonction 
// de la partie node-red
// 2 modes : temperature > ou < au seuil
int SetservoMoteur(int flag, String s, Servo *monServo) {

    int pos = POS_FINALE;
    
        if (flag == TRUE) 
        {
            int status = s.toInt();

            if (status >= seuil_Temp)
            {
                Serial.print("humidity seuil : ");
                Serial.println(pos);

                monServo -> write(pos);
                //delay(10);
            } else if (status <= seuil_Temp) 
            {
                pos = POS_INITIALE;
                Serial.print("humidity seuil : ");

                Serial.println(pos);
                monServo -> write(pos);
                delay(TEN);
            }
        }
    
    return pos;
}