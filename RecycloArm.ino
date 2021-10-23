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
  float Rotation;
  boolean found;
  Rotation = 90;
  found = false;
  Braccio.ServoMovement(20,         Rotation, 125, 180, 60, 90, 10);
  
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

//   //Given X, and Y from the camera, we calculate the angle needed for our arm
//  //to rotate to allow our arm to grab the can. 
  float ang1, ang2, ang3;//These values are given to us from our camera
//  //The base is initially at 115 degrees, using this we use X coordinate we find using the camera.
//  //The picture has origin is at the top right, meaning the X goes from 0 ------> 640 px
//  //To rotate that means when X < 320 turn left, when X > 320 turn right  
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
//  //We want to turn until the ultrasonic senses something, for this we set a certain distance matching our
//  //work space. 
  while (found == false){
  if (X > 320){
    while (distance > 15 && Rotation < 180){
      Braccio.ServoMovement(20,         Rotation, 125, 180, 60, 90, 10);
      distance = pulseIn(echo, HIGH);
      distance = (distance * .0343)/2; //duration times the speed of light cm/us
      Serial.print('\n');
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print('\n');
      Serial.print("Rotation: ");
      //Serial.print(Rotation);
      delay(1000);
      Rotation = Rotation + 5;
      if (distance <= 15)
          found = true;
      }
  }
//
  else if (X < 320){
    while (distance > 15 && Rotation > 0){
      Braccio.ServoMovement(20,         Rotation, 125, 180, 60, 90, 10);
      distance = pulseIn(echo, HIGH);
      distance = (distance * .0343)/2; //duration times the speed of light cm/us
      Serial.print('\n');
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print('\n');
      Serial.print("Rotation: ");
      //Serial.print(Rotation);
      delay(1000);
      Rotation = Rotation - 5;
      if (distance <= 15)
          found = true;
      }
  }
  else{
      while (distance > 15){
      Braccio.ServoMovement(20,         Rotation, 125, 180, 60, 90, 10);
      distance = pulseIn(echo, HIGH);
      distance = (distance * .0343)/2; //duration times the speed of light cm/us
      Serial.print('\n');
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print('\n');
      Serial.print("Rotation: ");
      //Serial.print(Rotation);
      delay(1000);
      if (distance <= 15)
          found = true;
      }
  }
  }
//  //Begin rotation after doing the math.
//  Braccio.ServoMovement(20,         90, 145, 180, 40, 90, 0);
//  delayMicroseconds(1000);//Experimental delay to make the robot do things in steps
//  //not in one single movement.
//  //----------Think after this part is where the problem arises-------------//
//
//  //For this function we need the 3 angles (M2, M3, M4) with regards to the ultrasonic sensor
//  //information.
//  //To have the sensor trigger we use the following:
//
//  //With this we have a working ultrasonic sensor.
//  //Now we need to code for detection of the soda can, that is, set the unit distance our arm
//  //stop moving once a distance is detected. This lower limit is measured from the base of the
//  //hand to our sensor, once this is reached the movement stops and the hand closes.
    ang1 = 125;
    ang2 = 180;
    ang3 = 60;
  while (distance > 8 & ang1 < 165 & ang2 > 0 & ang3 < 180){

    ang1 = ang1 + 1; //if the item isn't close enough we move up the arm a bit
    ang2 = ang2 - 1;
    //ang3 = ang3 + 1;
    Braccio.ServoMovement(20,         Rotation, ang1, ang2, 60, 90, 10); //moves the arm towards the item
    distance = pulseIn(echo, HIGH); //calculates the new distance
    distance = (distance * .0343)/2; //calculates the new distance
  }

    Braccio.ServoMovement(20,         Rotation, ang1, ang2, 60, 90, 73);

  //With our arm moving towards the soda can we now need some way to close hand of the arm.
  //To do this we need to use ultrasonic sensor information.
  if (distance <= 8){
    //This distance should be measured to be sure that the soda can is within the hand of the Braccio.
    Braccio.ServoMovement(20,         Rotation, ang1, ang2, 60, 90, 63);//63 Value means the gripper closes, this
    //should be strong enough to grasp the soda can when its standing upright.
    Braccio.ServoMovement(20,         180, 90, 180, 90, 90, 63);//This should rotate our soda can to the left as our angles are flipped
    Braccio.ServoMovement(20,         180, 90, 180, 90, 90, 10);
  }
//  //With the soda can within the hand we must now move it to a box on the side to store it.
//  //This should be relatively simple as we are do not need to be super sensitive with it,
//  //we simply need to rotate the base 180, and then drop it in the box.
  
//  delay(500);//Half second delay
//  Braccio.ServoMovement(20,         270, 90, 180, 90, 90, 0);//This should drop our soda can
//  //*************** THIS SHOULD BE THE END OF OUR CODE ***********************//
//  //From here we just need to test on the arm and check to make sure everything runs the way we want it to
}
