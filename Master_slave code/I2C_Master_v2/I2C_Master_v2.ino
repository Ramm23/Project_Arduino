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
  Wire.begin(D2, D1);    //works with D2-A4 and D1-A5
  Serial.begin(115200);  // start serial for output
  delay(100);
  Serial.print("\nMaster Ready");
}

void loop() {
  communication_send_M(); //sending data from master to slave
  delay(500);
  communication_receive_M();  //receiving data on master from slave
  delay(500);  // Add a delay before sending the next command
}

void communication_send_M() {

  Wire.beginTransmission(8);  // Address of the slave
  Wire.write("master-slave");            // Send a command (character 'A' in this example)
  Wire.endTransmission();

}

void communication_receive_M() {
  Wire.requestFrom(8, 13);    // request 13 bytes from slave device #8; change later 

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

}
