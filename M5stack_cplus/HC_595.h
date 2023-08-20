#ifndef HC_595_H
#define HC_595_H
#include <M5StickCPlus.h>
#define DIN    26//数据输入引脚
#define SRCLK  25//串行输入时钟
#define RCLK   0//锁存输入时钟

class hc_595
{
private:
   void write(byte row,byte col);
 //  void show_digital(unsigned int i,int delay_ms=247);//默认参数默认显示1s

	
public:
	   void init();
     void voice_show_led_digital(String speak_str);
        void show_digital(unsigned int i,int delay_ms=247);//默认参数默认显示1s
};


#endif
