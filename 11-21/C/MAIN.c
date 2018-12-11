#include <stm32f10x.h>
#include <stdio.h>
#include <delay.h>
#include <ADC.H>
#include "stmflash.h"

void tim3_init(u16 time);
void tim4_init(u16 time);
void uart1_init(u32 bound);
void Nvic_Init(void);
void ADC_Config(void);

u16 FLASH_WRITE_DAT0[1];
u16 FLASH_WRITE_DAT01[1];
u16 FLASH_WRITE_DAT02[1];
u16 FLASH_WRITE_DAT03[1];
u16 FLASH_WRITE_DAT04[1];
float FILTER_LOWPASSED(float value);
float ADC_ConvertedValueLocal[NOFCHANEL*2]; 
float V1;
int VV;
u8 data_to_send[50];
void PWM(int p,int q);
void Uart1_Put_Buf(unsigned char *DataToSend , u8 data_num);
extern int i_valve;
extern u16 AD_Value[1][8]; 
extern u8 u8_PWM_stat1,u8_PWM_v,u8_PWM_delay,u8_PWM_delay2;

void init(void)
{
  SystemInit();                  ////////////////系统时钟初始化
	uart1_init(9600);
	delay_init();
	tim3_init(500);
	tim4_init(500);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_0);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOB,GPIO_Pin_9);

	STMFLASH_Read(FA,(u16*)FLASH_WRITE_DAT0,sizeof (FLASH_WRITE_DAT0)); 
	i_valve=FLASH_WRITE_DAT0[0];
	STMFLASH_Read(D1,(u16*)FLASH_WRITE_DAT01,sizeof (FLASH_WRITE_DAT01)); 
	u8_PWM_delay=FLASH_WRITE_DAT01[0];
	STMFLASH_Read(D2,(u16*)FLASH_WRITE_DAT02,sizeof (FLASH_WRITE_DAT02)); 
	u8_PWM_delay2=FLASH_WRITE_DAT02[0];
	STMFLASH_Read(PWM_1,(u16*)FLASH_WRITE_DAT03,sizeof (FLASH_WRITE_DAT03)); 
	u8_PWM_v=FLASH_WRITE_DAT03[0];
	STMFLASH_Read(STA,(u16*)FLASH_WRITE_DAT04,sizeof (FLASH_WRITE_DAT04)); 
	u8_PWM_stat1=FLASH_WRITE_DAT04[0];
//	printf("!!!!!");
	Nvic_Init(); 
	ADC_Config();
}

void send(u8 a0,u8 a1,u8 a2,u8 a3,u8 a4,u8 a5,u8 len)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=a0;
	data_to_send[_cnt++]=a1;
	data_to_send[_cnt++]=a2;
	data_to_send[_cnt++]=a3;
	data_to_send[_cnt++]=a4;
	data_to_send[_cnt++]=a5;
	Uart1_Put_Buf(data_to_send,len);
}


int main()
{
   init();
   TIM3->CR1|=1<<0;                //////////////////打开定时器
	 while(1)
	 {
		 VV=(AD_Value[0][6]); //交流
		 V1= FILTER_LOWPASSED(VV);
		 PWM(i_valve,V1);
		 delay_ms(10);
	 }
}
