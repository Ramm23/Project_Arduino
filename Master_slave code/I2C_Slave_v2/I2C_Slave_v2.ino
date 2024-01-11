// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire.begin(8);  // join i2c bus with address #8
  Serial.print("Wire.onRequest(requestEvent);");
  Serial.print("Wire.onReceive(receiveEvent);");
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.print("\nSlave Ready");
}

void loop() {
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  //Wire.write("hello \n"); // respond with message of 6 bytes
  // as expected by master
}

void receiveEvent() {
  // Receive and process the command from the master
  while (Wire.available()) {
    char command = Wire.read();
    Serial.print(command, DEC);
    Serial.print("\n");
    // Perform an action based on the received command
    if (command == "A") {
      // Your action for command 'A'
      Serial.println("Received command 'A'. Performing action.");
      // Add your code here to perform a specific action when 'A' is received
    }
    // Add more conditions for other commands if needed
  }
}