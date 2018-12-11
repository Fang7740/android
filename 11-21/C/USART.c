#include <stm32f10x.h>
#include <stdio.h>
u8 TxCounter=0;
u8 TxBuffer[0xff];
u8 count=0; 
u8 Rx_Buf[50];
u8 Rx_Buf[50];	//����32�ֽڵĴ��ڽ��ջ���//����32�ֽڵĴ��ڽ��ջ���
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
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //����USART1ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA , &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA , &GPIO_InitStructure);
		USART_InitStructure.USART_BaudRate = bound;       //�����ʿ���ͨ������վ����
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8λ����
		USART_InitStructure.USART_StopBits = USART_StopBits_1;	//��֡��β����1��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;    	//������żУ��
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
		//����USART1ʱ��
		USART_ClockInitStruct.USART_Clock = USART_Clock_Disable; //ʱ�ӵ͵�ƽ�
		USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  	//SLCK������ʱ������ļ���->�͵�ƽ
		USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  	//ʱ�ӵڶ������ؽ������ݲ���
		USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
		USART_Init(USART1, &USART_InitStructure);
		USART_ClockInit(USART1, &USART_ClockInitStruct);
		//ʹ��USART1�����ж�
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		//ʹ��USART1
		USART_Cmd(USART1, ENABLE);
}
 
///////////////////////////////////////////////////////////////////////////////////USART2 init////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////USART1 IRQ////////////////////////////////////////////
void USART1_IRQHandler(void)                	//����1�жϷ������
{

	 if((USART1->SR & (1<<7))&&(USART1->CR1 & USART_CR1_TXEIE))
   if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
	{
		USART1->DR = TxBuffer[TxCounter++]; //дDR����жϱ�־          
		if(TxCounter == count)
		{
			USART1->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�ж�
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

//------ �����ַ���--------------
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
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
  USART1->DR = (u8) ch;      
	return ch;
}
