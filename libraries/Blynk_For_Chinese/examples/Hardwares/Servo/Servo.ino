/*************************************************************
  项目说明：控制舵机
  硬件连接：舵机连接在GPIO12管脚
  App项目设置:
  创建slider组件，输出管脚设置为V3,数值范围0-180

  blynk_for_chinese 库地址：https://github.com/hznupeter/blynk-library-for-chinese

  Blynk物联网学习资料：https://gitee.com/hznupeter/Blynk_IOT/wikis
 *************************************************************/

#define BLYNK_PRINT Serial // 开启串口监视
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
char auth[] = "2a365b624c0f4ea891256d4a66d428f7";//授权码
char ssid[] = "ssid";//wifi名称
char pass[] = "psssword";//wifi密码
Servo servo;

BLYNK_WRITE(V0)//通过虚拟管脚V0写入角度
{
  servo.write(param.asInt());
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);//官方服务器
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);//自建服务器域名模式
  //Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 1, 158), 8080);//自建服务器ip模式
  servo.attach(12);//舵机连接管脚
}

void loop()
{
  Blynk.run();
}

