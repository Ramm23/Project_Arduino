// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.
/*
//  Port correspondance for Wire.begin(a,b) statement
D0 - would be 16, but doesnt work for some reason??
D1 - 5 works
D2 - 4 works
D3 - 0 works
D4 - 2 works
D5 - 14 works
D6 - 12 works
D7 - 13 works
D8 - would be 15, but doesnt work for some reason??

*/

#include <Wire.h>

int i = 1;
void setup() {
  //Wire.begin();        // join i2c bus (address optional for master)
  //Wire.begin(0, 2);  // Works if A4-D3 and A5-D4
  //Wire.begin();  //works with A4-D2 and A5-D1
  //Wire.begin(0, 4);      //works with A4-D3 and A5-D2
  //Wire.begin(0, 5);      //works with A4-D3 and A5-D1
  Wire.begin(0, 13);     //works with A4-D and A5-D
  Serial.begin(115200);  // start serial for output
  delay(100);
  Serial.print("\nMaster Ready");
}

void loop() {
  communication();
  delay(500);  // Add a delay before sending the next command
}

void communication() {
  i = i + 1;
  Wire.beginTransmission(8);  // Address of the slave
  Wire.write(i);              // Send a command (character 'A' in this example)
  Wire.endTransmission();
  Serial.print("\nTried sending ");
  Serial.print(i, DEC);
}
