#define BLYNK_TEMPLATE_ID "TMPL5atEoR1Yv"
#define BLYNK_TEMPLATE_NAME "Mailbox"
#define BLYNK_AUTH_TOKEN "ThpmxDcN_9UNyEkiN4AEHcjPUUzD0KAn"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char ssid[] = "Kingme";
char pass[] = "EspPassWifi";

#include <Servo.h>


Servo myservo;  // create servo object to control a servo


int val;  // variable to read the value from the analog pin

BlynkTimer timer;  // Creating a timer object

BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  val = 180 * value;  // scale it to use it with the servo (value between 0 and 180)

  myservo.write(val);  // sets the servo position according to the scaled value

  //delay(15);
  // Update state
  const char* myName = "Romel2";
  Blynk.virtualWrite(V1, myName);
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  //Blynk.virtualWrite(V2, millis() / 1000);
}



void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  myservo.attach(D0);

  // Setup a timer function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}


void loop() {
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}