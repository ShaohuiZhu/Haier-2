#include "DC_Config.h"

/******************************************************************************************
 *	������ ��DC_GPIO_Conf
 *	����   ��void
 *  ����ֵ ��void
 *  ����   ��GPIO�˿ڵĹܽ�����
 *******************************************************************************************/
void DC_GPIO_Conf(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;		                    //����GPIO��ʼ���ṹ��
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);			//ʹ��GPIOB��AHP1ʱ��
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);			//ʹ��GPIOC��AHP1ʱ��
#if GPIO_CHANGE == 1
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);			//ʹ��GPIOC��AHP1ʱ��
#endif
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);			//ʹ��GPIOE��AHP1ʱ��
   
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//���ó�ʼ��ģʽΪ���ģʽ
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						  //�����������Ϊ�������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//��������ٶ�Ϊ100Mhz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;               //��������
   GPIO_InitStructure.GPIO_Pin = DC_OUT_PIN;                  //�����ʼ���Ĺܽ�Ϊ����ܽ�
   GPIO_Init(DC_OUT_PORT, &GPIO_InitStructure);					  			    //��ʼ��GPIO�Ĺܽ�
   
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_Pin = DC_IN_PIN;
   GPIO_Init(DC_IN_PORT,&GPIO_InitStructure);
   
   DC_OUT_PORT-> BSRRH = DC_OUT_PIN;  						 /*	������������õ͵�ƽ	*/
   DC_IN_PORT -> BSRRH = DC_IN_PIN;
   
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_Pin = DC_SWITCH_PIN;
   GPIO_Init(DC_SWITCH_PORT,&GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_Pin = DC_TEMPERATURE_PIN;
   GPIO_Init(DC_TEMPERATURE_PORT,&GPIO_InitStructure);
   
}

/******************************* ***********************************************************
 *	 ������ ��DC_EXIT_Conf()
 *	 ����   ��void
 *   ����ֵ ��void
 *   ����   ���˿��ж����ã�����ǰ�����DC_GPIO_Conf()���������Ź���
 ******************************************************************************************/
void DC_EXTI_Conf(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);          //ʹ��SYSSCFG
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);   //��EXTI_LINE1���ӵ�PC1�˿�

	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;				//�����ش����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTI_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line1);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     //���������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            //��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}

/******************************************************************************************
 *	 ������ ��setElecLevel
 *	 ����   ��uint16_t GPIO_Pin_x, ElecLevel DPFlag
 *   ����ֵ ��void
 *   ����   �����ö˿ڵ�ƽ�ߵ�
 ******************************************************************************************/
void setElecLevel(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x,ElecLevel DPFlag)
{
	if (DPFlag == HIGH)
	{
		GPIOx->BSRRL = GPIO_Pin_x;
	}else 
	{
		GPIOx->BSRRH = GPIO_Pin_x;
	}
}

/******************************************************************************************
 *	������ ��getElecLevel
 *	����   ��uint16_t GPIO_Pin_x
 *  ����ֵ ��DP
 *  ����   ����������˿ڵ�ƽ�ߵ�
 ******************************************************************************************/
ElecLevel getElecLevel(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x)
{
	if ((GPIOx->IDR & GPIO_Pin_x) != RESET )
	{
		return HIGH;
	}
	else
	{
		return LOW;
	}
}

/********************************************************
 * ������ ��GetSwitchStatus
 * ����   ��void 
 * ����ֵ ��SWITCH_STATUS
 * ����   ����ȡ������䲦��״̬��������״̬ΪONʱ����ȡ������䣻ΪOFFʱ������ȡ�������
 ********************************************************/
SWITCH_STATUS GetSwitchStatus(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
	if ( GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x) == 0)
	{
		return ON;
	}
	else return OFF;
}

//�����ǲ��Ի��ţ�������ʾ�����ֽ�
/*******************************************************************************************************************************
 * ������ ��DC_USART_Conf
 * ����   ��void 
 * ����ֵ ��void 
 * ����   ������USART6������ʼ��
 *******************************************************************************************************************************/
void DC_USART_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); //����USART6ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //����GPIOCʱ��
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);//
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);// 
	
	/*����GPIOC*/
	/*����GPIOC_Pin7ΪTX����*/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	/*����GPIOC_Pin6ΪRX���*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	/*����USART6*/
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure);
	
	USART_ClockStructInit(&USART_ClockInitStruct);
	USART_ClockInit(USART6, &USART_ClockInitStruct);
//	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); //ʹ�� USART6�ж�
	USART_Cmd(USART6, ENABLE); //ʹ�� USART6 
	
	USART_ClearFlag(USART6, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */   
}

/********************************************************************************************************************************
*��������DC_USART_SendBuf()
* ������USART_TypeDef* USART6 ��Ҫ���͵��Ĵ��ڵ�ָ�룬u8 *str ָ���͵��ַ�����ָ��
* ����ֵ��void
* ���ܣ���ĳ�����ڷ���һ���ַ���
*********************************************************************************************************************************/
void DC_USART_SendBuf(u8 *buf, u8 length)
{
	u8 i;
	for (i = 0; i < length; ++i)
	{		
		USART_SendData(USART6,buf[i]);																			//��*str��USART6���ͳ�ȥ
		while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET)				//�ȴ��������
		{
		}
	}
}

void DC_USART_PutChar( u8 ch )
{	
	USART_SendData(USART6,(u8)ch);																					//��ch��USART6���ͳ�ȥ
	while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET)				//�ȴ��������
	{
	}
}
