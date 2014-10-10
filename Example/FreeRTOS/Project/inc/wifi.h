#ifndef __WIFI_H_
#define __WIFI_H_
#include "includes.h"

#include "typedefine.h"
#include "modecmd.h"
#include "Gcom.h"
#include "NetEnDeCode.h"
#ifndef UHOMECOM_GLOBAL
#define UHOMECOM_EXT extern
#else
#define UHOMECOM_EXT
#endif

#define RECV_DATA_NOT_OPERATING 0
#define RECV_DATA_OPERATING 1

#define UART_OPERATE_OK       1
#define UART_OPERATE_ERROR    0

#define WIFI_RECV_NOT_EXCEED 0
#define WIFI_RECV_EXCEED 1

#define UHomeCom_BUSFREETMR (50*5) 


#define WIFI_UART             			 USART1
#define WIFI_UART_RCCCLK      			 RCC_APB2Periph_USART1
#define WIFI_UART_GPIO_AF    				 GPIO_AF_USART1
#define WIFI_UART_IRQ         			 USART1_IRQn
//#define WIFI_UART_IRQHandler				 USART1_IRQHandler

#define WIFI_UART_TX_GPIO      			 GPIOA
#define WIFI_UART_TX_PINS 			 		 GPIO_PinSource9
#define WIFI_UART_TX_PIN       			 GPIO_Pin_9
#define WIFI_UART_TX_RCCCLK    			 RCC_AHB1Periph_GPIOA
	
#define WIFI_UART_RX_GPIO       		 GPIOA
#define WIFI_UART_RX_PINS 			 		 GPIO_PinSource10
#define WIFI_UART_RX_PIN        		 GPIO_Pin_10
#define WIFI_UART_RX_RCCCLK    			 RCC_AHB1Periph_GPIOA

#define UHOMEMAX_IUNUM 16

extern bool isWifiValid;				// �ж�WIFI ģ����Ч

typedef enum{
	UHOMECOM_INIT = 0,	// ��ʼ��
	UHOMECOM_IDLE,		// ����
	UHOMECOM_CFG,			// ����
	UHOMECOM_NORMAL, //����ģʽ
	UHOMECOM_RX,			// ����
	UHOMECOM_TX		// ��������
}UHOMECOMSTATE;

typedef enum{
	FRM_NONE = 0,				//��Ӧ��֡
	FRM_REPLYCONFIRM,		// �ظ�ȷ��֡
	FRM_REPLYCONFIRM65,	//65ȷ��֡
	FRM_REPORTSTATE,		// �㱨״̬֡
	FRM_INVALID,				// ��Ч֡
	FRM_ALARM,					// ����֡
	FRM_IDCHK,					//�豸ʶ����Ӧ��
	FRM_VERSION,				//�豸�汾֡Ӧ��
	FRM_GETPASS,				//��ȡ��Կ֡
	FRM_REPORTALARM,		//�㱨����֡
	FRM_CONFIG,					//��������ģʽ֡
	FRM_NORMAL,					//��������ģʽ֡
	FRM_REPORTCONFIG,		//�豸�㱨����֡
}UHOMECOMFRAMETYPE;

typedef enum{
	OUTYP_UNDEF = 0,
	OUTYP_ALMPIC = 1,
	OUTYP_5IN1 = 2,
	OUTYP_HX7 = 3,
	OUTYP_SIDEFAN = 4,
	OUTYP_MIT5IN1 = 5,
	OUTYP_KVR = 6
}OUTYPE;

typedef struct {
	u8 mode;
	u8 state;
	u8 temp;
	u8 windspeed;
	u8 windUpDown;
	u8 windLeftRight;
}AirCondition;



typedef struct
{
//-----ͨѶ�������--------
    u8 ComSTS;               //�ڻ�ͨ��״̬��0��������ͨѶ��1������ͨѶ��У��ʹ���2������ͨѶ��У�����ȷ
    u8 ComErrCnt;            //ͨѶ���������������ͨѶ��У�����
//------���ʹ��-------------
    u8 _IUType;      //����
	
    u8 ValidExist       :1;
	u8 _FanSpeed        :2;  //���ת��
	u8 _DrainPump       :1;  //��ˮ��
	u8 _FloatSW         :1;  //���ӿ���
	u8 _Frz_Ovht        :2;  //���������
	u8 _res1            :1;
//---------ͨѶ��״̬���ݣ�----------------
    u8 _IOAddress;           //�����ͨ�ŵ�ַ
    u8 _CCAddress;           //D0-���ص�ַ

	u8 _DrvMode         :2;  //D1-����ģʽ
	u8 _CCCtlMode       :2;  //D1-����������ģʽ
    u8 _WCAddress       :4;  //D1-�����ַ
	
    u8 _D2TsetDef       :3;  //
	u8 _Mode_cmd        :4;  //D2-ģʽ����
    u8 _OnOff           :1;  //D2-���ػ�
	
	u8 _HP_CODE         :4;  //D3-HP code	
	u8 _Scode           :4;  //D3-Scode
	
    u8 _D4b10_Undef     :2;  //D4-δ����
    u8 _OutHealth       :1;  //D4-���������ת
    u8 _ErrClear1       :1;  //D4-����״̬���1
    u8 _RatingRun       :1;  //D4-���ת
    u8 _ForceRun        :1;  //D4-ǿ����ת
    u8 _TimeShort       :1;  //D4-��ʱ��־
    u8 _ForceDefrost    :1;  //D4-ǿ�Ƴ�˪��־

    u8 _FreshAir        :2;  //D5-�·�
    u8 _D5b2_Undef      :1;  //D5-δ����
    u8 _ErrClear2       :1;  //D5-����״̬���
    u8 _HealthCmd       :1;  //D5-������ת
    u8 _SwingCmd        :1;  //D5-�ڷ�
    u8 _FanSpeedCmd     :2;  //D5-�趨����

    u8 _FanChable       :1;  //D6-������������
    u8 _SwingEnable     :1;  //D6-�ڷ繦������
    u8 _HeatModeEnable  :1;  //D6-����ģʽ����
    u8 _AutoModeEnable  :1;  //D6-�Զ�ģʽ����
    u8 _Heater          :1;  //D6-�����״̬
	u8 _PreHeat         :1;  //D6-Ԥ��״̬
	u8 _StandBy         :1;  //D6-����״̬
	u8 _FilterReq       :1;  //D6-��ϴ����ָʾ

    u8 _ErrCode;             //D7-���ϴ���
//---------ͨѶ�����������ݣ�----------------
    s16 _TaiCmd;             //D2-�趨�¶�
    s16 _Tset;               //D2-�趨�¶ȣ���������
    s16 _Tai;                 //D2-�ڻ������¶�      
    s16 _Tc2;                 //D3-ϸ���¶�
    s16 _Tc1;                 //D4-�ֹ��¶�
    u16 _PMV;                 //D5-���ͷ���ǰ����
//-------------------------
    u16 _HP;
//	u32 _OnOffTimeSec;

	//------��ͨѶ���루���Э�����𲿷֣�----------
	u8 _OpMode            : 2;
	u8 _Quiet             : 1;
	u8 _RmCardDrv         : 1;
	u8 _CoActionDrv       : 1;
	u8 _DrySTS            : 1;
    u8 _Swing             : 1;  //�ڷ�
    u8 _Health            : 1;  //������ת
	
	u8 _PMVReason         : 4;
	u8 _ADGroupId         : 2;
	u8 _CoAction          : 1; //�������
	u8 _Res1              : 1;
	
	u8 _HtrOnReqPR        : 3; //ˮ�䡢���ȵ�Ԫ�ĵ���ȿ����������ȼ���0-�޿�������7-������ȼ���������
	u8 _OuCmdIuHtrOnEn    : 1; //���������������ڻ�������������źţ��ر�������һ�Ļ��ȵ�Ԫ������1������0������
	u8 _ReOuCmdIuHtrOnEn  : 1; //ˮ�䡢���ȵ�Ԫ������ĵ���ȿ��������źŵ�Ӧ�������źţ�
	u8 _OuCmdThermoOff    : 1; //��������ǿ�����ڻ�ThermoOff����1���ƣ���0�����ƣ�
	u8 _ReOuCmdThermoOff  : 1; //�������ڵ�Ԫ�������ǿ��ThermoOff�Ļ��ţ������źţ�
	u8 _ReOuCmdIuHtrOnReq : 1; //ˮ�䡢���ȵ�Ԫ������ĵ���ȿ��������źŵ�Ӧ�������źţ�
	
	//------���ȵ�Ԫ�·������ĺ�����ݣ����ȵ�Ԫ��ˮ�䡢��ů����ɻ���----------
	u8 _HU_WaterCmd       : 1; //����ˮ�趨����
	u8 _HU_FloorCmd       : 1; //��ů�趨����
	u8 _HU_DryCmd         : 1; //����趨����
	u8 _HU_SolarCmd       : 1; //̫�����趨����
  
	u8 _HU_WaterSts       : 1; //����ˮ״̬
	u8 _HU_FloorSts       : 1; //��ů״̬
	u8 _HU_DrySts         : 1; //���״̬
	u8 _HU_SolarSts       : 1; //̫����״̬

	u8 _HU_PowerSaveCmd   : 1; //�趨����
	u8 _HU_SterilizeCmd   : 1; //�趨����
	u8 _HU_QuietCmd       : 1; //�趨����
	u8 _HU_WaterCmdPower  : 1; //����ˮ�趨�������ͣ�ǿ������ˮ
	u8 _Res7              : 4;
	
    s16 _TfloorCmd;            //D2-��ů�趨�¶�
    s16 _TfloorSet;            //D2-��ů�趨�¶ȣ���������
    s16 _TdryCmd;              //����趨�¶�
    s16 _TdrySet;              //D2-�趨�¶ȣ���������
	s16 _Twi;
	s16 _Two;

	s16 _Twe;
	s16 _Tws;

	u8 _HU_D11;
	u8 _HU_D26;
	u8 _HU_D27;
	u8 _HU_D28;
	
    u16 _PMV2;          
    u16 _DCFanSpeed;          
	s16 _SHSCTar;
    s16 _Taf;              
    s16 _Tc22;          
    s16 _Tc12;
	
    u8  _DCFanDuty;          
	u8 _CommanderId;
	u8 _SoftVer;
} TIU;
typedef struct
{
	//OUTDOORTYPE _OuType; //D2-����ʶ���־
	//------ͨѶ����----------
	u8  _OpMode         :2;  //D0-��תģʽ
	u8  _OuCtlMode      :2;  //D0-����趨ģʽ��00-ͨ����01-���䣬02-���ȣ�03-�Ȼ���
    u8  _Address        :3;  //D0-��ַ
    u8  _HOffPMVSCctlFlg:1;  //D0b0 1/0 - ���ȸ�����Off����/���� ���������ڻ�������С��95Hz==8P����
	
	u8  _DefrostFlg     :1;  //��˪��ת��
	u8  _XRDefrostFlg   :1;  //���ȳ�˪��ת��
	u8  _Res1           :6;  //Ԥ��
	
	u16 _INVSpeed;           //D1-ѹ������תƵ��
	
	u8  _ressss         :3;  //
	u8  _TimerShort     :1;  //D2-��ʱ
	u8  _CompBackupRun  :1;  //D2-ѹ��������ת
	u8  _PdBackupRun    :1;  //D2-��ѹ����������ת��
	u8  _SH_MODE        :2;  //D2-�ڻ����ȶȿ��Ʒ�ʽ

	u8  _MisMatchCheck  :1;  //D3-�����߼����ת��
	u8  _OilRetFlg      :1;  //D3-������ת��
	u8  _OilBalFlg      :1;  //D3-������ת��
	u8  _ForceRun       :1;  //D3-ǿ����ת��
	u8  _QuietRun       :1;  //D3-��������ת��
	u8  _PowerSave      :1;  //D3-������ת
	u8  _AutoAddress    :1;  //D3-��ַ�Զ��趨��
	u8  _RateRun        :1;  //D3-���ת��

    u8  _OnIUCtl_PMV_EN :1;  //D4-�����ڻ���ThermoOn��PMVʹ��
    u8  _OffIUCtl_PMV_EN:1;  //D6-�����ڻ���ThermoOff��PMVʹ��
    u8  _OnIUCtl_PMV_MODE:2; //D5-����Կ������ڻ���������ON��PMV���Ƚ�����������
    u8  _CoolSHoffsetDir :1; //D5-�����ڻ����ȶ���������1��ʾ+��0��ʾ-
    u8  _CoolSHoffsetVal :1; //D5-�����ڻ����ȶ�����ֵ��1��ʾ1��0��ʾ0
    u8  _Res2            :2; //D5-Ԥ��
	
	u16 _OnIUCtl_PMV_Step;   //D4-on PMVĿ�꿪��
	u16 _OffIUCtl_PMV_Step;  //D6-off PMVĿ�꿪��
	
    s16  _Pd_temp;	
    s16  _Ps_temp;
    s16  _Tdi;     
    s16  _Tsi;     
    s16  _Tao;     
    s16  _Tdef;   
    s16  _Toili;  	
    s16  _Tfin;
	
    u8  _ErrCode;
	u8 _ComStage;  //ͨѶ���̽���  =0��ͨ����=1����ַ������ʼ��=2����ַ��������

	//------��ͨѶ���루���Э�����𲿷֣�----------
	u8  _SoftVer;
	u8  _RunSts;
	u8  _RunStsStep;
    u8  _ErrCodeSub;
    s16 _SHoffset_IU;
	u16 _FanSpeed;
	u16 _LEVa;
	
	u8  _4WV    :1;
	u8  _SV1    :1;
	u8  _SV2    :1;
	u8  _CHi    :1;
	u8  _CHa    :1;
	u8  _SVx    :1;
	u8  _Res3   :2;

	u8  _CmdIUTHermoOff     : 1;   //ǿ�����ڻ�ThermoOff����1���ƣ���0�����ƣ�
	u8  _5in1HeaterOnEnable : 1;   //�������һ��ˮ�䡢�廻������������źţ���1������0������
	u8  _5in1HeaterOnReq    : 1;   //�������һ��ˮ�䡢�廻������������źţ���1���󣬣�0�ޣ�
	u8  _res5               : 5;
	
	
	/*��Ϊ�����������*/	
	u8  _E2Ver;
	u8  _CT;          
	u8  _Icm;         /*  ѹ��������  */
	u8  _INVDBV;      /*  (TSL)ѹ��DC Bus voltage����λ��4V */  //�ߵ�ѹ����
	u16 _FanSpeed2;
	u8  _Data1;
	u8  _Data2;
	u8  _Data3;
	u8  _Data4;
	u8  _Data5;
	
	//����������:����ȫ��/��̨�ڻ�PMV ����ר��
	u8 _PMVPosEnCmd	    	:1;  //PMVĿ�꿪��ʹ��λ
	u8 _HeatTC2TempEnCmd	:1;  //�����ڻ�Tc2Ŀ���¶�ʹ��λ
	u8 _HeatTarSCEnCmd	    :1;  //�����ڻ�Ŀ������ʹ��λ
	u8 _COOLTarSHEnCmd	    :1;  //�����ڻ�Ŀ����ȶ�ʹ��λ
	u8 _ThermoOnPMVRateEnCmd:1;  //ThermoOn�ڻ� PMVĿ�꿪��Ϊ��ǰ���ȵı���ʹ��λ
	u8 _Ret5EnCmd 	        :3;  //Ԥ��
	
	u16 _PMVPosCmd;              //PMVĿ�꿪��
	s16 _Tc2Cmd;                 //�����ڻ�Tc2Ŀ���¶�
	s16 _TarSHSCCmd;             //�����ڻ�Ŀ����ȶȣ�Tc1-Tc2��/�����ڻ�Ŀ�����ȣ�Pd-Tc2��
	u8  _ThermoOnPMVRateCmd;     //ThermoOn�ڻ� PMVĿ�꿪��Ϊ��ǰ���ȵı�������Χ50-200��  ��50��ʾ50%��200��ʾ200%��
	
	u8 _FanCtlEnCmd	    	:1;  //�ڻ��������ʹ��λ
	u8 _FanCtlCmd	    	:2;  //bit 2-1: �ڻ����� ��0-ͣ  1-�ͷ� 2-�з� 3-�߷磩
	u8 _Ret6EnCmd 	        :5;  //Ԥ��
	
} TOU;
uint8_t Wifi_Data_Ready(void);
void Wifi_GPIO_Init(void);
void Wifi_Uart_Init(void);
void Wifi_Uart_SendByte(uint8_t c);
void Wifi_Uart_SendData(uint8_t *DAta, uint32_t DAtaNUm);
void Wifi_UART_IRQHandler(void);

void Wifi_Uart(void);
void Wifi_Test(void);
void RxData_Deal(void);
void Test_All(void);
void UhomeCom_Reset(void);		// uhomeͨѶ��ʼ��
void AlarmTmrRun();
extern OUTYPE OutDoorUnitType;		// ������ͣ����ݻ��ͱ���Ӧ���ϴ���

bool HasNewCmd_UHOME(void);

void SetNewCmdFlag_UHOME(void);

#endif //__WIFI_H
