//RecycloArm
//Jose Bernal-Mancilla, Bashar Muslih
//Sac State ECS
//2021

#include <Braccio.h>
#include <Servo.h>
#include <NewPing.h>


Servo base; //M1
Servo shoulder; //M2
Servo elbow; //M3
Servo wrist_rot; //M4
Servo wrist_ver; //M5
Servo gripper; //M6

//Sonar GPIO pins
#define echo 8
#define trig 9

void setup() {  
  //Begin serial monitor.
  Serial.begin(9600);
  
  //Initialize Ultrasonic.
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);

  //Initialize Braccio
  Braccio.begin();
}

void loop(){
  Braccio.ServoMovement(20,           90,  15, 180, 170, 90,  73);//initial position for the arm
  float Rotation;//defining the base angle of the arm
  float ang1, ang2, ang3;//defining angles for parts of the arm
  Rotation = 90;//sets base variable to 90'
  Braccio.ServoMovement(20,         Rotation, 80, 180, 115, 90, 10);//moving the arm to a position that's ready to grab
  
  float coord[2]; //Array with 2 values. [x, y]
  float X, Y;
  int i = 0;
  bool coordTransfer = false;

  while(coordTransfer == false){    
    if (Serial.available() > 0) { //Check if the Arduino has received data. This will give you the number of bytes already arrived and stored in the receive buffer.
      Serial.flush();
      while(Serial.available()){
        delay(1000);
        coord[i] = Serial.parseFloat();

        Serial.print("coord[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(coord[i]);
        
        i++;
        if (i == 2){
          coordTransfer = true;
        }
      }
    }
  }
  delay(1000);
  X = coord[0];
  Y = coord[1];


//The base is initially at 90 degrees, using this we use X coordinate we find using the camera.
//The picture has origin is at the top right, meaning the X goes from 0 ------> 640 px
//To rotate that means when X < 320 turn left, when X > 320 turn right  

//Initializing ultrasonic sensor
  digitalWrite(trig, LOW);
  delayMicroseconds(1000);
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  delayMicroseconds(1000);
  //Next we need to define the distance it will be using.
  float distance = pulseIn(echo, HIGH);
  distance = (distance * .0343)/2; //duration times the speed of light cm/us
  Serial.print('\n');
  Serial.print("Distance: ");
  Serial.print(distance);
  delay(1000);

  boolean found;
  found = false;
  
//We want to turn until the ultrasonic senses something, for this we set a certain distance matching our
//work space. 
  while (found == false){//the arm will loop until it detects something
    Rotation = 90;//if the loop ends at it doesn't find anything, it will reset to inital base position and starts over
  if (X < 320){//since the picture is 640 px wide, 320 is the center. If X is more than 320 that means it's on the right
    Serial.print("Bottle is on my right");
    Serial.print('\n');
    while (distance > 35 && Rotation < 180){//The arm is in a loop of moving and detecting, and will continue until it finds something or reaches the max range of movement
      Braccio.ServoMovement(20,         Rotation, 80, 180, 115, 90, 10);
      distance = pulseIn(echo, HIGH);
      distance = (distance * .0343)/2; //duration times the speed of light cm/us
      Serial.print('\n');
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print('\n');
      Serial.print("Rotation: ");
      Serial.print(Rotation);
      delay(1000);
      Rotation = Rotation + 3;
      if (distance <= 35)//if it detects something, it exists the loop
          Rotation = Rotation + 2;
          found = true;
      }
      found = true;
  }
//
  else if (X > 320){//since the picture is 640 px wide, 320 is the center. If X is less than 320 that means it's on the left
    Serial.print("Bottle is on my left");
    Serial.print('\n');
    while (distance > 35 && Rotation > 0){//The arm is in a loop of moving and detecting, and will continue until it finds something or reaches the max range of movement
      Braccio.ServoMovement(20,         Rotation, 80, 180, 115, 90, 10);
      distance = pulseIn(echo, HIGH);
      distance = (distance * .0343)/2; //duration times the speed of light cm/us
      Serial.print('\n');
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print('\n');
      Serial.print("Rotation: ");
      Serial.print(Rotation);
      delay(1000);
      Rotation = Rotation - 3;
      if (distance <= 35)//if it detects something, it exists the loop
          Rotation = Rotation - 2;
          found = true;
      }
      found = true;
  }
  else{
      Serial.print("Bottle is in front of me");//if the object is neither on left or right, it's in the front
      Serial.print('\n');
      found = true;
  }
  }
  //This part of the code starts when the object detects something in front of it
  //This is the grabbing process
    ang1 = 80;
    ang2 = 180;
    ang3 = 115;
    Serial.print('\n');
    Serial.print("I'm moving in closer to grab the bottle");
  while (distance >= 6 & ang1 < 165 & ang2 > 0 & ang3 > 0){//if the item isn't close enough we move up the arm a bit without breaking limits
    ang1 = ang1 + 1; 
    ang2 = ang2 - 1;
    ang3 = ang3 - .2;
    Braccio.ServoMovement(20,         Rotation, ang1, ang2, ang3, 90, 10); //moves the arm towards the item
    distance = pulseIn(echo, HIGH); //calculates the new distance
    distance = (distance * .0343)/2; //calculates the new distance
  }

  //This part of the code starts when the arm has the item in its claw, and may close it and move it
  //With our arm moving towards the soda can we now need some way to close hand of the arm.
  
    Serial.print('\n');
    Serial.print("I'm closing my claw and throwing the bottle away");
    //This distance should be measured to be sure that the soda can is within the hand of the Braccio.
    Braccio.ServoMovement(20,         Rotation, ang1, ang2, ang3, 90, 43);//43 Value means the gripper closes, this
    //should be strong enough to grasp the soda can when its standing upright.
    Braccio.ServoMovement(20,         Rotation, 110, ang2, ang3,  90, 43);
    delay(1000);
    Braccio.ServoMovement(20,         Rotation, 80, 180, 90, 90,  43);
    delay(1000);
    Braccio.ServoMovement(20,         180,      80, 180, 90, 90,  43);
    delay(1000);
    Braccio.ServoMovement(20,         180,      80, 180, 90, 90,  10);//10 means the gripper is open and it drops the item

  
//With the soda can within the hand we must now move it to a box on the side to store it.
//This should be relatively simple as we are do not need to be super sensitive with it,
//we simply need to rotate the base 180, and then drop it in the box.
  
}
