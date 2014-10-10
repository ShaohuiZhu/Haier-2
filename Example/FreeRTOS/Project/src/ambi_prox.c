#include "ambi_prox.h"

/**************************************************
 * ���������롢���Ӧ�жϽų�ʼ������ʱ����
 * ���룺��
 * �������
 * ���أ�void
***************************************************/
void Ambi_Prox_Gpio_Init(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(AMBI_PROX_INT_CLK, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure Px pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = AMBI_PROX_INT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(AMBI_PROX_INT_GPIO, &GPIO_InitStructure);

  /* Connect EXTI Line3 to Px pin */
  SYSCFG_EXTILineConfig(AMBI_PROX_INT_PORT, EXTI_PinSource3);

  /* Configure EXTI Line3 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI Line3 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**************************************************
 * ������������ID�ţ������ã�uart6�����Ϣ
 * ���룺��
 * �������
 * ���أ�void
***************************************************/
void Ambi_Prox_ReadID(void)
{
	uint8_t id = 0;
	TWI_Initialize();
	ms_Delay(100);
	I2CRdStr(AMBI_PROX_ADDR, REGISTER_ID, &id, 1);
	ms_Delay(100);
	I2CRdStr(AMBI_PROX_ADDR, REGISTER_ID, &id, 1);
	if(id != 0x21)
	{
		//Serial_PutString((uint8_t*)"AMBI_PROX sensor error!");
		while(1);//��ȡʧ�ܣ���ѭ��
	}
	//Serial_PutString((uint8_t*)"AMBI_PROX sensor ID id 0x21!");
}



/**************************************************
 * ���������ú���ƹ�LED����
 * ���룺Current -- ���õ�����С���ο�����飨vcnl4010��
 * �������
 * ���أ�void
***************************************************/
void Set_Current(uint8_t Current)
{
	uint8_t reg, send;
	reg = REGISTER_PROX_CURRENT;
	send = Current;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send, 1);
}

//
/**************************************************
 * ��������ȡ����ƹ�LED����
 * ���룺��
 * �����Current -- ������ţ��ο�����飨vcnl4010��
 * ���أ�void
***************************************************/
void Read_Current(uint8_t *Current)
{
	uint8_t reg, recv;
	I2CRdStr(AMBI_PROX_ADDR, reg, &recv, 1);
	*Current = recv;
}

//��������Ĵ���
void Set_CommandRegister(uint8_t Command)
{
	uint8_t reg, send;
	reg = REGISTER_COMMAND;
	send = Command;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send, 1);
}

//��ȡ����Ĵ���
void Read_CommandRegister(uint8_t *Command)
{
	uint8_t reg, recv;
	reg = REGISTER_COMMAND;
	I2CRdStr(AMBI_PROX_ADDR, reg, &recv, 1);
	*Command = recv;
}


//���þ����Ӧ�����ٶ�
void Set_ProximityRate(uint8_t ProximityRate)
{
	uint8_t reg, send;
	reg = REGISTER_PROX_RATE;
	send = ProximityRate;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send, 1);
}

//���Ӧ�������
void Set_AmbiConfiguration(uint8_t AmbiConfiguration)
{
	uint8_t reg, send;
	reg = REGISTER_AMBI_PARAMETER;
	send = AmbiConfiguration;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send, 1);
}

//�����ж�ģʽ
void Set_InterruptControl(uint8_t InterruptControl)
{
	uint8_t reg, send;
	reg = REGISTER_INTERRUPT_CONTROL;
	send = InterruptControl;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send, 1);
}

//��ȡ�ж�ģʽ
void Read_InterruptControl(uint8_t *InterruptControl)
{
	uint8_t reg, recv;
	reg = REGISTER_INTERRUPT_CONTROL;
	I2CRdStr(AMBI_PROX_ADDR, reg, &recv, 1);
	*InterruptControl = recv;
}

//�����ж�״̬λ����Ҫ�����ж�flag��clear
void Set_InterruptStatus(uint8_t InterruptStatus)
{
	uint8_t reg, send;
	reg = REGISTER_INTERRUPT_STATUS;
	send = InterruptStatus;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send, 1);
}

//��ȡ�ж�״̬�������ж��ж���Դ����Ϣ
void Read_InterruptStatus(uint8_t *InterruptStatus)
{
	uint8_t reg, recv;
	reg = REGISTER_INTERRUPT_STATUS;
	I2CRdStr(AMBI_PROX_ADDR, reg, &recv, 1);
	*InterruptStatus = recv;
}


void Set_ModulatorTimingAdjustment(uint8_t ModulatorTimingAdjustment)
{
	uint8_t reg, send;
	reg = REGISTER_PROX_TIMING;
	send = ModulatorTimingAdjustment;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send, 1);
}

//��ȡ��ǰ����ֵ����λΪcounts��counts��ʵ������
void Read_ProxiValue(uint16_t *ProxiValue)
{
	uint8_t reg, recv[2];
	reg = REGISTER_PROX_VALUE;
	I2CRdStr(AMBI_PROX_ADDR, reg, &recv[0], 2);
	*ProxiValue = recv[1] | (recv[0]<<8);
}


//��ȡ��ǰ����ǿ��ֵ
void Read_AmbiValue(uint16_t *AmbiValue)
{
	uint8_t reg, recv[2];
	reg = REGISTER_AMBI_VALUE;
	I2CRdStr(AMBI_PROX_ADDR, reg, &recv[0], 2);
	*AmbiValue = recv[1] | (recv[0]<<8);
}


//���ø��ж���ֵ
void Set_HighThreshold(uint16_t HighThreshold)
{
	uint8_t reg, send[2];
	reg = REGISTER_INTERRUPT_HIGH_THRES;
	send[0] = HighThreshold >> 8;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send[0], 1);
	reg = REGISTER_INTERRUPT_HIGH_THRES+1;
	send[1] = HighThreshold;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send[1], 1);
}


//���õ��ж���ֵ
void Set_LowThreshold(uint16_t LowThreshold)
{
	uint8_t reg, send[2];
	reg = REGISTER_INTERRUPT_LOW_THRES;
	send[0] = LowThreshold >> 8;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send[0], 1);
	reg = REGISTER_INTERRUPT_LOW_THRES + 1;
	send[1] = LowThreshold;
	I2CWrStr(AMBI_PROX_ADDR, reg, &send[1], 1);
}


//������
void AMBI_PROX_Test(void)
{
	uint8_t c1=0, c2, i = 0, current = 2;
	uint16_t prox, prox1, AmbiValue;
	uint32_t sumprox = 0;
	Set_Current(current);
	Read_Current(&c1);
	if(c1 != current)
	{
		//Serial_PutString((uint8_t*)"Prox current set error!");
		while(1);
	}
	Set_CommandRegister(COMMAND_ALL_DISABLE);							//����ǰʧ��
	Set_ProximityRate(PROX_MEASUREMENT_RATE_2);						//�����Ӧ������������
	Set_CommandRegister(COMMAND_PROX_ENABLE | 						//�����Ӧ���ܴ�
											COMMAND_AMBI_ENABLE |							//���Ӧ���ܴ�
											COMMAND_SELFTIMED_MODE_ENABLE);
	Set_AmbiConfiguration(AMBI_PARA_AVERAGE_32 |
												AMBI_PARA_AUTO_OFFSET_ENABLE |
												AMBI_PARA_MEAS_RATE_2);
	
	//�ȶ�ȡ��������״̬���������жϸ���ֵ������ֵ������
	for(i = 0; i < 30; i++)
	{
	do{
			Read_CommandRegister(&c2);
		}while(!(c2 & COMMAND_MASK_PROX_DATA_READY));
		Read_ProxiValue(&prox);
		sumprox += prox;
	}
	prox = sumprox/30;
	//Serial_PutString((uint8_t*)"Average prox is:");
	//Serial_SendData((uint8_t*)(&prox), 2);
	
	//���ԡ�����6��������Ӧ/���Ӧ��Ϣ
	while(1)
	{
		do{
			Read_CommandRegister(&c2);
		}while(!(c2 & (COMMAND_MASK_PROX_DATA_READY | COMMAND_MASK_AMBI_DATA_READY)));
		if(c2 & COMMAND_MASK_AMBI_DATA_READY)
		{
			Read_AmbiValue(&AmbiValue);
		}
		if(c2 & COMMAND_MASK_PROX_DATA_READY)
		{
			Read_ProxiValue(&prox1);
		}
		//Serial_SendData((uint8_t*)(&AmbiValue), 2);
		//Serial_SendData((uint8_t*)(&prox1), 2);
		ms_Delay(500);
	}
}


/****************************************************************************
 * ���������롢���Ӧ��ʼ��
 * ���룺��
 * �������
 * ���أ�0			--	��ʼ��ʧ��
 * 			 1			--	��ʼ���ɹ�
****************************************************************************/
uint8_t Ambi_Prox_Init(void)
{
	uint8_t command = 0, current = 2;
	TWI_Initialize();
	Set_Current(current);
	Read_Current(&command);
	if(command != current)
	{
		//Serial_PutString((uint8_t*)"Prox current set error!");
		return 0;			//fail
		//while(1);
	}
	Set_CommandRegister(COMMAND_ALL_DISABLE);							//����ǰʧ��
	Set_ProximityRate(PROX_MEASUREMENT_RATE_2);						//�����Ӧ������������
	Set_CommandRegister(COMMAND_PROX_ENABLE | 						//�����Ӧ���ܴ�
											COMMAND_AMBI_ENABLE |							//���Ӧ���ܴ�
											COMMAND_SELFTIMED_MODE_ENABLE);
	Set_AmbiConfiguration(AMBI_PARA_AVERAGE_32 |
												AMBI_PARA_AUTO_OFFSET_ENABLE |
												AMBI_PARA_MEAS_RATE_2);
	return 1;				//done
}


/****************************************************************************
 * ��������ȡ��������״̬���������жϸ���ֵ������ֵ������
 * ���룺��
 * �������
 * ����: �����Ӧ���ƽ��ֵ����λcounts��counts��ʵ������
****************************************************************************/
uint16_t Read_Aver_Prox(void)
{
	uint8_t i = 0, command;
	uint16_t prox = 0;
	uint32_t sumprox = 0;
	
	for(i = 0; i < 30; i++)
	{
	do{
			Read_CommandRegister(&command);
		}while(!(command & COMMAND_MASK_PROX_DATA_READY));
		Read_ProxiValue(&prox);
		sumprox += prox;
	}
	prox = sumprox/30;
	return prox;
}


/****************************************************************************
 * ��������ȡ���ξ���ֵ����λΪcounts��counts��ʵ������
 * ���룺��
 * �������
 * ����: ���ξ����Ӧ�������λcounts��counts��ʵ������
****************************************************************************/
uint16_t Read_single_Prox(void)
{
	uint8_t command = 0;
	uint16_t prox = 0;
	do{
			Read_CommandRegister(&command);
		}while(!(command & COMMAND_MASK_PROX_DATA_READY));
	if(command & COMMAND_MASK_PROX_DATA_READY)
	{
		Read_ProxiValue(&prox);
	}
	return prox;
}


/****************************************************************************
 * ��������ȡ���ι��Ӧֵ����λΪcounts������4�ɵõ�illuminance
 * ���룺��
 * �������
 * ����: ���ι��Ӧ���ֵ
****************************************************************************/
uint16_t Read_single_Ambi(void)
{
	uint8_t command = 0;
	uint16_t ambi = 0;
	do{
			Read_CommandRegister(&command);
		}while(!(command & COMMAND_MASK_AMBI_DATA_READY));
	if(command & COMMAND_MASK_AMBI_DATA_READY)
	{
		Read_AmbiValue(&ambi);
	}
	return ambi;
}


void Ambi_Prox_Test3(void)
{
	char c;
	float temp;
	uint16_t prox, ambi;
	Ambi_Prox_Init();
	while(1)
	{
		//����
		prox = Read_single_Prox();
		debug_printf("Proxi: %d counts,\t", prox);
		
		//���Ӧ
		ambi = Read_single_Ambi();
		temp = (float)ambi/4;
		debug_printf("Ambi: %.2f lx\r\n", temp);
	
		ms_Delay(500);
		//c = debug_getchar (0);
		if(c != 0)
			break;
	}
}
