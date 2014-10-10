#include "flash.h"
#include "stdlib.h"
uint8_t FlashAdDisable = 0;
uint8_t InterruptOccurWhenFlashOperatorFlag = 0;//interrupt occurd when flash is operated flag
FLASH_8BIT_DATA_Union FlashIoOutputValue_Union, FlashIoInputValue_Union;
uint8_t FlashIoStatus = FLASH_IO_STATUS_NULL;//Flash io tris value,input\output\null
uint8_t FlashCeStatus = FLASH_CE_NULL;//FLASH ce status enable/disable/null
Status_union FlashWorkFlag_union;
uint8_t InvalidBlockTable[FLASH_BLOCK_MAX_NUM];
uint8_t user_data[26];

uint16_t BlockPic[2000];
uint16_t BlockPicSize = 0;

uint16_t BlockBadList[10];
uint16_t BlockBadSize = 0;
uint16_t BadListPos=0;
uint16_t BadListNext=1;
/*--bad block */
typedef struct block_bad
{
	uint8_t block_bad_num;
	uint8_t block_chg_num;
	struct block_bad *next;
}block_bad_list;

/*--temp init--*/
typedef struct temp_list
{
	uint16_t block;
	uint16_t page;
	uint8_t temp;
	struct temp_list *next;
}temp_summer_list,temp_spring_list,temp_winter_list;

uint16_t temp_summer_length,temp_spring_length,temp_winter_length;
temp_summer_list *temp_summer_head=NULL,*temp_summer_p=NULL;
temp_spring_list *temp_spring_head=NULL,*temp_spring_p=NULL;
temp_winter_list *temp_winter_head=NULL,*temp_winter_p=NULL;
extern struct air_condition air_condition_record;
/************************************************************************************
 * ������Flash_Init()
 * ��������ʼ��flash������������߼��飬��������
 * ���룺��
 * �������
 * ���أ���
************************************************************************************/
void Flash_Init(void)
{	
	uint16_t i,j,ret;
	uint8_t listLength[1],readbadblock[1],readchgblock[1],readtemp[1];
	if(FlashInitFlag)
		return;
	FlashInitFlag = 1;
	temp_summer_head=(temp_summer_list*)malloc(sizeof(temp_summer_list));
	temp_spring_head=(temp_spring_list*)malloc(sizeof(temp_spring_list));
	temp_winter_head=(temp_winter_list*)malloc(sizeof(temp_winter_list));
	temp_summer_p=temp_summer_head;
	temp_spring_p=temp_spring_head;
	temp_winter_p=temp_winter_head;
	
	Flash_Multi_Gpio_Init();
	FLASH_WE_OUTPUT_1();
	FLASH_RE_OUTPUT_1();
	FLASH_ALE_OUTPUT_0();
	FLASH_CLE_OUTPUT_0();
	Flash_Ce_Disable();
	
	if(FLASH_OPERATE_OK != Flash_Read_ID())
	{
		while(1);//fail
	}
	
	for(i=0;i<10;i++)
	{
		if(Flash_Check_Bad_Block(i)!=FLASH_BAD_BLOCK)
		{
			BlockBadList[BlockBadSize++]=i;
		}
	}
	
	//BlockPic init
	for(i=2100;i<4096;i++)
	{
		if(Flash_Check_Bad_Block(i)!=FLASH_BAD_BLOCK)
		{
			BlockPic[BlockPicSize++]=i;
		}
	}
	/*-read temp and init the temp list-*/
	//summer list
	for(i=300;i<900;i++)
	{
		if(Flash_Check_Bad_Block(i)!=FLASH_BAD_BLOCK)
		{
			for(j=0;j<64;j++)
			{
				Flash_Read_Data(i, j, 7, 1, readtemp);
				if(readtemp[0]!=0xff)
				{
					temp_summer_p->block=i;
					temp_summer_p->page=j;
					temp_summer_p->temp=readtemp[0];
					temp_summer_length++;
					temp_summer_p=(temp_summer_list*)malloc(sizeof(temp_summer_list));
					temp_summer_p=temp_summer_p->next;
				}
				else
				{
					j=64,i=900;
				}
			}
		}
		else
		{
			temp_summer_length+=64;
		}
	}
	//spring and autumn list
	for(i=900;i<1500;i++)
	{
		if(Flash_Check_Bad_Block(i)!=FLASH_BAD_BLOCK)
		{
			for(j=0;j<64;j++)
			{
				Flash_Read_Data(i, j, 7, 1, readtemp);
				if(readtemp[0]!=0xff)
				{
					temp_spring_p->block=i;
					temp_spring_p->page=j;
					temp_spring_p->temp=readtemp[0];
					temp_spring_length++;
					temp_spring_p=(temp_spring_list*)malloc(sizeof(temp_spring_list));
					temp_spring_p=temp_spring_p->next;
				}
				else
				{
					j=64,i=1500;
				}
			}
		}
		else
		{
			temp_summer_length+=64;
		}
	}
	//winter list
	for(i=1500;i<2100;i++)
	{
		if(Flash_Check_Bad_Block(i)!=FLASH_BAD_BLOCK)
		{
			for(j=0;j<64;j++)
			{
				Flash_Read_Data(i, j, 7, 1, readtemp);
				if(readtemp[0]!=0xff)
				{
					temp_winter_p->block=i;
					temp_winter_p->page=j;
					temp_winter_p->temp=readtemp[0];
					temp_winter_length++;
					temp_winter_p=(temp_winter_list*)malloc(sizeof(temp_winter_list));
					temp_winter_p=temp_winter_p->next;
				}
				else
				{
					j=64,i=2100;
				}
			}
		}
		else
		{
			temp_summer_length+=64;
		}
	}
}
/************************************************************************************
 * ������Flash_Set_Io_Input()
 * ��������ģ�Ⲣ������������Ϊ���롣�����ã����ڲ�û��ʹ�ã���IO������Ϊ��©
 * ���룺��
 * �������
 * ���أ���
************************************************************************************/
void Flash_Set_Io_Input(void)
{
	//GPIO_InitTypeDef GPIO_InitStructure;
	if(FLASH_IO_STATUS_INPUT == FlashIoStatus)//have setted,just return
		return;
	FlashIoStatus = FLASH_IO_STATUS_INPUT;
	FLASH_IO0_GPIO->MODER &= ~(0x03 << (FLASH_IO0_PINS*2));
	FLASH_IO1_GPIO->MODER &= ~(0x03 << (FLASH_IO1_PINS*2));
	FLASH_IO2_GPIO->MODER &= ~(0x03 << (FLASH_IO2_PINS*2));
	FLASH_IO3_GPIO->MODER &= ~(0x03 << (FLASH_IO3_PINS*2));
	FLASH_IO4_GPIO->MODER &= ~(0x03 << (FLASH_IO4_PINS*2));
	FLASH_IO5_GPIO->MODER &= ~(0x03 << (FLASH_IO5_PINS*2));
	FLASH_IO6_GPIO->MODER &= ~(0x03 << (FLASH_IO6_PINS*2));
	FLASH_IO7_GPIO->MODER &= ~(0x03 << (FLASH_IO7_PINS*2));
}

/************************************************************************************
 * ������Flash_Set_Io_Output()
 * ��������ģ�Ⲣ������������Ϊ����������ã����ڲ�û��ʹ�ã���IO������Ϊ��©
 * ���룺��
 * �������
 * ���أ���
************************************************************************************/
void Flash_Set_Io_Output(void)
{
	//GPIO_InitTypeDef GPIO_InitStructure;
	if(FLASH_IO_STATUS_OUTPUT == FlashIoStatus)//have setted,just return
		return;
	FlashIoStatus = FLASH_IO_STATUS_OUTPUT;
	FLASH_IO0_GPIO->MODER |= GPIO_Mode_OUT << (FLASH_IO0_PINS*2);
	FLASH_IO1_GPIO->MODER |= GPIO_Mode_OUT << (FLASH_IO1_PINS*2);
	FLASH_IO2_GPIO->MODER |= GPIO_Mode_OUT << (FLASH_IO2_PINS*2);
	FLASH_IO3_GPIO->MODER |= GPIO_Mode_OUT << (FLASH_IO3_PINS*2);
	FLASH_IO4_GPIO->MODER |= GPIO_Mode_OUT << (FLASH_IO4_PINS*2);
	FLASH_IO5_GPIO->MODER |= GPIO_Mode_OUT << (FLASH_IO5_PINS*2);
	FLASH_IO6_GPIO->MODER |= GPIO_Mode_OUT << (FLASH_IO6_PINS*2);
	FLASH_IO7_GPIO->MODER |= GPIO_Mode_OUT << (FLASH_IO7_PINS*2);
}

/************************************************************************************
 * ������Flash_Multi_Gpio_Init()
 * ��������ʼ������nand flash��������ģʽ�����������ߺͲ��������ߣ���ָ�����в�������״̬
 * ���룺��
 * �������
 * ���أ���
************************************************************************************/
void Flash_Multi_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//���ƽų�ʼ��
	RCC_AHB1PeriphClockCmd(FLASH_WE_CLK | FLASH_CLE_CLK | 
												 FLASH_ALE_CLK | FLASH_RE_CLK | 
												 FLASH_RB_CLK | FLASH_CE_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
	GPIO_InitStructure.GPIO_Pin = FLASH_RE_PIN;
	GPIO_Init(FLASH_RE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_ALE_PIN;
	GPIO_Init(FLASH_ALE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_CLE_PIN;
	GPIO_Init(FLASH_CLE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_CE_PIN;
	GPIO_Init(FLASH_CE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_WE_PIN;
	GPIO_Init(FLASH_WE_GPIO, &GPIO_InitStructure);
	
	//ready/busy pin
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 
  GPIO_InitStructure.GPIO_Pin = FLASH_RB_PIN;  
  GPIO_Init(FLASH_RB_GPIO, &GPIO_InitStructure);
	
	//GPIO_SetBits(FLASH_RB_GPIO, FLASH_RB_PIN);
	
	//io��Ĭ�ϳ�ʼ��
	RCC_AHB1PeriphClockCmd(FLASH_IO0_CLK | FLASH_IO1_CLK | FLASH_IO2_CLK |
												 FLASH_IO3_CLK | FLASH_IO4_CLK | FLASH_IO5_CLK |
												 FLASH_IO6_CLK | FLASH_IO7_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	
	GPIO_InitStructure.GPIO_Pin = FLASH_IO0_PIN;
	GPIO_Init(FLASH_IO0_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_IO1_PIN;
	GPIO_Init(FLASH_IO1_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_IO2_PIN;
	GPIO_Init(FLASH_IO2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_IO3_PIN;
	GPIO_Init(FLASH_IO3_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_IO4_PIN;
	GPIO_Init(FLASH_IO4_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_IO5_PIN;
	GPIO_Init(FLASH_IO5_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_IO6_PIN;
	GPIO_Init(FLASH_IO6_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FLASH_IO7_PIN;
	GPIO_Init(FLASH_IO7_GPIO, &GPIO_InitStructure);
	
	FLASH_IO0_OUTPUT_1();
	FLASH_IO1_OUTPUT_1();
	FLASH_IO2_OUTPUT_1();
	FLASH_IO3_OUTPUT_1();
	FLASH_IO4_OUTPUT_1();
	FLASH_IO5_OUTPUT_1();
	FLASH_IO6_OUTPUT_1();
	FLASH_IO7_OUTPUT_1();
	FLASH_RB_OUTPUT_1();
}

/************************************************************************************
 * ������Flash_Ce_Enable()
 * ������ʹ��CE���ţ�ʹflash���빤��״̬
 * ���룺��
 * �������
 * ���أ���
************************************************************************************/
void Flash_Ce_Enable(void)
{
	if(FLASH_CE_ENABLE == FlashCeStatus)
		return;
	FlashCeStatus = FLASH_CE_ENABLE;
	FLASH_CE_OUTPUT_0();
}

/************************************************************************************
 * ������Flash_Ce_Disable()
 * ������ʧ��CE���ţ�ʹflash����Ӧ���ݼ���ַ����
 * ���룺��
 * �������
 * ���أ���
************************************************************************************/
void Flash_Ce_Disable(void)
{
	if(FLASH_CE_DISABLE == FlashCeStatus)
		return;
	FlashCeStatus = FLASH_CE_DISABLE;
	FLASH_CE_OUTPUT_1();
}


/************************************************************************************
 * ������Flash_Write_8Bit_Data()
 * ������mcuд8λ���ݼ�һ���ֽڵ�flash�����ݿ����������ַ������
 * ���룺DAta--�����������ַ�����ݵ�һ����
 * �������
 * ���أ���
************************************************************************************/
void Flash_Write_8Bit_Data(uint8_t DAta)
{
	uint8_t i;
	
	FLASH_IO0_OUTPUT_1();
	FLASH_IO1_OUTPUT_1();
	FLASH_IO2_OUTPUT_1();
	FLASH_IO3_OUTPUT_1();
	FLASH_IO4_OUTPUT_1();
	FLASH_IO5_OUTPUT_1();
	FLASH_IO6_OUTPUT_1();
	FLASH_IO7_OUTPUT_1();
	FLASH_RB_OUTPUT_1();
	
	Flash_Ce_Enable();
	//Flash_Set_Io_Output();	
	FlashIoOutputValue_Union.AllBit = DAta;
	FLASH_WE_OUTPUT_0();
	for(i = 0; i < 1; i++);
	
	FlashIoOutputValue_Union.OneBit.IO1?(FLASH_IO1_OUTPUT_1()):(FLASH_IO1_OUTPUT_0());
	FlashIoOutputValue_Union.OneBit.IO0?(FLASH_IO0_OUTPUT_1()):(FLASH_IO0_OUTPUT_0());
	FlashIoOutputValue_Union.OneBit.IO2?(FLASH_IO2_OUTPUT_1()):(FLASH_IO2_OUTPUT_0());
	FlashIoOutputValue_Union.OneBit.IO3?(FLASH_IO3_OUTPUT_1()):(FLASH_IO3_OUTPUT_0());
	FlashIoOutputValue_Union.OneBit.IO4?(FLASH_IO4_OUTPUT_1()):(FLASH_IO4_OUTPUT_0());
	FlashIoOutputValue_Union.OneBit.IO5?(FLASH_IO5_OUTPUT_1()):(FLASH_IO5_OUTPUT_0());
	FlashIoOutputValue_Union.OneBit.IO7?(FLASH_IO7_OUTPUT_1()):(FLASH_IO7_OUTPUT_0());
	FlashIoOutputValue_Union.OneBit.IO6?(FLASH_IO6_OUTPUT_1()):(FLASH_IO6_OUTPUT_0());
	
	for(i = 0; i < 1; i++);
	FLASH_WE_OUTPUT_1();
}

/************************************************************************************
 * ������Flash_Read_8Bit_Data()
 * ������mcu��flash��ȡһ���ֽ�����
 * ���룺��
 * �������
 * ���أ�data--�������ֽ�����
************************************************************************************/
uint8_t Flash_Read_8Bit_Data(void)
{
	uint8_t data, i;
	
	FLASH_IO0_OUTPUT_1();
	FLASH_IO1_OUTPUT_1();
	FLASH_IO2_OUTPUT_1();
	FLASH_IO3_OUTPUT_1();
	FLASH_IO4_OUTPUT_1();
	FLASH_IO5_OUTPUT_1();
	FLASH_IO6_OUTPUT_1();
	FLASH_IO7_OUTPUT_1();
	
	Flash_Ce_Enable();
	//Flash_Set_Io_Input();
	FLASH_RE_OUTPUT_0();//enable read function
	//for(i = 0; i < 1; i++);
	
	FlashIoInputValue_Union.OneBit.IO1 = FLASH_IO1_INPUT_VALUE();
	FlashIoInputValue_Union.OneBit.IO0 = FLASH_IO0_INPUT_VALUE();
	FlashIoInputValue_Union.OneBit.IO2 = FLASH_IO2_INPUT_VALUE();
	FlashIoInputValue_Union.OneBit.IO3 = FLASH_IO3_INPUT_VALUE();
	FlashIoInputValue_Union.OneBit.IO4 = FLASH_IO4_INPUT_VALUE();
	FlashIoInputValue_Union.OneBit.IO5 = FLASH_IO5_INPUT_VALUE();
	FlashIoInputValue_Union.OneBit.IO7 = FLASH_IO7_INPUT_VALUE();
	FlashIoInputValue_Union.OneBit.IO6 = FLASH_IO6_INPUT_VALUE();
	
	for(i = 0; i < 1; i++);
	FLASH_RE_OUTPUT_1();//disable read function
	
	
	data = FlashIoInputValue_Union.AllBit;
	return data;
}

/************************************************************************************
 * ������Flash_Read_Io_Status()
 * ������mcu��ȡflash��IO��״̬�������ж�ĳЩ�����Ƿ���ȷ���
 * ���룺BItNUm--����ȡλ
 * �������
 * ���أ�0--����λΪ��
				 1--����λΪ��
				 FLASH_OPERATE_BREAK--��ȡʧ�ܣ���;�˳�
************************************************************************************/
uint8_t Flash_Read_Io_Status(uint8_t BItNUm)
{
	uint8_t flash_data = 0x01;
	Flash_Ce_Enable();
	if(BItNUm > 7)
		return FLASH_OPERATE_BREAK;
	flash_data = flash_data << BItNUm;
	if((Flash_Read_8Bit_Data() & flash_data) == 0)
		return 0;
	return 1;
}

/************************************************************************************
 * ������Flash_Write_Command()
 * ������дһ���ֽ����flash
 * ���룺COmmand--��д����
 * �������
 * ���أ���
************************************************************************************/
void Flash_Write_Command(uint8_t COmmand)
{
	InterruptOccurWhenFlashOperatorFlag = 0;//flag=0,insure interrupt happen after this time
	Flash_Ce_Enable();
	FLASH_CLE_OUTPUT_1();
	Flash_Write_8Bit_Data(COmmand);
	FLASH_CLE_OUTPUT_0();
}

/************************************************************************************
 * ������Flash_Write_Row_Column_Address()
 * ������д���е�ַ��flash
 * ���룺BLockNUm--block��� 0-4095
				 PAgeNUm--page���   0-63
				 BYteNUm--byte���   0-2111���޸ĸò������Զ�ҳ�ڽ��в�������sector��
 * �������
 * ���أ���
************************************************************************************/
void Flash_Write_Row_Column_Address(uint16_t BLockNUm, uint8_t PAgeNUm, uint16_t BYteNUm) 
{
	uint8_t i, address[5];	
	address[0] = (uint8_t)BYteNUm;
	address[1] = (uint8_t)((BYteNUm>>8) & 0x0f);
	address[2] = (BLockNUm << 6) + PAgeNUm;
	address[3] = BLockNUm >> 2;
	address[4] = (BLockNUm >> 10) & 0x03;	
	FLASH_ALE_OUTPUT_1();
	for(i=0; i<5; i++)	
		Flash_Write_8Bit_Data(address[i]);
	for(i = 0; i < 5; i++);
	FLASH_ALE_OUTPUT_0();
}

/************************************************************************************
 * ������Flash_Write_Row_Address()
 * ������д�е�ַ��flash
 * ���룺BLockNUm--block��� 0-4095
				 PAgeNUm--page���   0-63
 * �������
 * ���أ���
************************************************************************************/
void Flash_Write_Row_Address(uint16_t BLockNUm, uint8_t PAgeNUm)
{
	uint8_t i, address[3];	
	address[0] = (BLockNUm << 6) + PAgeNUm;
	address[1] = BLockNUm >> 2;
	address[2] = (BLockNUm >> 10) & 0x03;	
	FLASH_ALE_OUTPUT_1();
	for(i=0; i<3; i++)	
		Flash_Write_8Bit_Data(address[i]);
	FLASH_ALE_OUTPUT_0();
}

/************************************************************************************
 * ������Flash_Write_Column_Address()
 * ������д�е�ַ��flash
 * ���룺BYteNUm--byte���   0-2111
 * �������
 * ���أ���
************************************************************************************/
void Flash_Write_Column_Address(uint16_t BYteNUm) 
{
	uint8_t i, address[2];
	address[0] = BYteNUm;
	address[1] = (BYteNUm >> 8) & 0x0f;
	FLASH_ALE_OUTPUT_1();
	for(i=0; i<2; i++)	
		Flash_Write_8Bit_Data(address[i]);	
	FLASH_ALE_OUTPUT_0();	
}

/************************************************************************************
 * ������Flash_Write_1Byte_Address()
 * ������дһ�ֽڵ�ַ��flash
 * ���룺ADdress--��д��ַ
 * �������
 * ���أ���
************************************************************************************/
void Flash_Write_1Byte_Address(uint8_t ADdress) 
{
	uint8_t i;
	FLASH_ALE_OUTPUT_1();
	Flash_Write_8Bit_Data(ADdress);
	for(i = 0; i < 5; i++);
	FLASH_ALE_OUTPUT_0();
}

/************************************************************************************
 * ������Flash_RB_Value()
 * ������RB�ŵı仯Ϊ��-��-�ߣ���RB��Ӧ���б仯�ĵط���⵽�ù�����ɣ����ܽ��к���������
				 �������
 * ���룺��
 * �������
 * ���أ�FLASH_OPERATE_OK--���⵽�����������
				 FLASH_OPERATE_ERROR--δ��⵽Ӧ�й��̣�ʧ��
************************************************************************************/
uint8_t Flash_RB_Value(void)
{
	//uint8_t i;
	uint8_t data1, data2;
	uint16_t rbdelaytime = FLASH_RB_DELAY_TIME;
	FLASH_RB_OUTPUT_1();
	
	data1 = FLASH_RB_INPUT_VALUE();
	while((--rbdelaytime) && (data1 == 0x01))
	{
		us_Delay(1);
		data1 = FLASH_RB_INPUT_VALUE();
	}
	FLASH_RB_OUTPUT_1();

	if((rbdelaytime == 0) && (0 == InterruptOccurWhenFlashOperatorFlag))
		return FLASH_OPERATE_ERROR;
	rbdelaytime = FLASH_RB_DELAY_TIME;
	data2 = FLASH_RB_INPUT_VALUE();
	while((--rbdelaytime)&&(data2 == 0x00))
	{
		us_Delay(1);
		data2 = FLASH_RB_INPUT_VALUE();
	}
	if((rbdelaytime == 0) && (0 == InterruptOccurWhenFlashOperatorFlag))
		return FLASH_OPERATE_ERROR;		
	if(1 == InterruptOccurWhenFlashOperatorFlag)//some interrupt happen,must wait some time 
		ms_Delay(2);
	//for(i = 0; i < 1; i++);
	return FLASH_OPERATE_OK;
}

/************************************************************************************
 * ������Flash_Read_Data()
 * ��������flashָ��λ�ö���ָ�����������ݣ��Բ���BYteNUm��NUm���к������ã����Զ�ĳҳ�е�
				 sector���ж�����
 * ���룺BLockNUm--block num 0-4095
				 PAgeNUm--page num   0-63
				 BYteNUm--byte num   0-2111
				 NUm--ָ�����������ݸ���
 * �����pREadDAta--�������ݴ��λ���׵�ַ
 * ���أ�FLASH_OPERATE_OK--��ȡ�ɹ�
				 FLASH_OPERATE_ERROR--��ȡʧ��
				 FLASH_OPERATE_BREAK--���飬���ܶ�ȡ
************************************************************************************/
uint8_t Flash_Read_Data(uint16_t BLockNUm, uint8_t PAgeNUm, uint16_t BYteNUm, uint16_t NUm, uint8_t *pREadDAta)
{
	uint8_t *pdata;	
	uint8_t trytimes = FLASH_OPERATE_TRY_TIME;
	while(trytimes--)
	{
		Flash_Write_Command(FLASH_READ_1ST_COMMAND);
		Flash_Write_Row_Column_Address(BLockNUm, PAgeNUm, BYteNUm);
		Flash_Write_Command(FLASH_READ_2ND_COMMAND);
		
		if(Flash_RB_Value() != FLASH_OPERATE_OK)
		{
			Flash_Ce_Disable();
			us_Delay(10);
			continue;
		}
		pdata = pREadDAta;
		while(pdata < pREadDAta + NUm)
			*pdata++ = Flash_Read_8Bit_Data();
		Flash_Ce_Disable();
		return FLASH_OPERATE_OK;
	}
	return FLASH_OPERATE_ERROR;
}

/************************************************************************************
 * ������Flash_Write_Data()
 * ��������flashָ��λ��д��ָ�����������ݣ��Բ���BYteNUm��NUm���к������ã����Զ�ĳҳ�е�
				 sector����д����
 * ���룺BLockNUm--block num 0-4095
				 PAgeNUm--page num   0-63
				 BYteNUm--byte num   0-2111
				 NUm--ָ����д�����ݸ���
 * �����pREadDAta--��д�������׵�ַ
 * ���أ�FLASH_OPERATE_OK--д��ɹ�
				 FLASH_OPERATE_ERROR--д��ʧ��
				 FLASH_OPERATE_BREAK--����δ���
************************************************************************************/
uint8_t Flash_Write_Data(uint16_t BLockNUm, uint8_t PAgeNUm, uint16_t BYteNUm, uint16_t NUm, uint8_t *pWRiteDAta)
{
	//uint8_t i;
	uint8_t retry;
	uint8_t *pdata = pWRiteDAta;
	uint8_t trytimes = FLASH_OPERATE_TRY_TIME;	
	uint8_t readoutdata[5000];
	uint8_t operatorflag = 0;

	while(trytimes--)
	{
		Flash_Write_Command(FLASH_PAGE_PROGRAM_1ST_COMMAND);//write program command 1 
		Flash_Write_Row_Column_Address(BLockNUm, PAgeNUm, BYteNUm);//write address
		pdata = pWRiteDAta;
		//for(i = 0; i < 50; i++);
		while(pdata < pWRiteDAta + NUm)													//write data
			Flash_Write_8Bit_Data(*pdata++);			
		Flash_Write_Command(FLASH_PAGE_PROGRAM_2ND_COMMAND);//write program command 2
		if(Flash_RB_Value() != FLASH_OPERATE_OK)							//wait program operate finished
		{
			Flash_Ce_Disable();
			us_Delay(10);
			continue;
		}
		Flash_Write_Command(FLASH_READ_STATUS_COMMAND);			//write read status command
		us_Delay(200);	
		if(0 != Flash_Read_Io_Status(0))										  //read the status of io0 
		{
			Flash_Ce_Disable();
			us_Delay(10);
			continue;
		}
		else
		{
			operatorflag = 1;
			break;
		}
	}
	if(1 != operatorflag)
		return FLASH_OPERATE_BREAK;
	ms_Delay(100);
	
	for(retry = 0; retry < 5; retry++)
	{
		if(FLASH_OPERATE_OK == Flash_Read_Data(BLockNUm, PAgeNUm, BYteNUm, NUm, readoutdata))//read out the data
		{
			if(1 != Test_Two_Mermory_Same_Or_Not(pWRiteDAta, readoutdata, NUm))
			{
				Flash_Ce_Disable();
				us_Delay(10);
				continue;
			}
			else
				return FLASH_OPERATE_OK;
		}
	}
	if(retry == 5)
	{
		Flash_Sign_Bad_Block(BLockNUm);//failed to write,it is a bad block,sign it
		return FLASH_OPERATE_ERROR;
	}
	return FLASH_OPERATE_BREAK;
}

/************************************************************************************
 * ������Flash_Reset()
 * ��������λflash
 * ���룺��
 * �������
 * ���أ�FLASH_OPERATE_OK--�ɹ�
				 FLASH_OPERATE_ERROR--ʧ��
************************************************************************************/
uint8_t Flash_Reset(void)
{
	uint8_t trytimes=FLASH_OPERATE_TRY_TIME;
	while(trytimes--)
	{
		Flash_Write_Command(FLASH_RESET_COMMAND);
		if(Flash_RB_Value()!=FLASH_OPERATE_OK)
		{
			Flash_Ce_Disable();
			ms_Delay(10);
			continue;
		}
		return FLASH_OPERATE_OK;	
	}
	return FLASH_OPERATE_ERROR;	
}

/************************************************************************************
 * ������Flash_Sign_Bad_Block()
 * ��������flash�Ļ�����б�ǣ�������λ��Ϊÿ���1st��2ndҳ�ĵ�2048���ֽڣ����ֽ�Ϊ
				 0xFFʱ�����ÿ��������ã�Ϊ0x00ʱ�����ÿ�Ϊ���飬�ú�����д������0x00�Ա�ǻ���
 * ���룺BLockNUm--����ǿ�
 * �������
 * ���أ�FLASH_OPERATE_OK--�ɹ�
				 FLASH_OPERATE_ERROR--ʧ��
************************************************************************************/
uint8_t Flash_Sign_Bad_Block(uint16_t BLockNUm)
{	
	uint8_t trytimes = FLASH_OPERATE_TRY_TIME;	
	uint8_t operatorflag = 0;
	uint8_t pagenum = 0;		
	for(pagenum = 0; pagenum < 2; pagenum++)
	{
		trytimes = FLASH_OPERATE_TRY_TIME;
		while(trytimes--)
		{
			Flash_Write_Command(FLASH_PAGE_PROGRAM_1ST_COMMAND);//write command 0x80 
			Flash_Write_Row_Column_Address(BLockNUm, pagenum, FLASH_BAD_BLOCK_SIGN_ADDRESS);//write address	
			Flash_Write_8Bit_Data(FLASH_BAD_BLOCK_SIGN);	//write bad block sign 0x00
			Flash_Write_Command(FLASH_PAGE_PROGRAM_2ND_COMMAND);//write command 0X10	
			Flash_RB_Value();//wait write finished				
			Flash_Write_Command(FLASH_READ_1ST_COMMAND);
			Flash_Write_Row_Column_Address(BLockNUm, pagenum, FLASH_BAD_BLOCK_SIGN_ADDRESS);
			Flash_Write_Command(FLASH_READ_2ND_COMMAND);
			Flash_RB_Value();//wait data complete		
		  if(FLASH_GOOD_BLOCK_SIGN == Flash_Read_8Bit_Data())//data is wrong 
			{
				Flash_Ce_Disable();
				ms_Delay(10);
				continue;			
			}
			else//data is right
			{
				operatorflag++;
				break;
			}
		}
	}
	if(2 == operatorflag)
		return FLASH_OPERATE_OK;
	else
		return FLASH_OPERATE_ERROR;	
}

/************************************************************************************
 * ������Flash_Check_Bad_Block()
 * ���������ĳ�����Ƿ�Ϊ���飬��ȡ�ÿ��1st��2ndҳ�ĵ�2048�ֽ��Ƿ�Ϊ0x00������Ϊ����
 * ���룺BLockNUm--�����
 * �������
 * ���أ�FLASH_OPERATE_OK--�ɹ�
				 FLASH_OPERATE_ERROR--ʧ��
************************************************************************************/
uint8_t Flash_Check_Bad_Block(uint16_t BLockNUm)
{
	uint8_t trytimes = FLASH_OPERATE_TRY_TIME;	
	uint8_t pagenum = 0;
	uint8_t data;
	
	for(pagenum = 0; pagenum < 2; pagenum++)
	{
		trytimes = FLASH_OPERATE_TRY_TIME;
		while(trytimes--)
		{
			Flash_Write_Command(FLASH_READ_1ST_COMMAND);
			Flash_Write_Row_Column_Address(BLockNUm, pagenum, FLASH_BAD_BLOCK_SIGN_ADDRESS);
			Flash_Write_Command(FLASH_READ_2ND_COMMAND);
			if(Flash_RB_Value() != FLASH_OPERATE_OK)	
			{
				Flash_Ce_Disable();
				ms_Delay(10);
				continue;//failed	
			}
			data = Flash_Read_8Bit_Data();
			if(data != FLASH_GOOD_BLOCK_SIGN)
				return FLASH_BAD_BLOCK;	
		}
	}
	return FLASH_GOOD_BLOCK;
}

/************************************************************************************
 * ������Flash_Erase_Block()
 * ������������������С��λΪ�飨block��������д�����ݵ�flashĳ����ַ���ٴ�д�������ݣ�������
				 ���в���������Ҳ����˵�õ�ַ���ڵĿ齫���������
 * ���룺BLockNUm--����������
 * �������
 * ���أ�FLASH_OPERATE_OK--�ɹ�
				 FLASH_OPERATE_ERROR--ʧ��
				 FLASH_OPERATE_BREAK--����
************************************************************************************/
uint8_t Flash_Erase_Block(uint16_t BLockNUm)
{
	uint8_t trytimes=FLASH_OPERATE_TRY_TIME;
	while(trytimes--)
	{
		if(Flash_Check_Bad_Block(BLockNUm) == FLASH_BAD_BLOCK)
			return FLASH_OPERATE_BREAK;//this is a bad clock,can't erase
		Flash_Write_Command(FLASH_BLOCK_ERASE_1ST_COMMAND);
		Flash_Write_Row_Address(BLockNUm, 0);	
		Flash_Write_Command(FLASH_BLOCK_ERASE_2ND_COMMAND);
		if(Flash_RB_Value() != FLASH_OPERATE_OK)
		{
			Flash_Ce_Disable();
			ms_Delay(10);
			continue;//failed	
		}		
		Flash_Write_Command(FLASH_READ_STATUS_COMMAND);
		us_Delay(200);
		if(Flash_Read_Io_Status(0) == 1)
		{
			Flash_Ce_Disable();
			us_Delay(10);
			continue;//failed	
		}
		else
			return FLASH_OPERATE_OK;
	}
	return FLASH_OPERATE_ERROR;
}

/************************************************************************************
 * ������Flash_Read_ID()
 * �������Ƿ��ܶ���ID������ID�Ƿ���ȷ�����ڼ��Ӳ�������Ƿ���ȷ��ע��defaultflashid��
				 ֵ�����ͺŸı�
 * ���룺��
 * �������
 * ���أ�FLASH_OPERATE_OK--�ɹ�
				 FLASH_OPERATE_ERROR--ʧ��
************************************************************************************/
uint8_t Flash_Read_ID(void)
{
	uint8_t defaultflashid[5] = {0xEC, 0xDC, 0x10, 0x95, 0x54};//������
	//uint8_t defaultflashid[5] = {0xEC, 0xF1, 0x00, 0x95, 0x41};
	uint8_t receiveflashid[5];
	uint8_t i = 0;
	uint8_t trytimes = FLASH_OPERATE_TRY_TIME;
	while(trytimes--)
	{
		Flash_Write_Command(FLASH_READ_ID_COMMAND);	//write read id command 
		//for(i=0; i<5; i++);
		Flash_Write_1Byte_Address(0x00);						//this address maybe anyone
		//for(i=0; i<5; i++);
		for(i=0; i<5; i++)
			receiveflashid[i] = Flash_Read_8Bit_Data();
		if(1 != Test_Two_Mermory_Same_Or_Not(defaultflashid, receiveflashid, 5))
		{
			Flash_Ce_Disable();
			us_Delay(10);
			continue;//failed	
		}
		return FLASH_OPERATE_OK;
	}
	return FLASH_OPERATE_ERROR;	
}

/************************************************************************************
 * ������Flash_Read_Random_Data()
 * ��������ͬһҳ��ָ�����β�ͬ�����ݽ��ж�ȡ���ֱ�ָ��ҳ�ڵ�ַ����ȡ����
 * ���룺BLockNUm--block num
				 PAgeNUm--page num
				 FIrstBYteNUm--first byte num
				 FIrstNUm--the data num that you read first time 
 		     SEcondBYteNUm--second byte num
 		     SEcondNUm--the data num that you read second time
 * �����pRAndomDAta--�������ݴ���׵�ַ
 * ���أ�FLASH_OPERATE_OK--�ɹ�
				 FLASH_OPERATE_ERROR--ʧ��
				 FLASH_OPERATE_BREAK--����
************************************************************************************/
uint8_t Flash_Read_Random_Data(uint16_t BLockNUm,uint8_t PAgeNUm,uint16_t FIrstBYteNUm,uint16_t FIrstNUm,uint16_t SEcondBYteNUm,uint16_t SEcondNUm,uint8_t *pRAndomDAta)
{
	uint8_t *pdata;
	if(FLASH_GOOD_BLOCK!=Flash_Check_Bad_Block(BLockNUm))//check it is bad or not
		return FLASH_OPERATE_BREAK;			
	if(Flash_Read_Data(BLockNUm,PAgeNUm,FIrstBYteNUm,FIrstNUm,pRAndomDAta)!=FLASH_OPERATE_OK)
		return FLASH_OPERATE_ERROR;	
	Flash_Write_Command(FLASH_RANDOM_DATA_OUTPUT_1ST_COMMAND);
	Flash_Write_Column_Address(SEcondBYteNUm);	
	Flash_Write_Command(FLASH_RANDOM_DATA_OUTPUT_2ND_COMMAND);
	pdata=pRAndomDAta+FIrstNUm;
	while(pdata<pRAndomDAta+FIrstNUm+SEcondNUm)
		*pdata++=Flash_Read_8Bit_Data();
	return FLASH_OPERATE_OK;		
}

/************************************************************************************
 * ������Flash_Write_Random_Data()
 * ��������ͬһҳ��ָ����������ַ�ֱ�д�벻ͬ�����ݣ��ֱ�ָ��ҳ�ڵ�ַ��д�������д������
 * ���룺BLockNUm--block num
				 PAgeNUm--page num
				 FIrstBYteNUm--first byte num
				 FIrstNUm--the data num that you read first time 
				 *pFIrstDAta--first write data
 		     SEcondBYteNUm--second byte num
 		     SEcondNUm--the data num that you read second time
				 *pSEcondDAta--second wriye data 
 * �������
 * ���أ�FLASH_OPERATE_OK--�ɹ�
				 FLASH_OPERATE_ERROR--ʧ��
				 FLASH_OPERATE_BREAK--����
************************************************************************************/
uint8_t Flash_Write_Random_Data(uint16_t BLockNUm, uint8_t PAgeNUm, uint16_t FIrstBYteNUm, uint16_t FIrstNUm, uint8_t *pFIrstDAta, uint16_t SEcondBYteNUm, uint8_t SEcondNUm, uint8_t *pSEcondDAta)
{
	uint8_t *pdata=pFIrstDAta;	
	if(FLASH_GOOD_BLOCK != Flash_Check_Bad_Block(BLockNUm))//check it is bad or not
		return FLASH_OPERATE_BREAK;	
	Flash_Write_Command(FLASH_PAGE_PROGRAM_1ST_COMMAND);//write command 0X80
	Flash_Write_Row_Column_Address(BLockNUm, PAgeNUm, FIrstBYteNUm);//write address
	pdata = pFIrstDAta;
	while(pdata < pFIrstDAta+FIrstNUm)													//write first data
		Flash_Write_8Bit_Data(*pdata++);	
	Flash_Write_Command(FLASH_RANDOM_DATA_INPUT_COMMAND);//write command 0X85
	Flash_Write_Column_Address(SEcondBYteNUm);
	pdata = pSEcondDAta; 
	while(pdata < pSEcondDAta + SEcondNUm)					//write second data
		Flash_Write_8Bit_Data(*pdata++);			
	Flash_Write_Command(FLASH_PAGE_PROGRAM_2ND_COMMAND);//write command 0X10	
	if(Flash_RB_Value()!=FLASH_OPERATE_OK)							//wait write finished 
		return FLASH_OPERATE_ERROR;
	Flash_Write_Command(FLASH_READ_STATUS_COMMAND);			//write read status command
	us_Delay(200);	
	if(Flash_Read_Io_Status(0)==0)										 //read IO0 status
		return FLASH_OPERATE_OK;
	else
		return FLASH_OPERATE_ERROR;	 
}

/************************************************************************************
 * ������Flash_Erase_The_Device()
 * ��������������flash
 * ���룺�� 
 * �������
 * ���أ���
************************************************************************************/
void Flash_Erase_The_Device(void)
{
	uint16_t blocknum=0;
	for(blocknum=0; blocknum < FLASH_BLOCK_MAX_NUM; blocknum++)
		Flash_Erase_Block(blocknum);
}
/************************************************************************************
**Fun:�Ƚ������ڴ��DAtaLEngth�����������Ƿ�һ��
**Intput:	pTArgetDAta  -- �׵�ַ1
					  pSOurceDAta-- �׵�ַ2
					  DAtaLEngth -- �Ƚ��ֽڸ���
**Outtput:	none
**Function outtput:	0: ��һ��
										1: һ��
************************************************************************************/
uint8_t Test_Two_Mermory_Same_Or_Not(uint8_t *pTArgetDAta, uint8_t *pSOurceDAta, uint16_t DAtaLEngth)
{
	uint8_t *pdata1=pTArgetDAta,*pdata2=pSOurceDAta;
	if(DAtaLEngth == 0)
		return 0;	
	while(pdata1 < (pTArgetDAta + DAtaLEngth))
	{
		if((*pdata1++)!=(*pdata2++))		
			return 0;		
	}
	return 1;
}
void List_Array(struct air_condition air_condition_user)
{
	user_data[0]=air_condition_user.id;
	user_data[1]=air_condition_user.month;
	user_data[2]=air_condition_user.week;
	user_data[3]=air_condition_user.day;
	user_data[4]=air_condition_user.hour;
	user_data[5]=air_condition_user.minute;
	
	user_data[6]=air_condition_user.out_temp;
	user_data[7]=air_condition_user.in_temp;
	user_data[8]=air_condition_user.air_temp;
	user_data[9]=air_condition_user.set_temp;
	
	user_data[10]=air_condition_user.out_mois;
	user_data[11]=air_condition_user.in_mois;
	user_data[12]=air_condition_user.air_windV;
	user_data[13]=air_condition_user.set_windV;
	
	user_data[14]=air_condition_user.air_windUD;
	user_data[15]=air_condition_user.set_windUD;
	user_data[16]=air_condition_user.air_windLR;
	user_data[17]=air_condition_user.set_windLR;
	
	user_data[18]=air_condition_user.air_mode;
	user_data[19]=air_condition_user.set_mode;
	user_data[20]=air_condition_user.air_scene;
	user_data[21]=air_condition_user.set_scene;
	
	user_data[22]=air_condition_user.air_status;
	user_data[23]=air_condition_user.set_status;
	user_data[24]=air_condition_user.is_person;
	user_data[25]=air_condition_user.air_fault;
}
/************************************************************************************
 *description:copy the information of the orignal block to the new block
 *input: orignal block number,new block number
 *output:none
************************************************************************************/
void Block_Copy_Block(uint16_t block_old,uint16_t block_new)
{
	uint16_t i;
	uint8_t buf[2048];
	for(i=0;i<64;i++)
	{
		Flash_Read_Data(block_old, i,0,2048,buf);
		Flash_Write_Data(block_new, i,0,2048,buf);
	}
}
//write user data to the flash
void Flash_Write_UserData(struct air_condition air_condition_user)
{
	uint16_t block,page;
	List_Array(air_condition_user);
	//�����·������ļ�
	switch(air_condition_user.month)
	{
		case RTC_Month_June:
		case RTC_Month_July:
		case RTC_Month_August:
		case RTC_Month_September:
			block=300+temp_summer_length/64;
			page=temp_summer_length%64;
			while(Flash_Check_Bad_Block(block)==FLASH_BAD_BLOCK)
			{
				temp_summer_length+=64;
				block=300+temp_summer_length/64;
			}
			Flash_Write_Data(block, page, 0, 26, user_data);
			temp_summer_p->block=block;
			temp_summer_p->page=page;
			temp_summer_p->temp=air_condition_user.air_temp;
			temp_summer_length++;
			temp_summer_p=(temp_summer_list*)malloc(sizeof(temp_summer_list));
			temp_summer_p=temp_summer_p->next;
			break;
		case RTC_Month_October:
		case RTC_Month_November:
		case RTC_Month_April:
		case RTC_Month_May:
			block=900+temp_spring_length/64;
			page=temp_spring_length%64;
			while(Flash_Check_Bad_Block(block)==FLASH_BAD_BLOCK)
			{
				temp_spring_length+=64;
				block=900+temp_spring_length/64;
			}
			Flash_Write_Data(block, page, 0, 26, user_data);
			temp_spring_p->block=block;
			temp_spring_p->page=page;
			temp_spring_p->temp=air_condition_user.air_temp;
			temp_spring_length++;
			temp_spring_p=(temp_spring_list*)malloc(sizeof(temp_spring_list));
			temp_spring_p=temp_spring_p->next;
			break;
		case RTC_Month_December:
		case RTC_Month_January:
		case RTC_Month_February:
		case RTC_Month_March:
			block=300+temp_winter_length/64;
			page=temp_winter_length%64;
			while(Flash_Check_Bad_Block(block)==FLASH_BAD_BLOCK)
			{
				temp_winter_length+=64;
				block=300+temp_winter_length/64;
			}
			Flash_Write_Data(block, page, 0, 26, user_data);
			temp_winter_p->block=block;
			temp_winter_p->page=page;
			temp_winter_p->temp=air_condition_user.air_temp;
			temp_winter_length++;
			temp_winter_p=(temp_winter_list*)malloc(sizeof(temp_winter_list));
			temp_winter_p=temp_winter_p->next;
			break;
	}
}
/************************************************************************************
 * ������BMP_Page_Read()
 * ��������ȡһҳ��bmpλͼ��Ϣ
 * ���룺page--��ȡҳ��ţ�ʵ����ҳΪ��ʶ���ܹ淶��Ӧ�ÿ顢ҳ��ϵ���ʽ��
 * �����pdata--��ȡ���ݴ��λ��
 * ���أ�0--ʧ��
				 1--�ɹ�
************************************************************************************/
uint8_t BMP_Page_Read(uint32_t page, uint8_t *pdata)
{
	//uint8_t readoutdata[2048];
	uint16_t block_num;
	uint8_t page_num, ret;
	block_num = page/64;
	page_num = page%64;
	ret = Flash_Read_Data(block_num, page_num, 0, 2048, pdata);
	if(ret == FLASH_OPERATE_OK)
		return 1;
	else
		return 0;
}

/************************************************************************************
 * ������Flash_Test()
 * ��������ÿһ���ÿһҳ����д�벢������⣬��д��������Ƿ�һ�£��������̳����Ͼã�
				 ʵ�ʶԲ��ֿ顢ҳ���в��Լ��ɡ�
 * ���룺�� 
 * �������
 * ���أ���
************************************************************************************/
void Flash_Test(void)
{
	char c;
	const uint16_t num = 2048;
	uint16_t ret, i;
	uint16_t j, k;
	uint8_t writebuf[num], readbuf[num];
	for(i = 0; i < num; i++)
	{
		writebuf[i] = 2048-i;
	}
	//Flash_Erase_The_Device();
	if(FLASH_OPERATE_OK != Flash_Read_ID())
	{
		while(1);//fail
	}
	//ret = Flash_Erase_Block(0);
	//if(ret == FLASH_OPERATE_OK)
	{	
		ret = Flash_Read_Data(0, 7, 0, num, readbuf);
		ret = Flash_Write_Data(0, 7, 0, num, writebuf);
		if(ret == FLASH_OPERATE_OK)
					//debug_printf("Block NO.%d Page NO.%d test OK!\r\n");	
		ret = Flash_Read_Data(0, 7, 0, num, readbuf);
	}
}