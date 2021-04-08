/*
    ****INCOMPLETE****
  
    Needs to be integrated into the code for the Arm; not independent code.
  
    @April 7, 2021
        Copy and pasted
        Reading 'Serial' documentation for Arduino
        https://www.arduino.cc/reference/en/language/functions/communication/serial/
    
*/
void setup() {
      /*
        Passes the value 9600 to the speed parameter.
        This tells the Arduino to get ready to exchange messages with the Serial Monitor at a data rate of 9600 bits per second.
      */
      Serial.begin(9600);
}
void loop() {
    float coord[1]; //Array with 2 values. [x, y]
    if (Serial.available() > 0) { //Check if the Arduino has received data. This will give you the number of bytes already arrived and stored in the receive buffer.
        for(int i = 0; i < 2; i++){ //Two iterations: one for 'x', one for 'y'
            String data = Serial.readStringUntil('\n'); //Define 'data' as the string read.
            Serial.println(data); //Print 'data'
            coord[i] = data.toFloat(); //Convert 'data' to a float
        }
    }
}
