#include"HC_595.h"
unsigned int dig_1[10][8]=
{
  {0x1c,0x22,0x22,0x22,0x22,0x22,0x22,0x1c},//0
  {0x1c,0x08,0x08,0x08,0x08,0x08,0x0c,0x08},//1
  {0x3e,0x04,0x08,0x10,0x20,0x20,0x22,0x1c},//2
  {0x1c,0x22,0x20,0x20,0x18,0x20,0x22,0x1c},//3
  {0x20,0x20,0x3e,0x22,0x24,0x28,0x30,0x20},//4
  {0x1c,0x22,0x20,0x20,0x1e,0x02,0x02,0x3e},//5
  {0x1c,0x22,0x22,0x22,0x1e,0x02,0x22,0x1c},//6
  {0x04,0x04,0x04,0x08,0x10,0x20,0x20,0x3e},//7
  {0x1c,0x22,0x22,0x22,0x1c,0x22,0x22,0x1c},//8
  {0x1c,0x22,0x20,0x3c,0x22,0x22,0x22,0x1c},//9

};
void hc_595::init()
{
	pinMode(DIN, OUTPUT);
	pinMode(SRCLK, OUTPUT);
	pinMode(RCLK, OUTPUT);
	digitalWrite(DIN, LOW);
	digitalWrite(SRCLK, LOW);
	digitalWrite(RCLK, LOW);

}
void hc_595::write(byte row,byte col )
{
	int pin_state;//输出的DIN
	digitalWrite(SRCLK, LOW);
	digitalWrite(RCLK, LOW);
	for (int i = 7; i >= 0; --i)//行按照d7-d0高位到低位对应
	{
		if (row & (1 << i))
		{
			pin_state = 1;

		}
		else
		{
			pin_state = 0;
		}
    digitalWrite(SRCLK, LOW);
		digitalWrite(DIN, pin_state);
		digitalWrite(SRCLK, HIGH);
	}
  for (int i = 7; i >= 0; --i)//列按照d7-d0高位到低位对应
  {
    
    
    if (col & (1 << i))
    {
      pin_state = 1;

    }
    else
    {
      pin_state = 0;
    }
    digitalWrite(SRCLK, LOW);
    digitalWrite(DIN, pin_state);
    digitalWrite(SRCLK, HIGH);

  }
  digitalWrite(RCLK, HIGH);
  delayMicroseconds(500);//数据锁存500us
  digitalWrite(RCLK, LOW); 

}
void hc_595::show_digital(unsigned int i,int delay_ms)//默认参数247
{
  
   for(int t=0;t<delay_ms;++t)
   {
        byte row=0x80;
        for(int j=0;j<8;++j)//执行一遍4.0039ms
        {
          this->write(row,dig_1[i][j]);
         
           row>>=1;
        }
    
   }
    this->write(0x00,0x00);
    digitalWrite(RCLK, HIGH);
    delayMicroseconds(500);//数据锁存500us
   digitalWrite(RCLK, LOW); 
}
//根据语音显示不同数字
//不能用switch ，switch不能用于string
void hc_595::voice_show_led_digital(String speak_str)
{

if(speak_str=="数字零。")
{
   this->show_digital(0);
      return;
}
else if(speak_str=="数字一。")
{
  this->show_digital(1);
      return;
}
else if(speak_str=="数字二。")
{
  this->show_digital(2);
      return;
}
else if(speak_str=="数字三。")
{
  this->show_digital(3);
      return;
}
else if(speak_str=="数字四。")
{
  this->show_digital(4);
      return;
}
else if(speak_str=="数字五。")
{
  this->show_digital(5);
      return;
}
else if(speak_str=="数字六。")
{
  this->show_digital(6);
      return;
}
else if(speak_str=="数字七。")
{
  this->show_digital(7);
      return;
}
else if(speak_str=="数字八。")
{
  this->show_digital(8);
      return;
}
else if(speak_str=="数字九。")
{
  this->show_digital(9);
      return;
}
  return;
  
  
}
