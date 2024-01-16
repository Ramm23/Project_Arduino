/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
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
char* passcodeA = "1234";
char* passcodeB = "2345";
char* passcodeC = "3456";
char* passcodeD = "4567";

void setup() {
  Serial.begin(115200);

  // passcodeA = "2345";
  // passcodeA = "3456";
  // passcodeA = "4567";
}

void loop() {
  //run_keypad();
  // i = 0;
  //char full_passcode = register_key();
  check_code();
  // Serial.print("\nTotal Received Passcode: ");
  // Serial.print(full_passcode);
  Serial.print("\nPasscode received;");
  check_correctness();
  Serial.print("\n \nPerforming Delay of 10 seconds");
  delay(10 * 1000);
}

/*void run_keypad() {
  Serial.print("\nInput passcode on numpad");

  while (key == NO_KEY) {
    key = keypad.getKey();
  }
  for (int i = 0; i < 3; i++) {
    //while (key == NO_KEY){
    //key = keypad.getKey();
    passcode[i] = key;
    Serial.print("\nGot key: ");
    Serial.print(key);
    key = NO_KEY;
    while (key == NO_KEY) {
      key = keypad.getKey();
    }
  }
  Serial.print("\nTotal Received Passcode: ");
}
*/


void check_correctness() {
  Serial.print("\nChecking correctenss...");
  if (inputString == passcodeA) {
    Serial.print("\nCorrect!");
  } 
  else if (inputString == passcodeB) {
    Serial.print("\nCorrect!");
  } 
  else if (inputString == passcodeC) {
    Serial.print("\nCorrect!");
  }   
  else if (inputString == passcodeD) {
    Serial.print("\nCorrect!");
  }   
  else {
    Serial.print("\nPassword incorrect");
    Serial.print("\nReceived password: ");
    Serial.print(inputString);
  }
}


/*
char register_key(){
  i = i+1;
  key = keypad.getKey();
  passcode[i] = key;
  Serial.print("\nGot key: ");
  Serial.print(key);
  key = NO_KEY;
  if (i < 4) {
    while (key == NO_KEY){
      register_key();
    }
  }
  else {
    return passcode;
  }
}

*/

/*char register_key() {
  Serial.print("\nInput passcode on numpad");
  for(int i = 0; i < 4; i++){
    while(key == NO_KEY){
      //Serial.print("\nWaiting for key inside while loop\n");
      key = keypad.getKey();
    }
    passcode[i] = key;
    Serial.print("\nGot key: ");
    Serial.print(key);
    key = keypad.getKey();
  }
  return passcode;
}
*/


void check_code() {  //adapted code from https://arduinogetstarted.com/faq/how-to-input-a-multiple-digits-number-using-the-keypad?utm_content=cmp-true
  Serial.print("\n \nInput your password, please:");
  inputString = "";  // Reset the inputString for the next input
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
          inputInt = inputString.toInt();  // YOU GOT AN INTEGER NUMBER
          inputString = "";                // clear input
          // DO YOUR WORK HERE
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
