/*************************************************************
  项目说明：DHT11测温湿度
  App项目设置:
  创建Value Display组件，输入管脚设置为V5,数值范围0-100,reading rate设置为push
  创建Value Display组件，输入管脚设置为V6,数值范围-20-60,reading rate设置为push

  硬件连接：DHT11连接在GPIO2管脚
  blynk_for_chinese 库地址：https://github.com/hznupeter/blynk-library-for-chinese

  Blynk物联网学习资料：https://gitee.com/hznupeter/Blynk_IOT/wikis
 *************************************************************/
#define BLYNK_PRINT Serial // 开启串口监视
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
char auth[] = "2a365b624c0f4ea891256d4a66d428f7";//授权码
char ssid[] = "ssid";//wifi名称
char pass[] = "psssword";//wifi密码
#define DHTPIN 2//传感器连接管脚
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
//该函数将会每秒钟发送数据给V5,V6,
//在app端，组件的读取频率应该设为PUSH。
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); //摄氏度
  //float t = dht.readTemperature(true); //华氏度
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);//将湿度发送给V5
  Blynk.virtualWrite(V6, t);//将湿度发送给V6
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);//官方服务器
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);//自建服务器域名模式
  //Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 1, 158), 8080);//自建服务器ip模式
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}

