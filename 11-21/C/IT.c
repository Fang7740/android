#include <stm32f10x.h>
#include <stdio.h>
#include <delay.h>

#define TIM_IT_Update                      ((uint16_t)0x0001)
#define TIM_FLAG_Update                    ((uint16_t)0x0001)
void Uart1_Put_Buf(unsigned char *DataToSend , u8 data_num);
void Data_read(void);

int T3;
u8 x=0;

extern vs16 _temp;
extern u8 u_Read_ok_2;
extern u8 data_to_send[50];
extern float V1;

void TIM4_IRQHandler(void)		//0.5ms中断一次
{	
	static u8 u_T41=0;	
	if(TIM4->SR & TIM_IT_Update)
	{
    TIM4->SR = ~TIM_FLAG_Update; ///////////清除标志位
    u_T41++;
			if(u_Read_ok_2==1&&u_T41==5)
			{
				u_T41=0;
        Data_read(); 
				u_Read_ok_2=0;
		}
	}
}


void TIM3_IRQHandler(void)		//0.5ms中断一次
{	
	if(TIM3->SR & TIM_IT_Update)
	{
	  TIM3->SR = ~TIM_FLAG_Update; ///////////清除标志位
		T3++;
		if(T3>=30)
		{
			 u8 _cnt=0; u8 sum=0;
		   u16 data=V1;//0x020E
			 data_to_send[_cnt++]=0xE7;
			 data_to_send[_cnt++]=0xE7;
			 data_to_send[_cnt++]=0xAA;
			 data_to_send[_cnt++]=3;
			 data_to_send[_cnt++]=((data&0X0F00)>>8);       
			 data_to_send[_cnt++]=((data&0x000f0)>>4);    
			 data_to_send[_cnt++]=(data&0x0000f); 
			 for(u8 i=0;i<_cnt-2;i++)
			 {
				 sum= sum+data_to_send[i+2];
			 }
			 data_to_send[_cnt++]=sum;
			 if(x==1)
			 Uart1_Put_Buf(data_to_send,_cnt);
			 T3=0;
		 }	
		}
}

