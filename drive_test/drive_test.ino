/*
Modified September 19, 2020
by Michael Denissov
*/
#define SERIAL_BUFFER_SIZE 256
#include <Servo.h>

Servo main_motor; // Servo object to control the main motor
                  // The ESC behaves close enough to a servo to use like a servo

int incomingByte = 0;     // For storing incoming serial data
bool start_read = false;  // Start reading serial flag
int sped = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);   // Opens serial port, sets data rate to 115200 bps
  main_motor.attach(2);   // Attaches the motor on pin 2 to the servo object
  sped = drive(0, 0);               // Set drive to 0
}

void loop() {
  
  // Do something only when you receive data:
  if (Serial.available() > 0) {
      
    // Read input from serial for drive if read flag is true
    if (start_read == true){
      if (incomingByte == '\n') {
        start_read = false;
        //Serial.println(start_read);
      } else {
        //Serial.println("reading");
        int val = Serial.parseInt();
        //int val = (int)incomingByte;
        //Serial.println((char)incomingByte);
        if (val == 0){
          sped = drive(0, sped);
        } else {
          sped = drive(map(val, 0, 1000, 1700, 2150), sped);
        }
      }
    }
    
    // Read the incoming byte:
    incomingByte = Serial.read();
       
    
    // Check if command is to drive the main motor
    if ((char)incomingByte == '~'){
      //Serial.println("printing");
      start_read = true; 
    }
    
  }

}

// Function to drive the main motor
int drive(int goal, int c_speed) {
  int modifier = 1;
  Serial.print("power to:"); // Debug prints
  Serial.println(goal);

  // Testing shows that 1480 is stop, and about 1700 is min speed
  if (goal == 0){
    c_speed = 1480;
    main_motor.writeMicroseconds(c_speed);
  } else {
    // For going down or up in speed
    if (goal >= c_speed){
      modifier = 1;
    } else {
      modifier = -1;
    }
    // Drive
    main_motor.writeMicroseconds(goal);
    for (int pos = c_speed; pos != goal; pos += modifier) {
      // in steps of 1 degree
      Serial.println(pos);
      main_motor.writeMicroseconds(pos);             
      delay(2);
      c_speed = pos;
    }  
  }
  return c_speed;
}
