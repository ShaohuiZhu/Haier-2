//ģ���׼I2C
#include "i2c.h"  
  
 
void TWI_Initialize(void);
   
  
/******************************************************************************* 
 * ��������:TWI_Delay                                                                      
 * ��    ��:��ʱ����                                                                      
 *                                                                                
 * ��    ��:��                                                                      
 * ��    ��:��                                                                      
 * ��    ��:��                                                                      
 * ��    ��:                                                                      
 * �޸�����:2010��6��8��                                                                     
 *******************************************************************************/  
void TWI_NOP(void)  
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
 * ��������:TWI_Initialize                                                                      
 * ��    ��:I2C��ʼ������                                                                      
 *                                                                                
 * ��    ��:��                                                                      
 * ��    ��:��                                                                      
 * ��    ��:��                                                                      
 * ��    ��:                                                                      
 * �޸�����:2010��6��8��                                                                     
 *******************************************************************************/  
void TWI_Initialize(void)  
{  
  GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_AHB1PeriphClockCmd(I2C_SDA_CLK | I2C_SCL_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
  //GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 
  GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;  
  GPIO_Init(I2C_SDA_GPIO, &GPIO_InitStructure);  
  
  //GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;  
  GPIO_Init(I2C_SCL_GPIO, &GPIO_InitStructure);  
    
  TWI_SDA_1();  
  TWI_SCL_0();   
}

/******************************************************************************* 
 * ��������:TWI_Initialize                                                                      
 * ��    ��:I2C��ʼ������                                                                      
 *                                                                                
 * ��    ��:��                                                                      
 * ��    ��:��                                                                      
 * ��    ��:��                                                                      
 * ��    ��:                                                                      
 * �޸�����:2010��6��8��                                                                     
 *******************************************************************************/  
void TWI_Init_Tem_Rh(void)  
{  
  GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_AHB1PeriphClockCmd(I2C_SDA_CLK | I2C_SCL_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
  //GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 
  GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;  
  GPIO_Init(I2C_SDA_GPIO, &GPIO_InitStructure);  
  
  //GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;  
  GPIO_Init(I2C_SCL_GPIO, &GPIO_InitStructure);  
    
  TWI_SDA_1();  
  TWI_SCL_0();   
}


/*----------------------------------------------------------------------------*/
//	�ڲ�����----����I2Cͨ��
/*----------------------------------------------------------------------------*/
void I2CStart(void)
{
	TWI_SDA_1();			/*������ʼ�����������ź�*/
	TWI_NOP();
	TWI_SCL_1();			/*������ʼ������ʱ���ź�*/
	TWI_NOP();				//��ʼ��������ʱ�����4.7us
	TWI_SDA_0();			/*������ʼ�ź�*/
	TWI_NOP();				//��ʼ��������ʱ�����4us		
	TWI_SCL_0();			/*ǯסI2C����,׼�����ͻ����*/
	TWI_NOP();
}

/*----------------------------------------------------------------------------*/
//	�ڲ�����----ֹͣI2Cͨ��
/*----------------------------------------------------------------------------*/
void I2CStop(void) //�ͷ�I2C����*/
{
	TWI_SDA_0();			/*����ֹͣ�����������ź�*/
	TWI_NOP();
	TWI_SCL_1();			//����ֹͣ������ʱ���ź�
	TWI_NOP();				//��ʼ��������ʱ�����4us	
	TWI_SDA_1();			/*����I2C����ֹͣ�ź�*/
	TWI_NOP();				/*ֹͣ��������ʱ�����4us*/
}


/*-------------------------------------------------
�ڲ�����--------SendAck(unsigned AckBit) ���ACK 
 		AckBit=0 Ϊ����Ӧ���ź� 
		AckBit=1 Ϊ���ͷ�Ӧ���ź� 
---------------------------------------------------*/
void SendAck(uint8_t AckBit)
{
	/*���͵�Ӧ����Ӧ���ź�*/
	if (AckBit == 0)
	{
		TWI_SDA_0();
	} 
	else
	{
		TWI_SDA_1();
	}
	TWI_NOP();
	TWI_SCL_1();			/*��ʱ����Ϊ��ʹӦ��λ��Ч*/
	TWI_NOP(); 				/*ʱ�Ӹ����ڴ���4us*/
	TWI_SCL_0();			/*��ʱ����ǯסI2C �����Ա���������ֽ����ݻ���ֹͣ����*/
	TWI_NOP();
}

/*------------------------------------------
�ڲ�����---------����һ���ֽڵ�I2C������
	���:��д�������ucdata ;�޷���ֵ 
-------------------------------------------*/
uint8_t I2CSendByte(uint8_t ucdata)
{
	uint8_t bACK;
	uint8_t  i;
	i=8;
	while(i--)			   	/*8 λû�������������*/
	{
		if((ucdata & 0x80)==0x80)
		{
			TWI_SDA_1();		/*������1*/
		}
		else
		{
			TWI_SDA_0();		/*���͵���0*/
		}
		TWI_NOP();
		TWI_SCL_1();			/*��ʱ����Ϊ��֪ͨ��������ʼ��������λ*/
		TWI_NOP();				/*��֤ʱ�Ӹ����ڴ���4us*/	
		TWI_SCL_0();			/*ǯס����׼��������һ������λ*/
		ucdata=ucdata<<1; /*������һλ����*/
		TWI_NOP();
	}
	TWI_NOP();
	TWI_SDA_1();				/*8λ���ݷ�����,�ͷ�I2C����,׼������Ӧ��λ*/
	TWI_NOP();
	TWI_SCL_1();				/*��ʼ����Ӧ���ź�*/
	TWI_NOP();
	if (TWI_SDA_STATE())
	{
		 bACK = 0;				/*�ȴ�Ӧ��,SDA=0ΪӦ��*/
	}
	else
	{
			bACK = 1;				/*�ȴ�Ӧ��,SDA=1ΪӦ��*/
	}	
	TWI_SCL_0();				/*���ͽ���ǯס����׼����һ�����ͻ�������ݻ������������*/
	return(bACK);				/*��ȷӦ�𷵻�0*/
}
	
/*------------------------------------------
�ڲ�����------��I2C����һ���ֽ�
		����ڲ���;���ؽ��յ����ֽ�.
--------------------------------------------*/
uint8_t I2CReadByte(void)
{
	uint8_t i=0,ByteData=0;	
	TWI_SDA_1();				/*��������Ϊ���뷽ʽ*/
	i=8;
	while(i--)
	{
		TWI_NOP();
		TWI_SCL_0();			//������Ϊ��׼����������
		TWI_NOP();				/*ʱ�ӵ����ڴ���4.7us*/	
		TWI_SCL_1();			/*��ʱ����Ϊ��ʹ��������������Ч*/
		TWI_NOP();
		ByteData=ByteData<<1;
		if (TWI_SDA_STATE())
		{
				ByteData++;
		}
	}		
	TWI_SCL_0();				/*8 λ��������ʱ���ߺ�������Ϊ��׼������Ӧ����Ӧ���ź�*/
	return(ByteData);
}

// ������һ���ӵ�ַ
uint8_t I2CRdStr(uint8_t ucSla,uint8_t ucAddress,uint8_t *ucBuf,uint8_t ucCount)
{
	//�����ӵ�ַ��������ȡ����
	uint8_t i=0;
	I2CStart();	
	if(!I2CSendByte(ucSla))
	{
		I2CStop();
		return 0;					/*ѡ�������ĵش�*/
	}
	if(!I2CSendByte(ucAddress))
	{
		I2CStop();
		return 0;					/*ѡ��һ���Ĵ����ش�*/
	}
	I2CStop();					//huangqin
	TWI_NOP();

	I2CStart();
	if(!I2CSendByte(ucSla+1))
	{
		I2CStop();
		return 0;					/*���Ͷ���������*/
	}
	i=ucCount;
	while(i--)
	{
		*ucBuf=I2CReadByte();/*���������Ĵ���*/
		if(i)
			SendAck(0);			/*δ�����������ֽ�,����Ӧ���ź�*/
		ucBuf++;
	}
	SendAck(1);					/*�����������ֽ�,���ͷ�Ӧ���ź�*/
	I2CStop();
	return 1;
}

// д����һ���ӵ�ַ
uint8_t I2CWrStr(uint8_t ucSla, uint8_t ucAddress, uint8_t *ucData, uint8_t ucNo)
{
	uint8_t i;
	I2CStart();
	if(!I2CSendByte(ucSla))
	{
		I2CStop();
		return 0;					/*write command*/
	}
	if(!I2CSendByte(ucAddress))
	{
		I2CStop();
		return 0;					/*write register Address*/
	}
	i=ucNo;
	while(i--)
	{
		if(!I2CSendByte(*ucData))
		{
			I2CStop();
			return 0;				/*write Data*/
		}
		ucData++;
	}
	I2CStop();
	return 1;
}

// ������˫�ֽ��ӵ�ַ
uint8_t I2CRdStr2(uint8_t ucSla,uint16_t usAddress, uint8_t *ucBuf,uint8_t ucCount)
{
	//�����ӵ�ַ��������ȡ����
	uint8_t i=0;

	    I2CStart();	
	    if(!I2CSendByte(ucSla))
	    {
		    I2CStop();
		    return 0;			/*ѡ�������ĵش�*/
	    }
			if(!I2CSendByte((usAddress >> 8) && 0x00FF))
	    //if(!I2CSendByte(usAddress / 256))
	    {
		    I2CStop();
		    return 0;		/*ѡ��һ���Ĵ����ش�*/
	    }
			if(!I2CSendByte(usAddress))
      //if(!I2CSendByte(usAddress % 256))
	    {
		    I2CStop();
		    return 0;		/*ѡ�ڶ����Ĵ����ش�*/
	    }

        I2CStop();//huangqin
        //nop;nop;
        TWI_NOP();

	I2CStart();
	if(!I2CSendByte(ucSla+1))
	{
		I2CStop();
		return 0;							/*���Ͷ���������*/
	}
	i=ucCount;
	while(i--)
	{
		*ucBuf=I2CReadByte();	/*���������Ĵ���*/
		if(i)
			SendAck(0);					/*δ�����������ֽ�,����Ӧ���ź�*/
		ucBuf++;
	}
	SendAck(1);							/*�����������ֽ�,���ͷ�Ӧ���ź�*/
	I2CStop();
	return 1;
}

// д����˫�ֽ��ӵ�ַ
uint8_t I2CWrStr2C(uint8_t ucSla,uint16_t usAddress, uint8_t *ucData, uint8_t ucNo)
{	//8563�ĵ�ַΪ0xa2
	uint8_t i;
	I2CStart();
	if(!I2CSendByte(ucSla))
	{
		I2CStop();
		return 0;		/*write command*/
	}
	if(!I2CSendByte((usAddress >> 8) && 0x00FF))
  //if(!I2CSendByte(usAddress / 256))
	{
		I2CStop();
		return 0;							/*write register Address*/
	}
	if(!I2CSendByte(usAddress))
  //if(!I2CSendByte(usAddress % 256))
	{
		I2CStop();
		return 0;							/*ѡ�ڶ����Ĵ����ش�*/
	}

	i=ucNo;
	while(i--)
	{
		if(!I2CSendByte(*ucData))
		{
			I2CStop();
			return 0;	/*write Data*/
		}
		ucData++;
	}
	I2CStop();
	return 1;
}

// д����˫�ֽ��ӵ�ַ
uint8_t I2CWrStr2(uint8_t ucSla,uint16_t usAddress, uint8_t *ucData, uint8_t ucNo)
{
	uint8_t i;
	uint8_t ucRt;
	uint16_t j;

	for (i = 0; i < ucNo; i++)
	{
		ucRt = I2CWrStr2C(ucSla, (usAddress + i), (ucData + i), 1);
		if (ucRt == 0)
		{
				return 0;
		}
		
		for (j = 60000; j > 0; j--)  // ��ʱҪ�㹻���ܱ�֤д��������
		{
		}
	}

	return 1;
}
