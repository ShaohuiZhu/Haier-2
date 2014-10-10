#include "DC_Protocol.h"

static BYTE cmd[LENGTH_OF_SEND_FRAME];
static BYTE STORE_CMD[LENGTH_OF_SEND_FRAME];
static BYTE RECEIVED_CMD[LENGTH_OF_RECEIVE_FRAME];
static __IO MachineStatus mStatus;		//�ڻ�״̬�����ڱ����ڻ�������Ϣ
static __IO u8 IS_INIT = 1;				//	�Ƿ��ڳ�ʼ���׶�
static BYTE DEFAULT_CMD[6] = {0X00,0X88,0X05,0X26,0XFF,0x00};   //	�޵������ʱ��Ĭ��ֵ
__IO    CONTROL_STATUS CFlag = IDLE_STATUS;  				//	�߿���״̬	
__IO u8 TIM_CAP_SET[LENGTH_OF_RECEIVE_FRAME * 8 + 1];
__IO u8 BUS_STAYUS = 0;  //	0:���У�1��æ	

/*	�����뿪�ش򿪣������߿������¶�Ϊ׼���������ڻ��¶�Ϊ׼	*/
//SWITCH_STATUS TEMPRATURE_MAIN = GetSwitchStatus(DC_TEMPERATURE_PORT,DC_TEMPERATURE_PIN);
SWITCH_STATUS TEMPRATURE_MAI = ON;

static void DC_InitComm(void);
static void Frame_ms(u32 time, ElecLevel DPFlag);   /*	ά��time����ĸߵ͵�ƽ						*/
static void CmdCopy(BYTE dest[],BYTE src[]);        /*	�������ֽڴ�src[]���Ƶ�dest[] 		*/
static BYTE CheckFunction(BYTE buf[],u8 DataLen);		/*	У�鷢�������										*/
static void SendCmdToMachine(void);      	 					/*  ���������ֽڵ��ڻ� 								*/
static u8   CheckReceivedCmd(void);					 				/*	У����������У����ȷ���򷵻�1	*/
static void AnalysisCmd(void);            					/*	��������													*/
void ReadCmdFromMachine(void);    		     					/*  ���ڻ��������ֽ�,�����ж��е���   */
void TaskReceive(void *p_arg);
void SendCallback( xTimerHandle pxTimer);
void MainCallback( xTimerHandle pxTimer);

extern xTimerHandle xTimerMain;
extern xTimerHandle xTimerInit;
extern xTimerHandle xTimerSend;

/*********************************************************
 * ��������DC_Config()
 * ������ void 
 * ����ֵ��void
 * ���ܣ���ʼ��ֱ��Э������
 ********************************************************/
void DC_Config(void)
{
	DC_GPIO_Conf();
	DC_EXTI_Conf();
	DC_USART_Conf();
	DC_InitTimer();  //��ʼ�����������ʱ��
	DC_InitComm();	 //��ʼ�������ֽ�
}

void DC_InitComm(void)
{
	/*	A:0X00,B:0X08,C:0X01,D:0X26,E:0XFF	 */
	CmdCopy(cmd,DEFAULT_CMD);

//	if( ON == GetSwitchStatus(DC_SWITCH_PORT,DC_SWITCH_PIN) )  		// 
//	{
//		cmd[0] = 0;
//		cmd[0] |= (MEMORY_DATA->OOFlag << 7);
//		cmd[0] |= (MEMORY_DATA->WDFlag << 6);
//		cmd[0] |= (MEMORY_DATA->HeatFlag << 4);
//		cmd[0] |= (MEMORY_DATA->HeatFlag << 3);
//		cmd[0] |= MEMORY_DATA->Mode;
//		
//		cmd[1] &= ~POS_D05;
//		cmd[1] |= MEMORY_DATA->Temperature;
//		cmd[1] |= (MEMORY_DATA->WindSpeed << 4);
//		
//		cmd[2] &= ~POS_D45;
//		cmd[2] |= (MEMORY_DATA->FreshAir << 4);
//	}
	cmd[LENGTH_OF_SEND_FRAME-1] = CheckFunction(cmd,LENGTH_OF_SEND_FRAME-1);
}
/*********************************************************    
 * ������ ��CmdCopy()
 * ����   ��BYTE *dest�� BYTE *src
 * ����ֵ ��void
 * ����   ����src[]�ڵ������ֽڸ��Ƶ�dest[]   
 *********************************************************/
static void CmdCopy(BYTE dest[],BYTE src[])
{
	u8 i;
	for(i = 0; i < LENGTH_OF_SEND_FRAME-1; ++i)
		dest[i] = src[i];
}

/********************************************************
 *������ ��CheckFunction
 *����   ��BYTE buf[]
 *����ֵ ��BYTE 
 *����   ��У��buf[]������У��ֵ
 ********************************************************/
static BYTE CheckFunction(BYTE buf[] ,u8 DataLen)
{
	BYTE res = 0;
	u8 i;
	for(i = 0; i < DataLen ;++i) res += buf[i];
	return (BYTE)res;
}


/********************************************************
 * ������ ��Frame_ms
 * ����   ��u32 time, ElecLevel DPFlag
 * ����ֵ ��void
 * ����   ��ά�ָߵ͵�ƽtime����
 ********************************************************/
static void Frame_ms(u32 time, ElecLevel DPFlag)
{
	setElecLevel(DC_OUT_PORT,DC_OUT_PIN,DPFlag);
	DC_DelayMS(time);						/*	������ռMPU�����޸�	*/
}

/********************************************************
 * ������ ��SendCmdToMachine()
 * ����   ��void
 * ����ֵ ��void
 * ����   ���������ֽ�ת��Ϊ���Ÿߵ͵�ƽ
 *******************************
 *���ݸ�ʽ: LSB
 *ͨ��ԭ��:
 * ������ : 24ms ��
 *	  0   : 12ms ��
 *	  1   :  4ms ��  
 *	 ��� :  4ms ��
 *******************************
 ********************************************************/
static void SendCmdToMachine(void)
{
	u8 i,j;
	CFlag = SENDING_STATUS;		//�߿�������״̬
	BUS_STAYUS = 1;				//����æ
	EXTI->IMR &= ~EXTI_Line1;	//�ر�һ�����ж�
	if ( xTimerIsTimerActive(xTimerSend) != pdFALSE)
	{
		xTimerStop(xTimerSend,0);  //ֹͣ���Ͷ�ʱ����
	}
	
	Frame_ms(GUIDANCE_MS,HIGH);
	for (i = 0; i < LENGTH_OF_SEND_FRAME; ++i)
	{
		for (j = 0;j < 8; ++j)
		{
			Frame_ms(INTERNAL_MS,LOW);
			if(cmd[i] & (0x01 << j))
			{
				Frame_ms(NUM1_MS,HIGH);
			}else
			{
				Frame_ms(NUM0_MS,HIGH);
			}
		}
	}
	setElecLevel(DC_OUT_PORT,DC_OUT_PIN,LOW);
	CFlag = WAIT_TO_RECEIVE;	//�߿����ȴ����ջ���
	BUS_STAYUS = 0;				//������
	EXTI->IMR |= EXTI_Line1;		//��һ�����ж�
	if (IS_INIT == 0 && xTimerIsTimerActive(xTimerSend) == pdFALSE)
	{
		if(xTimerSend != NULL) xTimerStart(xTimerSend,0);
	}
}

/********************************************************
 * ������ ��ReadCmdFromMachine
 * ����   ��void
 * ����ֵ ��void
 * ����   �������ڻ������������ֽڣ������յ��ֽڴ浽RECEIVED_CMD[]��
 *          ���ǵ�Э����չ����ҪԤ������ʱ�䣬�ӳ�1s
 ********************************************************/
void ReadCmdFromMachine(void) 
{
	uint32_t count;
	u8 i = 0;
	
	EXTI->IMR &= ~EXTI_Line1;
	CFlag = BEGIN_TO_RECEIVE;	//�߿���״̬��Ϊ��ʼ����״̬
	BUS_STAYUS = 1;  			//���ջ���ʱ��������æ
	
	if ( xTimerIsTimerActive(xTimerSend) != pdFALSE)
	{
		xTimerStop(xTimerSend,0);  //ֹͣ���Ͷ�ʱ����
	}
	
	count = 0;
	DC_DelayMS(1); //�ȴ���ƽ�ȶ�
	while(getElecLevel(DC_IN_PORT,DC_IN_PIN) != LOW)
	{
		++count;
		DC_DelayMS(1);
	}
//	DC_USART_PutChar(count);	
	
	if (( count >= GUIDANCE_MS && count - GUIDANCE_MS <= 2 ) || (count < GUIDANCE_MS && GUIDANCE_MS-count <= 2)) 
	{
		memset(RECEIVED_CMD,0,sizeof(RECEIVED_CMD)); 
		for ( ; i < LENGTH_OF_RECEIVE_FRAME * 8; ++i)
		{
			DC_DelayMS(1);
			while(getElecLevel(DC_IN_PORT,DC_IN_PIN) != HIGH)
			{
				DC_DelayMS(1);
			}
			
			count = 0;
			while(getElecLevel(DC_IN_PORT,DC_IN_PIN) != LOW)
			{
				++count;
				DC_DelayMS(1);				
			}
//			DC_USART_PutChar(count);
				
			if ((count >= NUM1_MS && count-NUM1_MS <=2) || (count < NUM1_MS && NUM1_MS-count <=2))
			{
				RECEIVED_CMD[i / 8] |= (0x01<<(i % 8));
			}
			else if ((count >= NUM0_MS && count-NUM0_MS <=2) || (count < NUM0_MS && NUM0_MS-count <=2))
			{
						//��Ӧ��λ�Ѿ���0
			}else
			{
				CFlag = RECEIVE_ERROR;
			}
		}
	}else
	{
		CFlag = RECEIVE_ERROR;
		DC_DelayMS(640); //����������������Ļ���
	}
	
	DC_USART_SendBuf(RECEIVED_CMD,5);
	
	if (CFlag == RECEIVE_ERROR)
	{
		//ʲô������
	}else
	{
		//	����֡����		
		if (1 == CheckReceivedCmd())
		{
			CmdCopy(STORE_CMD,cmd); 			//	����������洢�������´ο�������
			AnalysisCmd();								//	У����ȷ����������������߿���
			CFlag = RECEIVE_COMPLETED;  	//	���ݽ������
			if (IS_INIT) 									// �ж��Ƿ����ϵ��λ
			{
				if( xTimerIsTimerActive( xTimerMain ) != pdFALSE )
				{
					xTimerDelete(xTimerMain,0);  //ɾ����4�������� 
				}
				if( xTimerIsTimerActive( xTimerInit ) != pdFALSE )
				{
					xTimerDelete(xTimerInit,0);  //ɾ��2������
				}
//				xTimerSend = xTimerCreate("TimerSend",(200    / portTICK_RATE_MS),pdTRUE ,(void *)2,SendCallback);
				IS_INIT = 0;
			}
		}
		else
		{
			//	У�����
			CFlag =  CHECK_ERROR;
		}
	}
	
//	EXTI_ClearITPendingBit(EXTI_Line1);
	EXTI->IMR |= EXTI_Line1;
	BUS_STAYUS = 0;
	if (IS_INIT == 0 && xTimerIsTimerActive(xTimerSend) == pdFALSE)
	{
		if(xTimerSend != NULL) xTimerStart(xTimerSend,0);
	}
}

/********************************************************
 * ������ ��CheckReceivedCmd
 * ����   ��void
 * ����ֵ ��u8
 * ����   ����������Ƿ���ȷ����ȷ�򷵻�1�����򷵻�0
 ********************************************************/
static u8 CheckReceivedCmd(void)
{
	if (CheckFunction(RECEIVED_CMD,LENGTH_OF_RECEIVE_FRAME-1) == \
	   RECEIVED_CMD[LENGTH_OF_RECEIVE_FRAME-1])
	{
		return 1;
	}else 
	{
		return 0;
	}
}

/*	�����ã����޸�	*/
/***************************************************
 * ������isOn --- �Ƿ�ػ���0���ػ�   1������
 *      mode --- ģʽ��     1������   2����ʪ   4������
 *        temperature --- �趨�¶ȣ�0 --- 16  ...... 14----30
 *	       windSpeed --- ���٣�0���Զ�    1���ͷ�    2���з�    3���߷� 
 *       PAN --- �Ƿ�ڷ�    0��ֹͣ�ڷ�    1�� ��ʼ�ڷ�
 ***************************************************/
void DC_Control(u8 isOn, u8 mode ,u8 temperature, u8 windSpeed,u8 PAN)
{
//	CmdCopy(cmd,STORE_CMD); 			/*	��ǰһ�εĿ�������Ƶ�cmd��*/
	CmdCopy(cmd,DEFAULT_CMD);
	
	cmd[0] |= (isOn << 7);
	cmd[0] |= (PAN << 6);
	cmd[0] |= mode;
	
	if (mode == HeatMode )  //����ģʽ
	{
		cmd[0] |= POS_D3;  //���ȿ�
	}
	
	cmd[1] &= ~POS_D05;
	if(temperature >= 30)
	{
		cmd[1] |= 0x0E;
	}
	else if (temperature <= 16)
	{
		//	D03�Ѿ���0
	}
	else
	{
		cmd[1] |= (temperature - 16);
	}
	cmd[1] |= (windSpeed << 4);

/*	
	if (TEMPRATURE_MAIN == ON)
	{
		//	��ȡ�߿������¶ȣ�д���ֽ�E��
	}
*/	
	cmd[5] = CheckFunction(cmd,LENGTH_OF_SEND_FRAME-1);
	
//	HAS_CMD_TO_SEND = 1;
	SendCmdToMachine();
}

/**********************************************************
 * ���Է�һ֡���ݣ������߿�����һ֡����һ�����Աȿ���û�л���
 **********************************************************/
void DC_Test(void)
{
	cmd[0] = 0X01;
	cmd[1] = 0XA2;
	cmd[2] = 0X00;
	cmd[3] = 0X26;
	cmd[4] = 0XDB;
	cmd[5] = CheckFunction(cmd,LENGTH_OF_SEND_FRAME-1);  //cmd[5] = 0XA4;
	SendCmdToMachine();
}

static void AnalysisCmd(void)
{
	if (RECEIVED_CMD[2] != 0x00 && RECEIVED_CMD[2] != 0XFF)  /*	C�ֽڲ�Ϊ00��FF��Ϊ��Э�飬˫��ˢ��	*/
	{
		/*	������߿�������Ϊ׼���򲻱ؽ���A�ֽ�	*/
		if (RECEIVED_CMD[0] != 0xFF)
		{
			mStatus.IndoorTemperature = RECEIVED_CMD[0] & POS_D06;
		}else
		{
			mStatus.IndoorTemperature = 0xFF;
		}
//		
//		if (TEMPRATURE_MAIN == OFF)  //���ڻ�����Ϊ׼
//		{
//			if (RECEIVED_CMD[0] != 0xFF)
//			{
//				mStatus.IndoorTemperature = RECEIVED_CMD[0] & POS_D06;
//			}else
//			{
//				mStatus.IndoorTemperature = 0xFF;
//			}
//			// ���ڻ����������¶ȱ�����ϲ�
//		}
		
		/*	����B�ֽ�	*/
		mStatus.IsHeating     =    (RECEIVED_CMD[1] & POS_D7) >> 7;
		mStatus.InletGridFlag =    (RECEIVED_CMD[1] & POS_D6) >> 6;
		mStatus.MachineType   =    (RECEIVED_CMD[1] & POS_D5) >> 5;
		mStatus.ErrorInfo     =     RECEIVED_CMD[1] & POS_D04;
		
		/*	����C�ֽ�	*/
		mStatus.OnOffFlag     =    (RECEIVED_CMD[2] & POS_D7) >> 7;
		mStatus.WindSwingFlag =    (RECEIVED_CMD[2] & POS_D6) >> 6;
		mStatus.WaterFlag     =    (RECEIVED_CMD[2] & POS_D4) >> 4;
		mStatus.HeaterFlag    =    (RECEIVED_CMD[2] & POS_D3) >> 3;
		mStatus.Mode          =     RECEIVED_CMD[2] & POS_D02;
		
		/*	����D�ֽ�	*/
		mStatus.WindSpeed     =    (RECEIVED_CMD[3] & POS_D67) >> 6;
		mStatus.SetTemperature=    (RECEIVED_CMD[3] & POS_D25) >> 2;
		mStatus.RemoteControl =    (RECEIVED_CMD[3] & POS_D1 ) >> 1;
		mStatus.RemoteLock    =     RECEIVED_CMD[3] & POS_D0;
		
		/*	
	              ������״̬������դ���գ��Լ����ͱ�����ϼ�
	        */
		
		if (mStatus.RemoteControl)
		{
			/*	ˢ����ʾ������Ҫˢ�µ���Ϣ�������߿��������޸���һ�α���ķ����ֽ�����	*/
			/*	�޸ķ���ֱ������	*/
			cmd[0] &= ~POS_D67;
			cmd[0] &= ~POS_D03;
			cmd[0] |= (mStatus.OnOffFlag     << 7 );
			cmd[0] |= (mStatus.WindSwingFlag << 6 );
			cmd[0] |= (mStatus.HeaterFlag    << 3 );
			cmd[0] |=  mStatus.Mode;
			
			cmd[1] &= ~POS_D05;
			cmd[1] |= (mStatus.WindSpeed << 4);
			cmd[1] |= mStatus.SetTemperature;
		}
		
		if (mStatus.RemoteLock)
		{
			/*	�߿����������߿������ܲ���		*/	
		}
		
		if (mStatus.ErrorInfo)
		{
			/*	��ʾ������Ϣ,������ֶ�Ƶ���Ƶ��	*/
			switch (mStatus.ErrorInfo)
			{
				case 0:break;
				case 1:break;
				case 2:break;
				case 3:break;
				case 4:break;
				case 5:break;
				case 6:break;
				case 7:break;
				case 8:break;
				case 9:break;
				case 10:break;
				case 11:break;
				case 12:break;
				case 13:break;
				case 14:break;
				case 15:break;
				case 16:break;
				case 17:break;
				case 18:break;
				case 19:break;
				case 20:break;
				case 21:break;
				case 22:break;
				case 23:break;
				case 24:break;
				case 25:break;
				case 26:break;
				case 27:break;
				default:break;
			}
		}
	}
}


void TaskReceive(void *p_arg)
{
	ReadCmdFromMachine();
	vTaskDelete(NULL);
}

/******************************* ***********************************************************
 *	 ������ ��EXTI1_IRQHandler()
 *	 ����   ��void
 *   ����ֵ ��void
 *   ����   ���ⲿһ�����жϳ������
 ******************************************************************************************/
void EXTI1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line1);
	xTaskCreate( TaskReceive,  ( signed char * ) "CommReceive" , configMINIMAL_STACK_SIZE * 5, ( void * ) NULL,3, NULL );
}

void SendCallback( xTimerHandle pxTimer)
{
	SendCmdToMachine();
}

void MainCallback( xTimerHandle pxTimer)
{
	//DC_USART_PutChar('E');
	//�Զ��ػ�����ʾ������Ϣ�����ڻ����͹ػ�����
}


