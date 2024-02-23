#include <Arduino.h>
#include <Servo.h> //Servo motor
Servo myservo;  // create servo object to control a servo


int pos = 0;    // variable to store the servo position

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  myservo.attach(5); //servo motor
}

void loop() {
  // put your main code here, to run repeatedly:

  for (pos = 0; pos <= 180; pos += 5) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.print("pos : ");
    Serial.println(pos);
    delay(500);                       // waits 15 ms for the servo to reach the position
  }

  for (pos = 180; pos >= 0; pos -= 5) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.print("pos : ");
    Serial.println(pos);
    delay(500);                       // waits 15 ms for the servo to reach the position
  }
}
