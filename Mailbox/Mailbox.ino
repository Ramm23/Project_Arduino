/*

Created by Romel Meza, Henrik Hynkemejer, Frederik Trier, Max Brazhnyy
on 09/01/2024

*/

// Libraries
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

// Macros
#define RST D0 // Configurable pins as stated on the fritzing circuit

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

//RFID instance
MFRC522 mfrc522(SS_PIN, RST_PIN);   // creates the MFRC522 instance

// Global Variables


void setup()
{
  Serial.begin(115200);
  //RFID 
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  //
}

void loop()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())      // If there is a new card, then you escape the return loop
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())        // If there is data to read, then you escape the return loop
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)      // Set index i to iterate 4 times, because the uid has four bits.
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");         // check if the HEX number is below 16, if TRUE, add " 0" , else add " "
     Serial.print(mfrc522.uid.uidByte[i], HEX);                        // print the HEX value
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));    // save the data in "content"
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  Serial.print(content.substring(1));
  if (content.substring(1) == "6C 40 CB 38") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Enter Password");
    // call function sending command to slave
    // if code is correct, open. And show green LED.
    // Set button to high on the server.
    // do server read, and if button == high, open the mailbox. (A bit unnecessary, but we need to do a server.read)
    // Send name of person to server, and time of day from real time clock. 
    Serial.println();
    delay(3000);            // This delay was already in the code, when i got it. But i think it seems fine. 
  }
 
 else   {
    Serial.println(" Access denied");
    delay(3000);
  }
}
