// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Serial.begin(115200);
  
  Serial.print("\nSlave Ready");

}

void loop() {
  delay(100);
  Wire.onRequest(requestEvent); // register event

}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write("\nhello "); // respond with message of 6 bytes
  // as expected by master
}
