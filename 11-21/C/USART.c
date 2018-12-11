#include <stm32f10x.h>
#include <stdio.h>
u8 TxCounter=0;
u8 TxBuffer[0xff];
u8 count=0; 
u8 Rx_Buf[50];
u8 Rx_Buf[50];	//两个32字节的串口接收缓存//两个32字节的串口接收缓存
u8 Rx_Act=0;	
u8 Rx_Ok0 = 0;
u8 Rx_Ok1 = 0;
u8 RX_BUF_2[2048];
u8 u_len_2=0;
u8 u_Read_ok_2=0;


void uart1_init (u32 bound)
{
		USART_InitTypeDef USART_InitStructure;
		USART_ClockInitTypeDef USART_ClockInitStruct;
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //开启USART1时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA , &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA , &GPIO_InitStructure);
		USART_InitStructure.USART_BaudRate = bound;       //波特率可以通过地面站配置
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据
		USART_InitStructure.USART_StopBits = USART_StopBits_1;	//在帧结尾传输1个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;    	//禁用奇偶校验
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送、接收使能
		//配置USART1时钟
		USART_ClockInitStruct.USART_Clock = USART_Clock_Disable; //时钟低电平活动
		USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  	//SLCK引脚上时钟输出的极性->低电平
		USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  	//时钟第二个边沿进行数据捕获
		USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出
		USART_Init(USART1, &USART_InitStructure);
		USART_ClockInit(USART1, &USART_ClockInitStruct);
		//使能USART1接收中断
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		//使能USART1
		USART_Cmd(USART1, ENABLE);
}
 
///////////////////////////////////////////////////////////////////////////////////USART2 init////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////USART1 IRQ////////////////////////////////////////////
void USART1_IRQHandler(void)                	//串口1中断服务程序
{

	 if((USART1->SR & (1<<7))&&(USART1->CR1 & USART_CR1_TXEIE))
   if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
	{
		USART1->DR = TxBuffer[TxCounter++]; //写DR清除中断标志          
		if(TxCounter == count)
		{
			USART1->CR1 &= ~USART_CR1_TXEIE;		//关闭TXE中断
			USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
		}
	}

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    
	{
    TIM4->CR1|=1<<0;                    ///Open Timing
  TIM4->CNT=0;	
	USART1->SR &= ~USART_FLAG_RXNE;// clear interrupt
	u8 Res=USART1->DR;
	RX_BUF_2[u_len_2] = USART_ReceiveData(USART1);
  u_len_2++;
	u_Read_ok_2=1; 
  }  		
} 
	
int USART1_SendOneChar(uchar ch)  
{
  while (!(USART1->SR & USART_FLAG_TXE));
  USART1->DR = ch;
  USART1->SR &= ~USART_FLAG_TXE;
  return (ch);
}

//------ 发送字符串--------------
void USART1_SendStr(uchar *str)   
{   
    while( (*str)!='\0' )   
    	{   
        USART1_SendOneChar(*str);   
        str++;   
    	}   
}   


void Uart1_Put_Buf(unsigned char *DataToSend , u8 data_num)
{
	for(u8 i=0;i<data_num;i++)
	{
	TxBuffer[count++] = *(DataToSend+i);
	}
	  if(!(USART1->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 
}

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
  USART1->DR = (u8) ch;      
	return ch;
}
