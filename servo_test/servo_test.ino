/*
Modified September 19, 2020
by Michael Denissov
*/
#define SERIAL_BUFFER_SIZE 256
#include <Servo.h>

Servo steering; // Servo object to control the steering servo

int incomingByte = 0;     // For storing incoming serial data
int pos = 0;              // Variable to store the servo position
bool start_read = false;  // Start reading serial flag

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); // Opens serial port, sets data rate to 115200 bps
  steering.attach(3);   // Attaches the servo on pin 3 to the servo object
  steer(90);            // Set steering straight
}

void loop() {
  
  // Do something only when you receive data:
  if (Serial.available() > 0) {
      
    // Read input from serial for steering if read flug is true
    if (start_read == true){
      if (incomingByte == '\n') {
        start_read = false;
        //Serial.println(start_read);
      } else {
        //Serial.println("reading");
        int val = Serial.parseInt();
        //int val = (int)incomingByte;
        //Serial.println((char)incomingByte);
        steer(val);
      }
    }
    
    // Read the incoming byte:
    incomingByte = Serial.read();
       
    
    // Check if command is to steer the servo
    if ((char)incomingByte == '~'){
      //Serial.println("printing");
      start_read = true; 
    }
    
  }

}

// Function to steer the servo
void steer(int goal) {
  Serial.print("steering to:"); // Debug prints
  Serial.println(goal);

  // Testing shows that 120 and 45 are the max and min range of the servo
  if (goal > 120){
    goal = 120;
  } else if (goal < 45) {
    goal = 45;
  }

  // Steer
  steering.write(goal);
}
