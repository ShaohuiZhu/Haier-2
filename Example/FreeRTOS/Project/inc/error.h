#ifndef ERROR_INCLUDE_H
#define ERROR_INCLUDE_H
#include "typedefine.h"

#ifndef ERR_GLOBAL
#define ERR_EXT extern
#else
#define ERR_EXT
#endif

typedef union
{
	u16 Total;
	struct
	{
		u16 E2PROM       :1;   //5
		u16 WCCom        :1;   //7
		u16 Pump         :1;   //8
		u16 DupIOAd      :1;   //9
		u16 Def10        :1;   //10
		u16 Def11        :1;   //11---��
		u16 ZeroChk      :1;   //12---50Hz���㡢���ߺ�P��ͨ�Ź���
		u16 IOCom        :1;   //13
		u16 DCFan        :1;
		u16 Def7         :7;
	} Type;
} ERROR_TYPE;

ERR_EXT ERROR_TYPE  ErrorInfo;        	 //���ϱ�ʶ
ERR_EXT u8 Error;     //���ϴ���
ERR_EXT u8 SuperErr;  //�������ϴ��룬��Error ������ �ػ������Error=0��SuperErr=���ϣ����������Error=SuperErr=���ϣ�

#define ERR_E2PROM   ErrorInfo.Type.E2PROM  
#define ERR_Def6     ErrorInfo.Type.Def6    
#define ERR_WCCom    ErrorInfo.Type.WCCom   
#define ERR_Pump     ErrorInfo.Type.Pump    
#define ERR_DupIOAd  ErrorInfo.Type.DupIOAd 
#define ERR_Def11    ErrorInfo.Type.Def11   
#define ERR_ZeroChk  ErrorInfo.Type.ZeroChk
#define ERR_IOCom    ErrorInfo.Type.IOCom   
#define ERR_DCFan    ErrorInfo.Type.DCFan   

#define MAXZEROLOST 8   //����n(8�൱��5-7��)���Ҳ��������, ������
#define MAXZEROVALUELOST  MAXZEROLOST*2/5

void CheckError(void);

#endif