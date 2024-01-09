/*

Crerated by Romel Meza, Henrik Hynkemejer
on 09/01/2024

*/

// Libraries
#include <ESP8266WiFi.h>
#include <Servo.h>

// Macros
#define RST D0 // Configurable pins as stated on the fritzing circuit
#define MISO D6
#define MOSI D7
#define SDA D8
#define SCK D5
#define RED D3
#define BLUE D2
#define GREEN D3
#define PHOTORESISTOR A0
#define MOTOR D4

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
