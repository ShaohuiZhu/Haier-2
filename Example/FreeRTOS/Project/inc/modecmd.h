#ifndef __MODECMD_H
#define __MODECMD_H

#ifndef MODECMD_GLOBAL
#define MODECMD_EXT extern
#else
#define MODECMD_EXT
#endif

#include "typedefine.h"

typedef enum{
    CID_NONE    = 0,   //��
    CID_WC      = 1,   //�߿���
    CID_RC      = 2,   //ң����(A/B��)
    CID_CC      = 3,   //������
    CID_OU      = 4,   //������趨
    CID_RMCD    = 5,   //����
    CID_COACT   = 6,   //��������
    CID_PRCL    = 7,   //ͣ�粹��
	CID_KEY     = 8,   //ң�ؽ��հ尴��
    CID_ONOFFSW = 9,   //�����ź�
	CID_PC      = 10,   //Pc
	CID_UHOME = 11
} COMMANDERID;

typedef enum{
    MD_STOP = 0,
    MD_COOL = 1,
    MD_HEAT = 2,
    MD_AUTO = 3,
} MODE;

typedef enum{
    MDCC_AUTO        = 0,
    MDCC_BLOW        = 1,
    MDCC_COOL        = 2,
    MDCC_DRY         = 3,
    MDCC_HEAT        = 4,  //��ͨ����
} CCMODECMD;

typedef enum{
    CMS_NONE = 0, //�޼��п�����
    CMS_FIFO = 1, //��������
    CMS_CCTL = 2, //���п���
    CMS_LOCK = 3  //����
} CMODESELCMD;

typedef enum{
    FN_AUTO = 0,
    FN_LOW  = 1,
    FN_MID  = 2,
    FN_HIGH = 3,
    FN_FIXED= 4,
    FN_UND5 = 5,
    FN_UND6 = 6,
    FN_UND7 = 7
} FANCMD;

typedef enum{
    FA_NONE = 0,  //��
    FA_AUTO = 1,  //�Զ�����
    FA_HEAT = 2,  //ȫ�Ȼ���
    FA_COMM = 3   //��ͨ����
} FRESHAIRCMD;

typedef enum{
    PL_STOP = 0,  //ֹͣ
    PL_UP = 1,    //����
    PL_DN = 2,    //�½�
    PL_STDBY = 3  //����
} PNLFTCMD;

typedef enum{
    TSW_ON = 0,      //��ʱ��
    TSW_OFF = 1,     //��ʱ��
    TSW_ONOFF = 2,   //��ʱ����
    TSW_RPT = 3      //��ʱ����ѭ��
} TMRSWCMD;

typedef enum{
	HSW_POS1 = 0,//λ��1
	HSW_POS2 = 1,//λ��2
	HSW_POS3 = 2,//λ��3
	HSW_POS4 = 3,//λ��4
	HSW_POS5 = 4,//λ��5
	HSW_POS6 = 5,//λ��6
	HSW_POS7 = 6,//λ��7
	HSW_AUTO = 7,//�Զ�
	HSW_NONE = 10//
} HSWINGTYPE;

typedef enum{
	VSW_NONE    =  0,
	VSW_HLTH_UP =  1,//���������ϴ���
	VSW_POS1    =  2,//λ��1
	VSW_HLTH_DN =  3,//���������´���
	VSW_POS2    =  4,//λ��2
	VSW_POS3    =  6,//λ��3
	VSW_POS4    =  8,//λ��4
	VSW_POS5    = 10,//λ��5
	VSW_AUTO    = 12 //�Զ�
} VSWINGTYPE;

//---------cmd---------------------
MODECMD_EXT COMMANDERID    CommanderId;
MODECMD_EXT bool           CCTurnOnCmd;        //CCTurnOnCmd��CCModeCmd��ϵõ����ػ���ģʽ����������Ӧ��
MODECMD_EXT MODE           ModeCmd;            //ModeCmd��BlowCmd��DryCmd��ϵõ����ػ���ģʽ���߿���ң������Ӧ��
MODECMD_EXT CCMODECMD      CCModeCmd;
MODECMD_EXT CMODESELCMD    CcModeSelCmd;       //���п���ģʽѡ��
MODECMD_EXT s16            TaiCmd;             //�趨�¶ȣ����ڸ���������ͨѶ��
MODECMD_EXT FANCMD         FanSpeedCmd;
MODECMD_EXT VSWINGTYPE     VSwingCmd;          //��ֱ�ڷ�

MODECMD_EXT bool           TestCmd;
//MODECMD_EXT bool         TimeShortCmd;
MODECMD_EXT bool           FilterCmd;
MODECMD_EXT FRESHAIRCMD    FreshAirCmd;
MODECMD_EXT bool           HealthCmd;
MODECMD_EXT bool           FcDefrostCmd;       //ǿ�Ƴ�˪����
MODECMD_EXT bool           FcDefrostSTS;       //ǿ�Ƴ�˪״̬���յ�ǿ�Ƴ�˪����True��״̬��1������True����������ͳ�˪���
MODECMD_EXT bool           FcRunCmd;
MODECMD_EXT bool           RateRunCmd;
MODECMD_EXT PNLFTCMD       PanelLiftCmd;
MODECMD_EXT TMRSWCMD       TimerSwitchCmd;
MODECMD_EXT bool           TimerSwitchEnable;
MODECMD_EXT bool           DustCatcherCmd;        //����
MODECMD_EXT bool           QuickCmd;           //��Ч
MODECMD_EXT bool           PowerSaveCmd;
MODECMD_EXT bool           SleepCmd;
MODECMD_EXT bool           QuietCmd;
MODECMD_EXT bool           DryCmd;             //��ʪ
MODECMD_EXT bool           BlowCmd;            //�ͷ�
MODECMD_EXT bool           HeaterCmd;
MODECMD_EXT bool           StoreConfigCmd;
MODECMD_EXT bool           LockCmd;
MODECMD_EXT bool           BrouseCmd;
MODECMD_EXT bool           BkLightCmd;
MODECMD_EXT HSWINGTYPE     HSwingCmd;          //ˮƽ�ڷ�
MODECMD_EXT bool           BlinkCmd;
MODECMD_EXT u8 			   TimerOffTask_Enabled_old;
MODECMD_EXT u8 		       TimerOnTask_Enabled_old;
MODECMD_EXT bool           SleepCmd_old;
MODECMD_EXT u16 		   TimingBlinkTmr;

// UHOME ����
MODECMD_EXT bool HumDetectCmd;	// �˸�


MODECMD_EXT bool           AddWaterCmd;
MODECMD_EXT bool           ShowTempCmd;
MODECMD_EXT bool           NatualWindCmd;
MODECMD_EXT bool           FreqHoldCmd;
MODECMD_EXT bool           LightSenseCmd;
MODECMD_EXT bool           BodySenseCmd;
MODECMD_EXT bool           GasAlarmCmd;
MODECMD_EXT bool           OxygenCmd;
MODECMD_EXT s16            HumCmd;
MODECMD_EXT u8             SetAirTemp_Sel;  // ѡ��ư���ʾ�趨�¶�or��ʾ�����¶ȣ�Ĭ��[255]��ʾ�趨�¶ȣ�Ϊ0ʱ��ʾ���£� */          
MODECMD_EXT u8             NLEDTai_Cmd;
MODECMD_EXT u8             NLDEOFF_Cmd;

MODECMD_EXT s16 Tset;        //�ڻ�ʵ���趨�¶ȣ�TaiCmd + ����/�����¶Ȳ��� + ˯���¶Ȳ�����
MODECMD_EXT s16 Tcomp_CL;    //�����¶Ȳ������Զ�ģʽ��SCODE���㡢����ȿ��ء��Զ������ж϶������ã�
MODECMD_EXT s16 Tcomp_HT;    //�����¶Ȳ������Զ�ģʽ��SCODE���㡢����ȿ��ء��Զ������ж϶������ã�
MODECMD_EXT s16 Tcomp_Sleep; //˯���¶Ȳ���
MODECMD_EXT s16 Tdif;        //ֻ���Զ�ģʽת���ж���������
extern s16 FreshCool_TasR;  //    00C        12    �·������Ŀ���¶�TasR��[18]����λ��                 
extern s16 FreshHeat_TasR;  //    00D        16    �·������Ŀ���¶�TasR��[22]����λ��                 

void StdCmdMk_Reset(void);
void StdCmdMk(void);
void SETTaiCompensation_RC(void);       //ң�����趨�¶Ȳ���
void TaiCompensationMk(void);//ѡ���¶Ȳ���

#define HasNewCmd(dst) HasNewCmd_##dst()                // ��ѯĳ��ͨѶģ�飬�Ƿ��յ���ָ��
#define SetNewCmdFlag(dst) SetNewCmdFlag_##dst() // ֪ͨĳ��ͨѶģ�飬����ģ��ı���ָ����ѱ�ϵͳ����
#define GetCmd(dst,cmd) Get##cmd##_##dst()              // ��ѯĳ��ͨѶģ���յ���ָ��ָ��ֵ

#define IsVSwingCmdAUTO() (((PCB_WEDGE_THIN_NEW != PCBType&&VSW_HLTH_UP == VSwingCmd) || (PCB_WEDGE_THIN_NEW != PCBType&&VSW_HLTH_DN == VSwingCmd) || VSW_AUTO == VSwingCmd )? TRUE : FALSE) //71�Զ��ڷ�
#define IsHSwingCmdAUTO() ((HSW_AUTO==HSwingCmd)? TRUE:FALSE)

#endif