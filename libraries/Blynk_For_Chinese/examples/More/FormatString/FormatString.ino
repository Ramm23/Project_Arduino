/*************************************************************
  项目说明：字符串格式转换
  App项目设置:  创建Value Display组件，输入管脚设置为V5
 *************************************************************/
#define BLYNK_PRINT Serial // 开启串口监视
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "2a365b624c0f4ea891256d4a66d428f7";//授权码
char ssid[] = "ssid";//wifi名称
char pass[] = "psssword";//wifi密码
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendTemperature()
{
  float t = float(random(100, 300)) / 10;// 生成10.0到30.0之间的随机数作为温度
  String str = String(t, 1) + "℃";  // 格式转换:保留一位小数，并添加℃
  Blynk.virtualWrite(V5, str);// 将信息发给V5管脚
}

void setup()
{
  Serial.begin(9600);
 Blynk.begin(auth, ssid, pass);//官方服务器
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);//自建服务器域名模式
  //Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 1, 158), 8080);//自建服务器ip模式
  timer.setInterval(1000L, sendTemperature);//每秒钟调用sendTemperature函数
}

void loop()
{
  Blynk.run();
  timer.run();
}

