/*

Created by Romel Meza, Henrik Hynkemejer, Frederik Trier, Max Brazhnyy
on 09/01/2024

*/

// Libraries
#include <ESP8266WiFi.h>
#include <Servo.h>

// Macros
#define RST D0 // Configurable pins as stated on the fritzing circuit

// RFID pins
#define RST D3
#define SS D4
#define SCK D5
#define MISO D6
#define MOSI D7
//Ground
//3.3V 

//RGB pins
#define RED D0
#define BLUE D1
#define GREEN D2

//Other pins
#define PHOTORESISTOR A0
#define MOTOR SMD

//Unused pins
#define SDA D8

// Global Variables


void setup()
{
  Serial.begin(115200);
  // put your setup code here, to run once:
}

void loop()
{
  // put your main code here, to run repeatedly:
}
