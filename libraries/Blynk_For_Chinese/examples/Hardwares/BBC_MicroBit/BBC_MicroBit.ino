/*************************************************************
  项目说明：Micro:bit 案例

温馨提示：本项目需要以下第三方库支持
https://github.com/sandeepmistry/arduino-nRF5
https://github.com/sandeepmistry/arduino-BLEPeripheral

Blynk 支持Microbit具体使用方法请参考
http://help.blynk.cc/how-to-connect-different-hardware-with-blynk/arduino/bbc-microbit-arduino-ide

  blynk_for_chinese 库地址：https://github.com/hznupeter/blynk-library-for-chinese

  Blynk物联网学习资料：https://gitee.com/hznupeter/Blynk_IOT/wikis
  

  注意：蓝牙连接功能还在测试中！
 *************************************************************/

#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleBLEPeripheral.h>
#include <BLEPeripheral.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Create ble serial instance, parameters are ignored for MicroBit
BLESerial SerialBLE(0, 0, 0);

void setup() {
  Serial.begin(9600);

  SerialBLE.setLocalName("Blynk");
  SerialBLE.setDeviceName("Blynk");
  SerialBLE.setAppearance(0x0080);
  SerialBLE.begin();

  Serial.println("Waiting for connections...");

  Blynk.begin(SerialBLE, auth);
}

void loop() {
  SerialBLE.poll();

  if (SerialBLE) {    // If BLE is connected...
    Blynk.run();
  }
}