# Senior-Project

Project Name: RecyloArm
Project Desc: We have built an autonomous robotic arm that can grab objects using OpenCV machine vision and ultrasonic sensors. The arm is a Braccio arm kit that is hooked up to an Arduino board, as well as an Ultrasonic sensor that is hooked to the same board. The Arduino board is connected to a Rasberry PI that has a camera connected to it.

Process: 
1- The Camera (using OpenCV) identifies an object and takes a picture to it
2- It  sends the coordiantes of the object to the arm
3- The arm starts turning towards the object and assumes a grabbing position
4- Using the ultrasonic sensor attached to the arm the arm detects with the object is directly in front of it
5- The arm moves in to grab and using the ultrasonic sensor it knows when it's close enough and closes its gripper
6- The arm then lifts the object and carries it to a programmable location
