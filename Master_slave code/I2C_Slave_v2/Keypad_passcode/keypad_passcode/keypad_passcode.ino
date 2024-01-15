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

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char* passcodeA;
char* passcodeB;
char* passcodeC;
char* passcodeD;
char* passcode;
char key;
int i = 0;

void setup(){
  Serial.begin(115200);
  passcodeA = "1234";
  passcodeA = "2345";
  passcodeA = "3456";
  passcodeA = "4567";

}
  
void loop(){
  //run_keypad();
  i = 0;
  char full_passcode = register_key();
  Serial.print("\nTotal Received Passcode: ");
  Serial.print(full_passcode);
  delay(10*1000);
}

void run_keypad(){
  Serial.print("\nInput passcode on numpad");
  
  while (key == NO_KEY){
      key = keypad.getKey();
  }  
  for (int i =0; i<3 ; i++){
    //while (key == NO_KEY){
    //key = keypad.getKey();
    passcode[i] = key;
    Serial.print("\nGot key: ");
    Serial.print(key);
    key = NO_KEY;
    while (key == NO_KEY){
      key = keypad.getKey();
    }
  }
  Serial.print("\nTotal Received Passcode: ");
  Serial.print(passcode);

}
void check_passcode(){
  if (passcode == passcodeA){
    Serial.print("\npasscode == passcodeA");
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

char register_key() {
  Serial.print("\nInput passcode on numpad");
  while(i<4){
    key = keypad.getKey();
    if (key != NO_KEY)
      i = i+1;
      passcode[i] = key;
      Serial.print("\nGot key: ");
      Serial.print(key);
  }
  return passcode;
}


