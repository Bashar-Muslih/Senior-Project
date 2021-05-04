/*
  ****COMPLETE****

  Needs to be integrated into the code for the arm; not independent code.
  'Serial' documentation for Arduino:
  https://www.arduino.cc/reference/en/language/functions/communication/serial/

*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float coord[2]; //Array with 2 values. [x, y]
  float X, Y;
  int i = 0;
  bool coordTransfer = false;

  while(coordTransfer == false){
    if (Serial.available() > 2) { //Check if the Arduino has received data. This will give you the number of bytes already arrived and stored in the receive buffer.
      Serial.flush();
      while(Serial.available()){
        delay(1000);
        coord[i] = Serial.parseFloat();
        i++;
      }
    }
    if (i == 3){
      coordTransfer == true;
    }
  }
  delay(1000);
  X = coord[0];
  Y = coord[1];
  Serial.print("You sent me X = ");
  Serial.println(X);
  delay(1000);
  Serial.print("You sent me Y = ");
  Serial.println(Y);
  delay(1000);
}
