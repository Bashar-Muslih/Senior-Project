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
  Braccio.ServoMovement(20,         90, 155, 180, 35, 90, 10);
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
  initial();
    //Given X, and Y from the camera, we calculate the angle needed for our arm
  //to rotate to allow our arm to grab the can. 
  float X, Y, T_rotation, Rotation, pi, x, ang1, ang2, ang3;//These values are given to us from our camera
  float coord[1]; //Array with 2 values. Intended for [x, y].
  //The base is initially at 90 degrees, using this we can see that if X and Y
  //are positive therefore we are in Quadrant 1. If X is negative and Y is positive
  //we are in Quadrant 2.
  pi = 3.14;//Pi needs to be defined.
  
  if (Serial.available() > 0) { //Check if the Arduino has received data. Returns number of bytes already arrived and stored in the receive buffer.
    for(int i = 0; i < 2; i++){ //Two iterations: one for 'x', one for 'y'
      String data = Serial.readStringUntil('\n'); //Define 'data' as the string read. Sender uses '\n' to separate data.
      Serial.println(data); //Print value
      coord[i] = data.toFloat(); //Convert 'data' to a float and store in 'coord[i]'
    }
  }
  //Assign received coordinates:
  //X = coord[0];
  //Y = coord[1];
  
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
  if (distance > 25; x++){
    ang1 = 155 + x;
    ang2 = 180 - x;
    ang3 = 35 + x;
    
    Braccio.ServoMovement(20,         Rotation, ang1, ang2, ang3, 90, 10);//Compiles but not sure if it works
    //need to glue the ultrasonic sensor
  }
  //With our arm moving towards the soda can we now need some way to close hand of the arm.
  //To do this we need to use ultrasonic sensor information.
  if (distance == 25){
    //This distance should be measured to be sure that the soda can is within the hand of the Braccio.
    Braccio.ServoMovement(20,         Rotation, ang1, ang2, ang3, 90, 73);//73 Value means the gripper closes, this
    //should be strong enough to grasp the soda can when its standing upright.
  }
  //With the soda can within the hand we must now move it to a box on the side to store it.
  //This should be relatively simple as we are do not need to be super sensitive with it,
  //we simply need to rotate the base 180, and then drop it in the box.
  Braccio.ServoMovement(20,         180, 90, 45, 135, 90, 73);//This should rotate our soda can to the left as our angles are flipped
  delay(500);//Half second delay
  Braccio.ServoMovement(20,         180, 90, 45, 135, 90, 10);//This should drop our soda can
  //*************** THIS SHOULD BE THE END OF OUR CODE ***********************//
  //From here we just need to test on the arm and check to make sure everything runs the way we want it to
}
