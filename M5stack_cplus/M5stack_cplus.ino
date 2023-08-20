// 联网相关库
#include <WiFi.h>
#include <WiFiClient.h>

// 语音识别相关库
#include <driver/i2s.h>
#include <HTTPClient.h>
#include "BaiduRest.h"

// M5StickC相关库（含TFT彩屏库）
#include <M5StickCPlus.h>
//led点阵库
#include"HC_595.h"
//
#include "image.h"  // 图片取模数据
#include "Function_wrapper.h"  // 函数封装

// -------------------------- 用户设置开始 ------------------------------ //
// -------------------------------------------------------------------- //

// WiFi账号与密码
char ssid[] = "iPhone 13";
char pass[] = "987654123";

// 语音识别授权码
#define voiceAuth "4c75259f06e48aef99f97e6d861218fc"

// -------------------------------------------------------------------- //
// -------------------------- 用户设置结束 ------------------------------ //

BaiduRest rest;
hc_595 hc;
uint8_t microphonedata0[1024 * 80];
size_t byte_read = 0;
int16_t *buffptr;
uint32_t data_offset = 0;//超时时间
String speakStr;//识别结果
bool speakFlag = false;//麦克风标志


// M5StickC屏幕显示
void M5TextDisplay(String Text) 
{
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(10, 5);
  M5.Lcd.writeHzk(utf8togb2312(String(Text).substring(((1 - 1) * 3), (9 * 3))));
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.writeHzk(utf8togb2312(String(Text).substring(((10 - 1) * 3), (18 * 3))));
  M5.Lcd.setCursor(10, 55);
  M5.Lcd.writeHzk(utf8togb2312(String(Text).substring(((19 - 1) * 3), (27 * 3))));
}

void setup()
{
  Serial.begin(9600);

  // M5显示屏初始化
  M5.begin();
  M5.Lcd.setRotation(0);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.loadHzk16();
  M5.Axp.ScreenBreath(10);
  M5.Lcd.drawXBitmap(0, 40, network, 80, 80, RED);  // 显示WiFi离线图标

  // M5按键初始化
  pinMode(37, INPUT_PULLUP);


  // 麦克风初始化
  Serial.println("Init microphone");
  InitI2SSpeakOrMic(1);
  delay(100);

  //初始化led点阵
  hc.init();
  

  
  //初始化wifi
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED)
  {
    WiFi.begin(ssid,pass);
    delay(500);
    Serial.print(".");
    
  }
  rest.settoken(voiceAuth);

  M5.Lcd.drawXBitmap(0, 40, network, 80, 80, GREEN);  // 显示WiFi在线图标
  //M5.Lcd.setRotation(1);
}

void loop()
{
   
 // M5.Lcd.setRotation(1);
  M5.Lcd.drawXBitmap(0, 40, network, 80, 80, GREEN);  // 显示WiFi在线图标
  
  // 按下M5按键开始语音识别
  if (!digitalRead(37))
  {
    M5.Lcd.setRotation(0);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.drawXBitmap(0, 17, microphone, 80, 125, GREEN);//显示麦克风图标
    data_offset = 0;//超时时间归零
    speakFlag = false;//说话标志位为否

    // 麦克风开始录音
    InitI2SSpeakOrMic(0);
    while (1)
    {
      i2s_read(I2S_NUM_0, (char *)(microphonedata0 + data_offset), 1024, &byte_read, (100 / portTICK_RATE_MS));
      data_offset += 1024;

      // 松开M5按键或超时停止录音
      if (digitalRead(37) || data_offset >= 81919)
        break;
    }
    Serial.println("end");
    // 提交录音，若果识别成功返回识别文字
    if (rest.Pcm2String(microphonedata0, data_offset, DEV_PID_MANDARIN, &speakStr) != -1)
    {
      Serial.println("Speech recognition success!");
      Serial.println(speakStr);
      if (speakStr.equals("")) 
      {
        M5TextDisplay("您没有说话！");   // 未说话反馈显示
      } 
      else 
      {
        
        M5TextDisplay(speakStr);  // M5屏幕显示语音识别结果文本
        hc.voice_show_led_digital(speakStr);//led点阵显示
        delay(500);
        Serial.println(speakStr);
      }
      speakFlag = true;
    } else 
    {
      Serial.println("Voice recognition failed!");
      M5TextDisplay("语音识别失败！");
    }
  }
   while(WiFi.status()!=WL_CONNECTED)
  {
    M5.Lcd.setRotation(1);
    M5.Lcd.drawXBitmap(0, 40, network, 80, 80, RED);  // 显示WiFi离线图标
    
    WiFi.begin(ssid,pass);
    delay(500);
    Serial.print(".");
    
  }
   //M5.Lcd.setRotation(0);
   
}
