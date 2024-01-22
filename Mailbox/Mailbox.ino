/*

Created by Romel Meza, Henrik Hynkemejer, Frederik Trier, Max Brazhnyy, Johanita Janse van Rensburg
on 09/01/2024

*/
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL4BreCfEgx"
#define BLYNK_TEMPLATE_NAME "Notifier"
#define BLYNK_AUTH_TOKEN "BBPRqHHYc3hgFMvCK9UHKbURm0Vgx0aZ"

// Libraries
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>

Servo servo;


// Macros
//#define RST D0  // Configurable pins as stated on the fritzing circuit

// RFID pins
#define RST_PIN D3
#define SS_PIN D4
#define SCK D5
#define MISO D6
#define MOSI D7

#define RED D0

//Other pins
#define PHOTORESISTOR A0
#define MOTOR D8


BlynkTimer timer;  // Initializes timer for uptime log

// Your WiFi cLED_BUILTINentials.
// Set password to "" for open networks.
char ssid[] = "ICE";
char pass[] = "Bund1Ice";  // Henrik's mobile hotspot. You are welcome to join :)
int counter;  // Counter to increase


//RFID instance
MFRC522 mfrc522(SS_PIN, RST_PIN);  // creates the MFRC522 instance

// Global Variables
float prevLight = 0.0;
float currentLight = 0.0;

//Master-slave variables
char* messageToSend;  // Global variable to store the message
int c = 3;
char c_char = '3';



void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  Serial.begin(115200);
  Wire.begin(D2, D1);   //works with D2-A4 and D1-A5
  messageToSend = "1";  // Assign the message to the global variable
  

  // RFID
  SPI.begin();         // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522

  // Initialize connection to the BLYNK server
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  // Reset mail counter
  counter = -1;
  Blynk.virtualWrite(V0, counter);

  // Servo-motor
  servo.attach(MOTOR, 544, 2400);  //using the servo library to set the pin and the max and min value of roation set through miliseconds linked to the pwm.
  servo.write(180);                // these values are calibrated for the servo motor being placed on the internal ledge with the brown wire facing down.
  Serial.print("\nMaster Connected and Ready");
}

// This function will be called every time Slider Widget
// in Blynk web writes values to the Virtual Pin 1
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();  // assigning incoming value from pin V1 to a variable
  counter = -1;
  Blynk.virtualWrite(V0, counter);
  Serial.print("V1 Slider value is: ");
  Serial.println(pinValue);

  int value = param.asInt();

  int val = 180 * value;  // scale it to use it with the servo (value between 0 and 180)

  servo.write(val);
  
}

// This function will be called every time Button Widget
// in Blynk app writes values to the Virtual Pin 2
BLYNK_WRITE(V2) {
  int pinValue = param.asInt();  // assigning incoming value from pin V2 to a variable
  counter = -1;
  Blynk.virtualWrite(V0, counter);

  int value = param.asInt();

  int val = 180 * value;  // scale it to use it with the servo (value between 0 and 180)

  servo.write(val);
  
}



//


void loop() {
  // RED LED blinks with 1second intervals when system is connected to internet and ready to receive RFID cards
  digitalWrite(RED, HIGH);
  delay(500);
  digitalWrite(RED, LOW);
  delay(500);
  
  Blynk.run();
  timer.run();  // Initiates BlynkTimer

  //check change in light levels
  checkLight();

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())  // If there is a new card, then you escape the return loop
  {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())  // If there is data to read, then you escape the return loop
  {
    return;
  }
  //Show UID of presented card on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)  // Set index i to iterate 4 times, because the uid has four bits.
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");            // check if the HEX number is below 16, if TRUE, add " 0" , else add " "
    Serial.print(mfrc522.uid.uidByte[i], HEX);                           // print the HEX value
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));  // save the data in "content"
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();
  Serial.print(content.substring(1));
  if (content.substring(1) == "6C 40 CB 38")  //change here the UID of the card/cards that you want to give access
  {
    //

    Serial.println("Passing over to slave"); // Message telling the operation now carries on on the slave

    messageToSend = "A";     // Assign the message to the global variable
    communication_send_M();  //sending data from master to slave
    Blynk.virtualWrite(V3, "Frederik"); // Writing name of user that tried to access the mailbox to the server
    
    while (c != 48 && c != 49) { // while the received answer from the slave regarding the correctness of the password is not correct or incorrect, wait in this while loop
      communication_receive_M();
    }
  } else if (content.substring(1) == "53 55 F8 34")  //change here the UID of the card/cards that you want to give access
  {
    
    Serial.println("Passing over to slave");  // Message telling the operation now carries on on the slave
    messageToSend = "B";     // Assign the message to the global variable
    communication_send_M();  //sending data from master to slave
    Blynk.virtualWrite(V3, "Max"); // Writing name of user that tried to access the mailbox to the server
    
    while (c != 48 && c != 49) {
      communication_receive_M();  // while the received answer from the slave regarding the correctness of the password is not correct or incorrect, wait in this while loop
    }


  } else if (content.substring(1) == "04 AB 8F AA DA 51 80" || content.substring(1) == "26 8B 2D E6")  // Henriks Keychain and DTU card //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Passing over to slave");
    messageToSend = "C";     // Assign the message to the global variable
    communication_send_M();  //sending data from master to slave
    Blynk.virtualWrite(V3, "Henrik"); // Writing name of user that tried to access the mailbox to the server

    while (c != 48 && c != 49) {
      communication_receive_M();  // while the received answer from the slave regarding the correctness of the password is not correct or incorrect, wait in this while loop
    }
  } else if (content.substring(1) == "56 73 B8 75")  // Johanita's DTU card //change here the UID of the card/cards that you want to give access
  {

    Serial.println("Passing over to slave");
    messageToSend = "D";     // Assign the message to the global variable
    communication_send_M();  //sending data from master to slave
    Blynk.virtualWrite(V3, "Johanita"); // Writing name of user that tried to access the mailbox to the server

    while (c != 48 && c != 49) {
      communication_receive_M();  // while the received answer from the slave regarding the correctness of the password is not correct or incorrect, wait in this while loop
    }

  } else if (content.substring(1) == "E4 4A E5 52")  // Romel's DTU card //change here the UID of the card/cards that you want to give access
  {

    Serial.println("Passing over to slave");
    messageToSend = "E";     // Assign the message to the global variable
    communication_send_M();  //sending data from master to slave
    Blynk.virtualWrite(V3, "Romel");  // Writing name of user that tried to access the mailbox to the server

    while (c != 48 && c != 49) {
      communication_receive_M();  // while the received answer from the slave regarding the correctness of the password is not correct or incorrect, wait in this while loop
    }
  } else {// if the read card is not one of the valid ones, flash red light
    flashLight(RED, 5); 
    delay(5000); // wait 5 seconds before next try

  }

  if (c == 49) { // If the entered password is correct, the master will receive 49, which corresponds to 1 in the ASCII table
    // in case the passcode is correct, the green light is flashed by the slave, since that's where it's connected
    Serial.print("Correct passcode!");
    counter = 0;
    Blynk.virtualWrite(V0, counter);
    servo.write(0); // opening the lock

    delay(5000);  // wait 5 seconds before closing the lock; can be increased in real-world scenario to fit the needs of the user

    servo.write(180); // closing the lock

  } else if (c == 48) { // If the entered password is incorrect, the master will receive 48, which corresponds to 0 in the ASCII table
    flashLight(RED, 5); // flash red light

    Serial.print("Wrong passcode!");  // wait 5 seconds before next try

    delay(5000);

  }
  c = 51; // reset c value to 51 (can be whatever, other than 48 or 49)
}

// Functions

void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000);
}

void checkLight() {
  // Function that checks change in light
  float threshold = prevLight * 1.5;  //proportional threshold for determining if light change is gradual or sudden
  currentLight = analogRead(PHOTORESISTOR);
  float lightChange = abs(currentLight - prevLight);  //change in light level since last reading
  if (lightChange > threshold) {                      //sudden change in light i.e. mailbox has been opened
    counter++;
    Blynk.virtualWrite(V0, counter);  //tell the server -> need to send 1/HIGH/True on light sensor recognised mialbox being opened
    delay(500);
    prevLight = currentLight;  //updating the light level
    delay(500);
  } else {  //gradual light change due to noise - no action required
    delay(500);
    prevLight = currentLight;
    delay(500);
  }
}

void communication_send_M() {
  //  communicate(send) message from master to slave
  Wire.beginTransmission(8);  // Address of the slave
  Wire.write(messageToSend);  // Send a command (character 'A' in this example)
  Wire.endTransmission();
}

void communication_receive_M() {
  //  communicate (receive) message from slave to master
  Wire.requestFrom(8, 1);  // request 1 bytes from slave device #8
  Serial.print("\n");
  while (Wire.available()) {  // slave may send less than requested
    c = Wire.read();
    c_char = (char)c;  // receive a byte as character
  }
  delay(1000);
}

void flashLight(int color, int times) {
  // function that flashes a light "color" "times" times
  for (int i = 0; i < times + 1; i++) {
    digitalWrite(color, HIGH);
    delay(50);
    digitalWrite(color, LOW);
    delay(50);
  }
}
