//slave_with_passcode

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
String passcodeA = "1234";
String passcodeB = "2345";
String passcodeC = "3456";
String passcodeD = "4567";
String passcodeE = "5678";
char* messageToSend;  // Assuming it's a single character

char message_received = '0';  //1 if a message has been received; 0 if a message has not been received
char command  = '0';
//char* correctness;

#include <Wire.h>

//char* messageToSend;  // Global variable to store the message


void setup() {
  Serial.begin(115200);
  Wire.begin(8);        // join i2c bus with address #
  //messageToSend = "s";  // Assign the message to the global variable
  pinMode(GREEN, OUTPUT);
  Serial.print("\nSlave Ready\n");
  
}

void loop() {
  Serial.print("\nbeginning of loop ");
  while (message_received == '0') {
    
    delay(100);
    communication_receive_s();  // receiving data on slave from master
  }
  check_code();

  check_correctness();

 
  
  communication_send_s();  // sending data from slave to master
  Serial.print("\n Delay for 3 seconds ");
  delay(3000);
  //messageToSend = "3";
  //message_received = '0';
}


void receiveEvent() {
  // Receive and process the command from the master
  Serial.print("\n");
  Serial.print("command before =");
  Serial.print(command);
  while (Wire.available()) {
    command = Wire.read();
    //Serial.print(command);
    if (command == 'A' || command == 'B' || command == 'C' || command == 'D') {
      message_received = '1';
    } else message_received = "0";
  }
  Serial.print("command after =");
  Serial.print(command);
}

void requestEvent() {
  Serial.print("Sending");
  Serial.print(messageToSend);
  Wire.write(messageToSend, sizeof(messageToSend + 1));
  delay(1500);
  //messageToSend = "3";
}

void communication_send_s() {
  Wire.onRequest(requestEvent);
}

void communication_receive_s() {
  
  Wire.onReceive(receiveEvent);
}

void check_code() {  //adapted code from https://arduinogetstarted.com/faq/how-to-input-a-multiple-digits-number-using-the-keypad?utm_content=cmp-true
  flashLight(GREEN, 4);
  Serial.print("\n \nInput your password, please:");
  messageToSend = "3";
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

void check_correctness() {
  Serial.print("\nChecking correctenss...");
  Serial.print(" for password:");
  Serial.print(command);
  if (command == 'A' && inputString.equals(passcodeA)) {
    Serial.print("\nCorrect!");
    flashLight(GREEN, 4);
    messageToSend = "1";
  } 
  else if (command == 'B' && inputString.equals(passcodeB)) {
    Serial.print("\nCorrect!");
    flashLight(GREEN, 4);
    messageToSend = "1";
  } 
  else if (command == 'C' && inputString.equals(passcodeC)) {
    Serial.print("\nCorrect!");
    flashLight(GREEN, 4);
    messageToSend = "1";
  }   
  else if (command == 'D' && inputString.equals(passcodeD)) {
    Serial.print("\nCorrect!");
    flashLight(GREEN, 4);
    messageToSend = "1";
  }   
  else if (command == 'E' && inputString.equals(passcodeE)) {
    Serial.print("\nCorrect!");
    flashLight(GREEN, 4);
    messageToSend = "1";
  }
  else {
    Serial.print("\nPassword incorrect");
    Serial.print("\nReceived password: ");
    Serial.print(inputString);
    messageToSend = "0";
  }
}
///
void flashLight(int color,int times) {
  for (int i = 0; i < times + 1; i++) {
    digitalWrite(color, HIGH);
    delay(50);
    digitalWrite(color, LOW);
    delay(50);
  }
}

