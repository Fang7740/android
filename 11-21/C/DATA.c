#include <stm32f10x.h>
#include <stdio.h>
#include <delay.h>
#include "stmflash.h"

//E7 E7 70 00 70 打开串口 
//E7 E7 72 00 72  暂停 //E7 E7 73 00 73
//E7 E7 74 00 74  触发 //E7 E7 75 00 75
//E7 E7 EE 03 00 00 0C FD  发送阀值

//E7 E7 40 01 01 42 修改脉冲宽度
//E7 E7 50 01 01 52 延时时间修改E7 E7 50 01 32 83
//E7 E7 42 01 04 47  控制脉 冲间隔修改
//E7 E7 44 01 05 xx  预触发偏执修改器
//E7 E7 78 00 78 触发方式电平 // E7 E7 76 00 76 非电平
void send(u8 a0,u8 a1,u8 a2,u8 a3,u8 a4,u8 a5,u8 len);

extern u8 u_len_2;
extern u8 RX_BUF_2[2048];
extern u8 x;
u16 FLASH_WRITE_DAT2[1];

int i_valve;
u16 u16_valve;
u8 u8_PWM_stat1=0x0f,u8_PWM_stat=1;
u8 u8_PWM_v=1;
u8 u8_PWM_delay=0;
u8 u8_PWM_delay2=0;

void Data_read()
{
	if(RX_BUF_2[0]==0xE7&&RX_BUF_2[1]==0xE7)
	{  
		if(RX_BUF_2[2]==0x30&&RX_BUF_2[3]==0x00&&RX_BUF_2[4]==0x30)  //打开串口
		{
			delay_ms(10);
			send(0xE7 ,0xE7, 0x31, 0x00, 0x31,0,5);    
			x=1;
		}
		if(RX_BUF_2[2]==0x20&&RX_BUF_2[3]==0x00&&RX_BUF_2[4]==0x20)  //打开串口
		{
			delay_ms(10);
			x=1;
			send(0xE7 ,0xE7, 0x21, 0x00, 0x21,0,5);    
		}
		
	if(RX_BUF_2[2]==0x70&&RX_BUF_2[3]==0x00&&RX_BUF_2[4]==0x70) //暂停
		{
			delay_ms(10);
			send(0xE7 ,0xE7, 0x71, 0x00, 0x71,0,5);
			u8_PWM_stat=0;
		}		
		
		if(RX_BUF_2[2]==0x72&&RX_BUF_2[3]==0x00&&RX_BUF_2[4]==0x72) //自动
		{
			delay_ms(10);
			send(0xE7 ,0xE7, 0x73, 0x00, 0x73,0,5);
			u8_PWM_stat=1;
		}		
		
		if(RX_BUF_2[2]==0xEE&&RX_BUF_2[3]==0x03) //读取阀值//E7 E7 EE 03 02 05 08 00 
		{
     u16_valve=(RX_BUF_2[4]<<8)|(RX_BUF_2[5]<<4)|RX_BUF_2[6];
     i_valve=(int)u16_valve;
		 FLASH_WRITE_DAT2[0]=i_valve;
		 STMFLASH_Write(FA,(u16*)FLASH_WRITE_DAT2,1);
     delay_ms(10);			
     send(0xE7 ,0xE7, 0x77, 0x00, 0x77,0,5);
		}
		
		if(RX_BUF_2[2]==0x74&&RX_BUF_2[3]==0x00&&RX_BUF_2[4]==0x74) //触发
		{
			 delay_ms(10);
       send(0xE7 ,0xE7, 0x75, 0x00, 0x75,0,5);
			 GPIO_SetBits(GPIOA,GPIO_Pin_1);
		   delay_ms(50);
			 GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		}
		
		if(RX_BUF_2[2]==0x50&&RX_BUF_2[3]==0x01) //延时时间修改
		{
		 delay_ms(10);
     send(0xE7 ,0xE7, 0x51, 0x00, 0x51,0,5);
		 u8_PWM_delay=RX_BUF_2[4];
		 FLASH_WRITE_DAT2[0]=u8_PWM_delay;
		 STMFLASH_Write(D1,(u16*)FLASH_WRITE_DAT2,1);
		}	

    if(RX_BUF_2[2]==0x40&&RX_BUF_2[3]==0x01) //修改脉冲宽度
		{
			delay_ms(10);
      send(0xE7 ,0xE7, 0x41, 0x00, 0x41,0,5);
			u8_PWM_v= RX_BUF_2[4];
			FLASH_WRITE_DAT2[0]=u8_PWM_v;
		 STMFLASH_Write(PWM_1,(u16*)FLASH_WRITE_DAT2,1);
		}	
		
    if(RX_BUF_2[2]==0x42&&RX_BUF_2[3]==0x01) //控制脉冲间隔修改
		{
			delay_ms(10);
      send(0xE7 ,0xE7, 0x43, 0x00, 0x43,0,5);
			u8_PWM_delay2= RX_BUF_2[4];
		  FLASH_WRITE_DAT2[0]=u8_PWM_delay2;
		 STMFLASH_Write(D2,(u16*)FLASH_WRITE_DAT2,1);
		}	
		
				
    if(RX_BUF_2[2]==0x44&&RX_BUF_2[3]==0x01) //控制脉冲间隔修改
		{
			delay_ms(10);
      send(0xE7 ,0xE7, 0x45, 0x00, 0x45,0,5);
			u8_PWM_delay2= RX_BUF_2[4];
		  FLASH_WRITE_DAT2[0]=u8_PWM_delay2;
		 STMFLASH_Write(D2,(u16*)FLASH_WRITE_DAT2,1);
		}	
		
    if(RX_BUF_2[2]==0x78&&RX_BUF_2[3]==0x00) //触发方式电平 
		{
			delay_ms(10);
      send(0xE7 ,0xE7, 0x79, 0x00, 0x79,0,5);
			u8_PWM_stat1=0xF0;
			FLASH_WRITE_DAT2[0]=u8_PWM_stat1;
		 STMFLASH_Write(STA,(u16*)FLASH_WRITE_DAT2,1);
		}	
		
    if(RX_BUF_2[2]==0x76&&RX_BUF_2[3]==0x00) //非电平
		{
			delay_ms(10);
      send(0xE7 ,0xE7, 0x7B, 0x00, 0x7B,0,5); 
			u8_PWM_stat1=0x0F;
			FLASH_WRITE_DAT2[0]=u8_PWM_stat1;
		 STMFLASH_Write(STA,(u16*)FLASH_WRITE_DAT2,1);
		}			
     
	}		
			u_len_2=0;
}


