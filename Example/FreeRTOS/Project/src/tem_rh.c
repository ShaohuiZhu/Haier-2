//ʹ��ģ��I2C
#include "tem_rh.h"

/******************************************************************************* 
 * ��������:Tem_Rh_Delay                                                                      
 * ��    ��:��ʱ����                                                                      
 *                                                                                
 * ��    ��:��                                                                      
 * ��    ��:��                                                                      
 * ��    ��:��                                                                      
 * ��    ��:                                                                      
 * �޸�����:2010��6��8��                                                                     
 *******************************************************************************/  
void Tem_Rh_NOP(void)  
{
 vu32 i, j;  
 vu32 sum = 0;  
 i = 5;  
 while(i--)  
 {
     for (j = 0; j < 100; j++)    // 10
     sum += i;   
 }
 i = sum;
 //sum = i;  
}
  
/******************************************************************************* 
 * ��������:Tem_Rh_Initialize                                                                                                                                                                                                          
 * ��    ��:��                                                                      
 * ��    ��:��                                                                      
 * ��    ��:��                                                                                                                                                                                                        
 *******************************************************************************/  
void Tem_Rh_Gpio_Init(void)  
{  
  GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_AHB1PeriphClockCmd(TEM_RH_SDA_CLK | TEM_RH_SCL_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
  //GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 
  GPIO_InitStructure.GPIO_Pin = TEM_RH_SDA_PIN;  
  GPIO_Init(TEM_RH_SDA_GPIO, &GPIO_InitStructure);  
  
  //GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Pin = TEM_RH_SCL_PIN;  
  GPIO_Init(TEM_RH_SCL_GPIO, &GPIO_InitStructure);  
    
  TEM_RH_SDA_1();  
  TEM_RH_SCL_0();   
}


/*----------------------------------------------------------------------------*/
//	�ڲ�����----����TEM_RHͨ��
/*----------------------------------------------------------------------------*/
void TEM_RH_Start(void)
{
	//SDA=1;		/*������ʼ�����������ź�*/
    TEM_RH_SDA_1();
	//nop;nop;nop;nop;nop;
    Tem_Rh_NOP();

	//SCL=1;		/*������ʼ������ʱ���ź�*/
    TEM_RH_SCL_1();
	//nop;nop;nop;nop;nop;   //��ʼ��������ʱ�����4.7us
    Tem_Rh_NOP();
    
	//SDA=0;		/*������ʼ�ź�*/
    TEM_RH_SDA_0();
	//nop;nop;nop;nop;nop; //��ʼ��������ʱ�����4us
    Tem_Rh_NOP();

	//SCL=0;		/*ǯסTEM_RH����,׼�����ͻ����*/
    TEM_RH_SCL_0();
	//nop;nop;nop;nop;nop;
    Tem_Rh_NOP();
}

/*----------------------------------------------------------------------------*/
//	�ڲ�����----ֹͣTEM_RHͨ��
/*----------------------------------------------------------------------------*/
void TEM_RH_Stop(void) //�ͷ�TEM_RH����*/
{
	//SDA=0;		/*����ֹͣ�����������ź�*/
    TEM_RH_SDA_0();
	//nop;nop;nop;nop;nop;
    Tem_Rh_NOP();
	//SCL=1;		//����ֹͣ������ʱ���ź�
    TEM_RH_SCL_1();
	//nop;nop;nop;nop;nop; //��ʼ��������ʱ�����4us
    Tem_Rh_NOP();

	//SDA=1;		/*����TEM_RH����ֹͣ�ź�*/
    TEM_RH_SDA_1();
	//nop;nop;nop;nop;nop;/*ֹͣ��������ʱ�����4us*/
    Tem_Rh_NOP();
}


/*-------------------------------------------------
�ڲ�����--------Tem_Rh_SendAck(unsigned AckBit) ���ACK 
 		AckBit=0 Ϊ����Ӧ���ź� 
		AckBit=1 Ϊ���ͷ�Ӧ���ź� 
---------------------------------------------------*/
void Tem_Rh_SendAck(uint8_t AckBit)
{
	//SDA=AckBit;		/*���͵�Ӧ����Ӧ���ź�*/
    if (AckBit == 0)
    {
        TEM_RH_SDA_0();
    } 
    else
    {
        TEM_RH_SDA_1();
    }
	//nop;nop;nop;nop;nop;
    Tem_Rh_NOP();
	//SCL=1;			/*��ʱ����Ϊ��ʹӦ��λ��Ч*/
    TEM_RH_SCL_1();
	//nop;nop;nop;nop;nop;   /*ʱ�Ӹ����ڴ���4us*/
    Tem_Rh_NOP();
	//SCL=0;			/*��ʱ����ǯסTEM_RH �����Ա���������ֽ����ݻ���ֹͣ����*/
    TEM_RH_SCL_0();
	//nop;nop;nop;nop;nop;
    Tem_Rh_NOP();
}

/*------------------------------------------
�ڲ�����---------����һ���ֽڵ�TEM_RH������
	���:��д�������ucdata ;�޷���ֵ 
-------------------------------------------*/
uint8_t TEM_RH_SendByte(uint8_t ucdata)
{
	uint8_t bACK;
	uint8_t  i;
	i=8;
	while(i--)			   	/*8 λû�������������*/
	{
		if((ucdata & 0x80)==0x80)
        {
			//SDA=1;		/*������1*/
            TEM_RH_SDA_1();
        }
		else
        {
			//SDA=0;		/*���͵���0*/
            TEM_RH_SDA_0();
        }
		//nop;nop;nop;nop;nop;
        Tem_Rh_NOP();
		//SCL=1;			/*��ʱ����Ϊ��֪ͨ��������ʼ��������λ*/
        TEM_RH_SCL_1();
		//nop;nop;nop;nop;nop;  /*��֤ʱ�Ӹ����ڴ���4us*/
        Tem_Rh_NOP();
		//SCL=0;			/*ǯס����׼��������һ������λ*/
        TEM_RH_SCL_0();
		ucdata=ucdata<<1;  /*������һλ����*/
        //nop;nop;
        Tem_Rh_NOP();
	}
	//nop;nop;nop;nop;nop;
    Tem_Rh_NOP();
	//SDA=1;			/*8λ���ݷ�����,�ͷ�TEM_RH����,׼������Ӧ��λ*/
    TEM_RH_SDA_1();
	//nop;nop;nop;nop;nop;
    Tem_Rh_NOP();
	//SCL=1;			/*��ʼ����Ӧ���ź�*/
    TEM_RH_SCL_1();
	//nop;nop;nop;nop;nop;
    Tem_Rh_NOP();
    // ?????????????????????????????????????????????????????????????????????????
	//if(SDA)		/*�ȴ�Ӧ��,SDA=0ΪӦ��*/      
	//	bACK=0;
	//else
	//	bACK=1;
    if (TEM_RH_SDA_STATE())
    {
       bACK = 0;
    }
	//while(!TEM_RH_SDA_STATE());
	//bACK = 0;
    else
    {
        bACK = 1;
    }
    
	//SCL=0;		/*���ͽ���ǯס����׼����һ�����ͻ�������ݻ������������*/
    TEM_RH_SCL_0();
	return(bACK);		/*��ȷӦ�𷵻�0*/
}
	
/*------------------------------------------
//��������TEM_RH����һ���ֽ�
//���룺��
//�������
//���أ����յ��ֽڸ���
--------------------------------------------*/
uint8_t TEM_RH_ReadByte(void)
{
	uint8_t i=0,ByteData=0;
	//SDA=1;			/*��������Ϊ���뷽ʽ*/
    TEM_RH_SDA_1();
	i=8;
	while(i--)
	{
	   	//nop;nop;nop;nop;
        Tem_Rh_NOP();
		//SCL=0;		//������Ϊ��׼����������
        TEM_RH_SCL_0();
	//	Delay(3);
		//nop;nop;nop;nop;nop; /*ʱ�ӵ����ڴ���4.7us*/
        Tem_Rh_NOP();
		//SCL=1;		/*��ʱ����Ϊ��ʹ��������������Ч*/
        TEM_RH_SCL_1();
		//nop;nop;nop;nop;nop;
        Tem_Rh_NOP();
		ByteData=ByteData<<1;
		//if(SDA)
		//	ByteData++;
        if (TEM_RH_SDA_STATE())
        {
            ByteData++;
        }
	}
	//SCL=0;			/*8 λ��������ʱ���ߺ�������Ϊ��׼������Ӧ����Ӧ���ź�*/
    TEM_RH_SCL_0();
	return(ByteData);
}


/****************************************************************************
 * ������������һ���ӵ�ַ
 * ���룺ucSla			--	I2C�豸��ַ
 * 			 ucAddress 	--	����/�������Ĵ���
 * 			 ucNo				--	���������ݸ���
 * �����ucData			--	�������ݴ��
 * ���أ�0					--	����ʧ��
 *			 1					--	�����ɹ�
****************************************************************************/
uint8_t TEM_RH_RdStr(uint8_t ucSla,uint8_t ucAddress, uint8_t *ucBuf,uint8_t ucCount)
{
	//�����ӵ�ַ��������ȡ����
	// PCF8563�ĵ�ַΪ0xa2,
	uint16_t waittime = TEM_RH_WAIT_DATA_READY_TIME;
	uint8_t i=0;

	TEM_RH_Start();	
	if(!TEM_RH_SendByte(ucSla))
	{
		TEM_RH_Stop();
		return 0;			/*ѡ�������ĵش�*/
	}
	if(!TEM_RH_SendByte(ucAddress))
	{
		TEM_RH_Stop();
		return 0;		/*ѡ��һ���Ĵ����ش�*/
	}
		
	TEM_RH_Stop();//huangqin
	//nop;nop;
	
	while(--waittime && (!TEM_RH_SendByte(ucSla+1)))
	{
		Tem_Rh_NOP();
		Tem_Rh_NOP();
		Tem_Rh_NOP();

		TEM_RH_Start();
	}
	
	if(waittime == 0)
	{
		return 0;//�涨ʱ����û�ж����������ݣ�ʧ��
	}
	
	i=ucCount;
	while(i--)
	{
		*ucBuf=TEM_RH_ReadByte();/*���������Ĵ���*/
		if(i)
			Tem_Rh_SendAck(0);		/*δ�����������ֽ�,����Ӧ���ź�*/
		ucBuf++;
	}
	Tem_Rh_SendAck(1);			/*�����������ֽ�,���ͷ�Ӧ���ź�*/
	TEM_RH_Stop();
	return 1;
}


/****************************************************************************
 * ������д����һ���ֽ��ӵ�ַ
 * ���룺ucSla			--	I2C�豸��ַ
 * 			 ucAddress 	--	����/�������Ĵ���
 * 			 ucData			--	��д������
 * 			 ucNo				--	��д�����ݸ���
 * �������
 * ���أ�0					--	д��ʧ��
 * 			 1					--	д��ɹ�
****************************************************************************/
uint8_t TEM_RHWrStr(uint8_t ucSla, uint8_t ucAddress, uint8_t *ucData, uint8_t ucNo)
{	//8563�ĵ�ַΪ0xa2
	uint8_t i;
	TEM_RH_Start();
	if(!TEM_RH_SendByte(ucSla))
	{
		TEM_RH_Stop();
		return 0;		/*write command*/
	}
	if(!TEM_RH_SendByte(ucAddress))
	{
		TEM_RH_Stop();
		return 0;	/*write register Address*/
	}
	i=ucNo;
	while(i--)
	{
		if(!TEM_RH_SendByte(*ucData))
		{
			TEM_RH_Stop();
			return 0;	/*write Data*/
		}
		ucData++;
	}
	TEM_RH_Stop();
	return 1;
}



/**************************************************
 * ��������λ������ע����λ����Ҫһ����ʱ��֤������������
 * ���룺��
 * �������
 * ���أ�void
 * ���϶� = (175.72*(float)Temper)/65536 - 46.85;
***************************************************/
uint8_t Tem_Rh_SoftRestart(void)
{
	TEM_RH_Start();
	if(!TEM_RH_SendByte(TEM_RH_ADDR))
	{
		TEM_RH_Stop();
		return 0;		/*write command*/
	}
	if(!TEM_RH_SendByte(TEM_RH_RESET_COMMAND))
	{
		TEM_RH_Stop();
		return 0;	/*write register Address*/
	}
	TEM_RH_Stop();
	return 1;
}

/**************************************************
 * ��������ȡ�¶�ֵ���辭����õ����϶�
 * ���룺��
 * �����Temper���¶�ԭʼֵ
 * ���أ�void
 * ���϶� = (175.72*(float)Temper)/65536 - 46.85;
***************************************************/
void Tem_Rh_ReadTem(uint16_t *Temper)
{
	uint8_t temp[2];
	Tem_Rh_Gpio_Init();							//ģ��I2C�ܽų�ʼ��
	Tem_Rh_NOP();
	Tem_Rh_SoftRestart();
	ms_Delay(20);										//��ʱ��֤��λ�ɹ�
	TEM_RH_RdStr(TEM_RH_ADDR, TEM_MEAS_COMMAND, &temp[0], 2);
	temp[1] &= 0xfc;								//����λ���㣨Ĭ�ϲ������Ϊ14bit��
	*Temper = (uint16_t)(temp[0]<<8) + temp[1];
}


/**************************************************
 * ��������ȡʪ��ֵ���辭����õ����ʪ��
 * ���룺��
 * �����Rh��ʪ��ԭʼֵ
 * ���أ�void
 * ���ʪ�ȣ�%�� = 125*Rh/65536 - 6;
***************************************************/
void Tem_Rh_ReadRh(uint16_t *Rh)
{
	uint8_t temp[2];
	Tem_Rh_Gpio_Init();							//ģ��I2C�ܽų�ʼ��
	Tem_Rh_NOP();
	Tem_Rh_SoftRestart();
	ms_Delay(20);										//��ʱ��֤��λ�ɹ�
	TEM_RH_RdStr(TEM_RH_ADDR, RH_MEAS_COMMAND, &temp[0], 2);
	temp[1] &= 0xfc;								//����λ����
	*Rh = (uint16_t)(temp[0]<<8) + temp[1];
}


//���������Ժ���3
void Tem_Rh_Test3(void)
{
	char c;
	double temp;
	uint16_t t = 0, r = 0;
	Tem_Rh_Gpio_Init();
	debug_printf("Temperature & Humidity test begin!\r\n");
	while(1)
	{
		//�����¶Ȳ���ӡ
		Tem_Rh_ReadTem(&t);
		temp = (float)t;
		temp = (float)(175.72*temp/65536 - 46.85);
		debug_printf("Temperature: %.1f'C\t", temp);
		
		
		//����ʪ�Ȳ���ӡ
		Tem_Rh_ReadRh(&r);
		temp = (float)r;
		temp = 125*temp/65536 - 6;
		debug_printf("Humidity: %.1f%%\r\n", temp);
		
		ms_Delay(1000);
		//c = debug_getchar (0);
		if(c == 0)
		{
			continue;
		}
		else if(c == 't' || c == 'T')
		{
			debug_printf ("\nCmd> "); 
			debug_printf("%c\r\n", c);
			ms_Delay(200);
			debug_printf("Temperature & Humidity test continue!\r\n");
		}
		else
		{
			debug_printf ("\nCmd> "); 
			debug_printf("%c\r\n", c);
			ms_Delay(200);
			debug_printf("Temperature & Humidity test terminates!\r\n");
			break;
		}
	}
}
