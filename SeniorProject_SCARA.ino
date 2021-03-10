#include<Servo.h>

#define Servo Joint_1;
#define Servo Joint_2;
#define Servo Joint_3;
#define Servo Joint_4;

void setup() {
  //Declaring position variable for servos
  //Need to physically set servos to 0 degrees
  int pos_j1 = 0;
  int pos_j2 = 0;
  int pos_j3 = 0;
  int pos_j4 = 0;

  //Serial Monitor
  Serial.begin(9600);
  Serial.println("");
  delay(1000);
}

void loop() {

}
