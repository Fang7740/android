#include <stm32f10x.h>
#include <delay.h>

extern u8 u8_PWM_stat,u8_PWM_stat1;
extern u8 u8_PWM_v;
extern u8 u8_PWM_delay;
extern u8 u8_PWM_delay2;
u8 u8_pwm,u8_pwm_old=0;

void PWM(int p,int q)
{

	if(q>p)
	{
		u8_pwm=1;
			  	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	}
	else
	{
		u8_pwm=2;
			  GPIO_SetBits(GPIOB,GPIO_Pin_9);
	}
	if(u8_PWM_stat1==0xf0&&u8_PWM_stat!=0)
	{	
	//----------非电平触发-----------//
		if(q>p)
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	
		}
		else
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
		}
	//----------非电平触发------------//
	}
	
 if(u8_PWM_stat1==0x0f&&u8_PWM_stat!=0)
{	
		if(u8_pwm==1)
		{
			u8_pwm_old=1;
		}	

		if(u8_pwm==2&&u8_pwm_old==1)
		{
			if(u8_PWM_delay!=0)
			    delay_ms(u8_PWM_delay*5); 
			    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			    GPIO_SetBits(GPIOA,GPIO_Pin_0);
					delay_ms(u8_PWM_v*5);
					u8_pwm_old=0;
					u8_pwm=0;
			   GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			   GPIO_SetBits(GPIOA,GPIO_Pin_1);
	
		}
				
				
}
		if(u8_PWM_delay2!=0)
			    delay_ms(u8_PWM_delay2*5); 
	
}
 