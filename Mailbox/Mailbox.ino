/*

Crerated by Romel Meza,
on 09/01/2024

*/

// Libraries
#include <ESP8266WiFi.h>
#include <Servo.h>

// Macros
#define N 0;
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 9                 // Configurable, see typical pin layout above
#define SS_PIN 10                 // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

// Global Variables

const char Servo_motor = D1;

void setup()
{
  Serial.begin(115200);
  // put your setup code here, to run once:
}

void loop()
{
  // put your main code here, to run repeatedly:
}
