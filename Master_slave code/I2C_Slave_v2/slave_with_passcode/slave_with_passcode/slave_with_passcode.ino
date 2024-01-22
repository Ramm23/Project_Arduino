//slave_with_passcode
/*

Created by Romel Meza, Henrik Hynkemejer, Frederik Trier, Max Brazhnyy, Johanita Janse van Rensburg
on 09/01/2024

*/


#include <Keypad.h>

#define GREEN 12

const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 5, 4, 3, 2 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 9, 8, 7, 6 };  //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


char key;
int i = 0;
String inputString;
long inputInt;
// Defining passwords for each of the 5 users
String passcodeA = "1234";
String passcodeB = "2345";
String passcodeC = "3456";
String passcodeD = "4567";
String passcodeE = "5678";
char* messageToSend;  // Message to send to master; Assuming it's a single character

char message_received = '0';  //variable for checking if message received from master; 1 if a message has been received; 0 if a message has not been received
char command  = '0'; // Actual received message. Could be A, B C, D or E, depending on user

#include <Wire.h>


void setup() {
  Serial.begin(115200);
  Wire.begin(8);        // join i2c bus with address #8
  pinMode(GREEN, OUTPUT);
  Serial.print("\nSlave Ready\n"); // Announce that slve is ready for operation
}

void loop() {
  while (message_received == '0') { // while that waits for a message from master to carry on to check the passcode
    delay(100);
    communication_receive_s();  // receiving data on slave from master
    
  }
  flashLight(GREEN, 4); // flashing green light because correct card was presented
  
  check_code(); // wait for the user to input 4 numbers on the numpad = code

  check_correctness(); // check if the entered 4 numbers correspond to the passcode of the user

  communication_send_s();  // sending data from slave to master
  delay(3000);
  // reseting values of both received and sent messages
  messageToSend = "3";
  message_received = '0';
}


void receiveEvent() {
  // Receive and process the command from the master
  while (Wire.available()) {
    command = Wire.read();
    if (command == 'A' || command == 'B' || command == 'C' || command == 'D') {
      message_received = '1';
    } else message_received = "0";
  }
  Serial.print("command after =");
  Serial.print(command);
}

void requestEvent() {
  // support function to send a message to the master
  Serial.print("Sending");
  Serial.print(messageToSend);
  Wire.write(messageToSend, sizeof(messageToSend + 1));
  delay(1500);
}

void communication_send_s() {
  //  communicate(send) message from slave to master
  Wire.onRequest(requestEvent);
}

void communication_receive_s() {
  //  communicate (receive) message from master to slave
  Wire.onReceive(receiveEvent);
}

void check_code() {  //adapted code from https://arduinogetstarted.com/faq/how-to-input-a-multiple-digits-number-using-the-keypad?utm_content=cmp-true
  
  Serial.print("\n \nInput your password, please:");
  messageToSend = "3";
  inputString = "";  // Reset the inputString for the next input; inputString is the variable where the entered code is stored
  while (inputString.length() != 4) {

    char key = keypad.getKey();
    while (key == NO_KEY) {
      key = keypad.getKey();
    }
    if (key) {
      Serial.println(key);

      if (key >= '0' && key <= '9') {  // only act on numeric keys
        inputString += key;            // append new character to input string


      } else if (key == '#') {
        if (inputString.length() > 0) {
          inputInt = inputString.toInt(); 
          inputString = "";          
        }
      } else if (key == '*') {
        inputString = "";  // clear input
      }
    }
  }
  Serial.print("Full passcode (inputString): ");
  Serial.println(inputString);
  return;
}

void check_correctness() {
  // funcitons that checks if entered password corresponds to the user that tapped the RFID card
  Serial.print("\nChecking correctenss...");
  Serial.print(" for password:");
  Serial.print(command);
  if (command == 'A' && inputString.equals(passcodeA)) {
    Serial.print("\nCorrect!");
    flashLight(GREEN, 4); // flash green light upon correct code
    messageToSend = "1"; // define message to send to master as correct one
  } 
  else if (command == 'B' && inputString.equals(passcodeB)) {
    Serial.print("\nCorrect!");
    flashLight(GREEN, 4); // flash green light upon correct code
    messageToSend = "1";  // define message to send to master as correct one
  } 
  else if (command == 'C' && inputString.equals(passcodeC)) {
    Serial.print("\nCorrect!");
    flashLight(GREEN, 4); // flash green light upon correct code
    messageToSend = "1";  // define message to send to master as correct one
  }   
  else if (command == 'D' && inputString.equals(passcodeD)) {
    Serial.print("\nCorrect!");
    flashLight(GREEN, 4); // flash green light upon correct code
    messageToSend = "1";  // define message to send to master as correct one
  }   
  else if (command == 'E' && inputString.equals(passcodeE)) {
    Serial.print("\nCorrect!");
    flashLight(GREEN, 4); // flash green light upon correct code
    messageToSend = "1";  // define message to send to master as correct one
  }
  else {
    Serial.print("\nPassword incorrect");
    Serial.print("\nReceived password: ");
    Serial.print(inputString);
    messageToSend = "0"; // define message to send to master as incorrect one
  }
}

void flashLight(int color,int times) {
  // function that flashes a light "color" "times" times
  for (int i = 0; i < times + 1; i++) {
    digitalWrite(color, HIGH);
    delay(50);
    digitalWrite(color, LOW);
    delay(50);
  }
}