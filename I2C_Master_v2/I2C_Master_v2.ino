// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <Wire.h>

void setup() {
  //Wire.begin();        // join i2c bus (address optional for master)
  Wire.begin(0,2);
  Serial.begin(115200);  // start serial for output
  delay(100);
  Serial.print("\nMaster Ready");
  

}

void loop() {
  communication();
  delay(500); // Add a delay before sending the next command
}

void communication() {
  Wire.beginTransmission(8); // Address of the slave
  Wire.write("A"); // Send a command (character 'A' in this example)
  Wire.endTransmission();
  Serial.print("\nTried sending A");
}
