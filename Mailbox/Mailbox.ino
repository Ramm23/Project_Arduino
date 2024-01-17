/*

Created by Romel Meza, Henrik Hynkemejer, Frederik Trier, Max Brazhnyy
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
#define RST D0  // Configurable pins as stated on the fritzing circuit

// RFID pins
#define RST_PIN D3
#define SS_PIN D4
#define SCK D5
#define MISO D6
#define MOSI D7
//Ground
//3.3V

// I2C pins
//D1  , dont know which is SDA and SCL
//D2

//RGB pins
#define RED D0
#define BLUE SD2
#define GREEN SD3

//Other pins
#define PHOTORESISTOR A0
#define MOTOR D8


BlynkTimer timer;  // Initializes timer for uptime log

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ICE";
char pass[] = "Bund1Ice";  // Henrik's mobile hotspot. You are welcome to join :)
//char ssid[] = "Zyxel_B6F1";
//char pass[] = "R3787P33UX";
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
  Serial.begin(115200);
  Wire.begin(D2, D1);   //works with D2-A4 and D1-A5
  messageToSend = "1";  // Assign the message to the global variable
  Serial.print("\nMaster Ready");
  //RFID
  SPI.begin();         // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522

  // Initialize connection to the BLYNK server
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  // Reset mail counter
  counter = 0;
  Blynk.virtualWrite(V0, counter);

  //motor
  servo.attach(MOTOR, 544, 2400);                //using the servo library to set the pin and the max and min value of roation set through miliseconds linked to the pwm.
  servo.write(0);                             // these values are calibrated for the servo motor being placed on the internal ledge with the brown wire facing down.
}
// This function will be called every time Slider Widget
// in Blynk web writes values to the Virtual Pin 1
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();  // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V1 Slider value is: ");
  Serial.println(pinValue);
}

// This function will be called every time Button Widget
// in Blynk app writes values to the Virtual Pin 2
BLYNK_WRITE(V2) {
  int pinValue = param.asInt();  // assigning incoming value from pin V2 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V2 Button value is: ");
  Serial.println(pinValue);
}

BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  int val = 180 * value;  // scale it to use it with the servo (value between 0 and 180)

  servo.write(val);  // sets the servo position according to the scaled value

  //delay(15);
  // Update state
  //Blynk.virtualWrite(V1, userName);
}

//


void loop() {
  Serial.print("top of loop");
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
  //Show UID on serial monitor
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
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  Serial.print(content.substring(1));
  if (content.substring(1) == "6C 40 CB 38")  //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Passing over to slave");
    messageToSend = "A";     // Assign the message to the global variable
    communication_send_M();  //sending data from master to slave
    Blynk.virtualWrite(V3, "Frederik");
    // Show yellow light on the RGB
    // call function that initiates numpad on the slave Arduino. Check if the password is correct on the slave.
    // If Master recieves recieves "Correct" then continue, otherwise show RED light.
    // If the password is correct, send HIGH to server.
    // Show green light on the RGB
    // do server read, and if button == high, the mailbox opens. (A bit unnecessary, but we need to do a server.read)
    // Send name of person to server, and time of day from real time clock.
    while (c != 48 && c != 49) {
      communication_receive_M();
      Serial.print(c);
    }
  }
  if (content.substring(1) == "53 55 F8 34")  //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Passing over to slave");
    messageToSend = "B";     // Assign the message to the global variable
    communication_send_M();  //sending data from master to slave
    Blynk.virtualWrite(V3, "Max");
    // Show yellow light on the RGB
    // call function that initiates numpad on the slave Arduino. Check if the password is correct on the slave.
    // If Master recieves recieves "Correct" then continue, otherwise show RED light.
    // If the password is correct, send HIGH to server.
    // Show green light on the RGB
    // do server read, and if button == high, the mailbox opens. (A bit unnecessary, but we need to do a server.read)
    // Send name of person to server, and time of day from real time clock.
    while (c != 48 && c != 49) {
      communication_receive_M();
      Serial.print(c);
    }
  }
  if (content.substring(1) == "04 AB 8F AA DA 51 80" || content.substring(1) == "26 8B 2D E6")  // Henriks Keychain and DTU card //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Passing over to slave");
    messageToSend = "C";     // Assign the message to the global variable
    communication_send_M();  //sending data from master to slave
    Blynk.virtualWrite(V3, "Henrik");

    // Show yellow light on the RGB
    // call function that initiates numpad on the slave Arduino. Check if the password is correct on the slave.
    // If Master recieves recieves "Correct" then continue, otherwise show RED light.
    // If the password is correct, send HIGH to server.
    // Show green light on the RGB
    // do server read, and if button == high, the mailbox opens. (A bit unnecessary, but we need to do a server.read)
    // Send name of person to server, and time of day from real time clock.
    while (c != 48 && c != 49) {
      communication_receive_M();
      Serial.print(c);
    }
  }
  if (content.substring(1) == "56 73 B8 75")  // Johanita's DTU card //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Passing over to slave");
    messageToSend = "D";     // Assign the message to the global variable
    communication_send_M();  //sending data from master to slave
    Blynk.virtualWrite(V3, "Johanita");
    // Show yellow light on the RGB
    // call function that initiates numpad on the slave Arduino. Check if the password is correct on the slave.
    // If Master recieves recieves "Correct" then continue, otherwise show RED light.
    // If the password is correct, send HIGH to server.
    // Show green light on the RGB
    // do server read, and if button == high, the mailbox opens. (A bit unnecessary, but we need to do a server.read)
    // Send name of person to server, and time of day from real time clock.
    while (c != 48 && c != 49) {
      communication_receive_M();
      Serial.print(c);
    }
  }
  if (content.substring(1) == "E4 4A E5 52")  // Romel's DTU card //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Passing over to slave");
    messageToSend = "E";     // Assign the message to the global variable
    communication_send_M();  //sending data from master to slave
    Blynk.virtualWrite(V3, "Romel");
    // Show yellow light on the RGB
    // call function that initiates numpad on the slave Arduino. Check if the password is correct on the slave.
    // If Master recieves recieves "Correct" then continue, otherwise show RED light.
    // If the password is correct, send HIGH to server.
    // Show green light on the RGB
    // do server read, and if button == high, the mailbox opens. (A bit unnecessary, but we need to do a server.read)
    // Send name of person to server, and time of day from real time clock.
    while (c != 48 && c != 49) {
      communication_receive_M();
      Serial.print(c);
    }
  }

  Serial.println();
  //delay(3000);  // This delay was already in the code, when i got it. But i think it seems fine.
  Serial.print("received output from slave:");
  Serial.print(c);
  if (c == 49) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);
    Serial.print("Correct passcode!");
    servo.write(180);
    delay(30000);
    servo.write(0);

    //Blynk.virtualWrite(V3, userName);
  } else if (c == 48) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);
    Serial.print("Wrong passcode!");
  }
  c = 51;
  /*
  else {
    Serial.println(" Access denied");
    // Red light
    delay(3000);
  }
  */
}

void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000);
}

void checkLight() {
  float threshold = prevLight * 1.1;  //proptional threshold for determinig if light change is gradual or sudden
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

void lockDoor() {
  servo.write(0);               //when the motor rod is angled at 0 degrees, the bottle cap(lock meachnism for prototype), will prevent the door from being opened
}

void unlockDoor() {
  servo.write(180);         //motor rod angled at 180 degrees and lock mechanism and therefor door is free to move
}

void communication_send_M() {
  //Serial.print("Sending A");
  Wire.beginTransmission(8);  // Address of the slave
  Wire.write(messageToSend);  // Send a command (character 'A' in this example)
  Wire.endTransmission();
}

void communication_receive_M() {
  Wire.requestFrom(8, 1);  // request 2 bytes from slave device #8; change according to need
  Serial.print("\n");
  while (Wire.available()) {  // slave may send less than requested
    c = Wire.read();
    c_char = (char)c;  // receive a byte as character
    //Serial.print(c);          // print the character
  }
  delay(1000);
}