// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This keypad example code is in the public domain.
/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/

#include <Keypad.h>

const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 2, 3, 4, 5 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 6, 7, 8, 9 };  //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char attempt[6] = { 0, 0, 0, 0, 0, 0 };  // Array to store the pressed keys
int z = 0;
bool var = false;



#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(8);  // join i2c bus with address #8
  Serial.print("\nSlave Ready\n");
}

void loop() {
  /*
  char customKey;                     // resets the keypad input
  customKey = customKeypad.getKey();  // Reads the Keypad input

  if (customKey != NO_KEY) {    // if there is an input
    Serial.println(customKey);  // print the input
  }
  delay(100);
  
  readKeypad();
  */
  delay(100);
  communication_receive_s(); // receiving data on slave from master

  delay(100);
  communication_send_s(); // sending data from slave to master

}


void receiveEvent() {
  // Receive and process the command from the master
  Serial.print("\n");
  while (Wire.available()) {
    char command = Wire.read();
    Serial.print(command);
    //Serial.print(command, DEC);
    //Serial.print("\n");
    // Perform an action based on the received command
    if (command == 'A') {
      // Your action for command 'A'
      Serial.println("Received command 'A'. Performing action.");
      // Add your code here to perform a specific action when 'A' is received
    }
    // Add more conditions for other commands if needed
  }
}

void readKeypad() {
  char key = customKeypad.getKey();
  if (key != NO_KEY) {
    Serial.print(key);
    if (z > 5) {  // Reset the input if more than 4
      z = 0;
    }
    if (key == '*') {  //Initialize the PIN read -> This can change to when the RFID is read
      var = true;
    }

    if (key == '#') {  // '#' resets the attempt
      if (z == 5) {    // Except when it's the last character
        //  checkPin();
        Serial.println(attempt);
      }
      var = false;
      z = 0;
    }

    if (var) {
      attempt[z] = key;  // Write the pin code
      z++;
    }
  }
}

void requestEvent() {
  Wire.write("\nslave-master"); // respond with message of 6 bytes
  // as expected by master
}

void communication_send_s() {
  Wire.onRequest(requestEvent); // register event
}

void communication_receive_s(){
  Wire.onReceive(receiveEvent);
}
