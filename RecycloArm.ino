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
#define echo 12
#define trig 13

void setup() {  
  //Begin serial monitor.
  Serial.begin(9600);
  
  //Initialize Ultrasonic.
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);

  //Initialize Braccio
  Braccio.begin();
}

void initial(){
  Braccio.ServoMovement(20,         90, 45, 180, 180, 90, 73);
  //Angles needed to set our manipulator into the initial position
  //This position is stable for when we rotate our base.
}

void grabbing(){
  Braccio.ServoMovement(20,         90, 155, 180, 35, 90, 73);
  //Angles needed to set our manipulator in to the grabbing position
  //When in this position the end effector is approx 448.35 mm away from the base.
}

void rotation(){
  //Given X, and Y from the camera, we calculate the angle needed for our arm
  //to rotate to allow our arm to grab the can. 
  float X, Y, T_rotation, Rotation, pi;//These values are given to us from our camera
  //The base is initially at 90 degrees, using this we can see that if X and Y
  //are positive therefore we are in Quadrant 1. If X is negative and Y is positive
  //we are in Quadrant 2.
  pi = 3.14;//Pi needs to be defined.
  //Dummy position.
  X = 351; 
  Y = 971;
  Serial.println(X);
  Serial.println(Y);
  if (X > 0 && Y > 0){
    T_rotation = asin(X/sqrt(pow(X,2) + pow(Y,2)));//This T_rotation is given in rads
    T_rotation = T_rotation * 180/pi;//We convert this to degrees to allow
    //us to subtract from our initial rotation.
    //As we are at 90 degrees in the intial position we therefore have to subtract
    //this T_rotation from 90 degrees.
    Rotation = 90 - T_rotation;//This value is what we will put into our M1 input.
  }
  else if (X < 0 && Y > 0){
    T_rotation = asin(X/sqrt(pow(X,2) + pow(Y,2)));//This T_rotation is given in rads
    T_rotation = T_rotation * 180/pi;//We convert this to degrees to allow
    //us to subtract from our initial rotation.
    //As we are at 90 degrees in the intial position we therefore have to add
    //this T_rotation to 90 degrees.
    Rotation = 90 + abs(T_rotation);//This value is what we will put into our M1 input.
    //We use absolute value as we will get a negative number when dealing with negative X.
  }
  Serial.println(Rotation);//Doesnt work for some reason, need to work on this.
  //Begin rotation after doing the math.
  Braccio.ServoMovement(20,         Rotation, 45, 180, 180, 90, 73);//Doesnt work, think 
  //Rotation variable is messing it up.
}

void loop(){
    //Given X, and Y from the camera, we calculate the angle needed for our arm
  //to rotate to allow our arm to grab the can. 
  float X, Y, T_rotation, Rotation, pi, x, ang1, ang2, ang3;//These values are given to us from our camera
  //The base is initially at 90 degrees, using this we can see that if X and Y
  //are positive therefore we are in Quadrant 1. If X is negative and Y is positive
  //we are in Quadrant 2.
  pi = 3.14;//Pi needs to be defined.
  //Dummy position.
  X = -351; 
  Y = 971;
  
  if (X > 0 && Y > 0){
    T_rotation = asin(X/(sqrt(pow(X,2) + pow(Y,2))));//This T_rotation is given in rads
    T_rotation = T_rotation * 180/pi;//We convert this to degrees to allow
    //us to subtract from our initial rotation.
    //As we are at 90 degrees in the intial position we therefore have to subtract
    //this T_rotation from 90 degrees.

    //****************IMPORTANT INFORMATION*****************
    //Apparently, our axis are flipped meaning 0 degrees is to the left and 180 to the right
    
    Rotation = 90 + T_rotation;//This value is what we will put into our M1 input.
  }
  else if (X < 0 && Y > 0){
    T_rotation = asin(X/(sqrt(pow(X,2) + pow(Y,2))));//This T_rotation is given in rads
    T_rotation = T_rotation * 180/pi;//We convert this to degrees to allow
    //us to subtract from our initial rotation.
    //As we are at 90 degrees in the intial position we therefore have to add
    //this T_rotation to 90 degrees.
    Rotation = 90 + T_rotation;//This value is what we will put into our M1 input.
    //We use absolute value as we will get a negative number when dealing with negative X.
  }
  Serial.println('\n');
  Serial.println("Base Angle: ");
  Serial.println(Rotation);//Doesnt work for some reason, need to work on this.
  //Begin rotation after doing the math.
  Braccio.ServoMovement(20,         Rotation, 45, 180, 180, 90, 73);

  //For this function we need the 3 angles (M2, M3, M4) with regards to the ultrasonic sensor
  //information.
  //To have the sensor trigger we use the following:
  digitalWrite(trig, LOW);
  delayMicroseconds(100);
  digitalWrite(trig, HIGH);
  delayMicroseconds(100);
  digitalWrite(trig, LOW);
  //Next we need to define the distance it will be using.
  //Next we need to define the distance it will be using.
  float distance = pulseIn(echo, HIGH);
  distance = 50; //(distance * .0343)/2; //duration times the speed of light cm/us
  Serial.print('\n');
  Serial.print("Distance: ");
  Serial.print(distance);
  //With this we have a working ultrasonic sensor.
  //Now we need to code for detection of the soda can, that is, set the unit distance our arm
  //stop moving once a distance is detected. This lower limit is measured from the base of the
  //hand to our sensor, once this is reached the movement stops and the hand closes.
  //Was thinking this might be a way to get our loop working but its not working right now.
  if (distance >= 25){
    ang1 = 155 - x;
    ang2 = 180 - x;
    ang3 = 35 - x;
  }
}
