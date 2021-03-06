// Arduino/Teensy example for Arduino Braccio

// Include the library InverseK.h
#include "InverseK.h"
#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

void setup() {
  // Setup the lengths and rotation limits for each link of the arm
  Link base, upperarm, forearm, hand;

  Braccio.begin();

  base.init(0, b2a(0.0), b2a(180.0));
  upperarm.init(200, b2a(15.0), b2a(165.0));
  forearm.init(200, b2a(0.0), b2a(180.0));
  hand.init(270, b2a(0.0), b2a(180.0));

  // Attach the links to the inverse kinematic model
  InverseK.attach(base, upperarm, forearm, hand);

  float a0, a1, a2, a3;

  // InverseK.solve() return true if it could find a solution and false if not.

  // Calculates the angles without considering a specific approach angle
  // InverseK.solve(x, y, z, a0, a1, a2, a3)
  if(InverseK.solve(550, 0, 50, a0, a1, a2, a3)) {
    Braccio.ServoMovement(20, a0, a1, a2, a3, 0, 0); //need to move it to void loop()
  } else {
    Serial.println("No solution found!");
  }
}

void loop() {
    
}

// Quick conversion from the Braccio angle system to radians
float b2a(float b){
  return b / 180.0 * PI - HALF_PI;
}

// Quick conversion from radians to the Braccio angle system
float a2b(float a) {
  return (a + HALF_PI) * 180 / PI;
}
