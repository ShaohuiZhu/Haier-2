/** @file timer.h
  * @author  ������
  * @version V1.0.0
  * @date    2009��10��
  * @brief   ���ĵ�����ϵͳ���ඨʱ��.
  ******************************************************************************
  * @copy
  *
  *     ��ʱ��ģ��ռ��һ��Ӳ����ʱ����Դ�����ö�ʱ�жϣ��ж�����200΢�롣
  *     �ڴ�ģ����ʵ���ṩ�����ඨʱ����
  *     - ͨ�ö�ʱ�����һ�������񣬶Զ�ʱ�������Ǻܸߵ�Ӧ�á��ṩ�������ԣ�
  *       - ��ʱ���ȣ�25ms,100ms,1sec,5sec,10sec,1min,5min��10min.
  *       - ����ģʽ���ݼ��������ݼӼ�����
  *       - ��ʱ���ܣ�����ѡ��֧����ʱ�����߲�֧����ʱ����ʱ�������Ե��ڡ�
  *       - ��ͣ���ܣ��������κ�ʱ����ͣ��ʱ�������¼�ʱ��
  *       - �趨�Ͳ�ѯ��ʱֵ�����ڵݼ���ʱ�������趨��ʱ���ȣ��ݼӼ�ʱ�ļ�ʱ���ȶ���0xffff��
  *     - ���ٶ�ʱ����Զ�ʱ����Ҫ��ߣ���ӦѸ�ٵĶ�ʱ���󣬾��Ⱦ�Ϊ200us���ݼ�������������ʱ��������ͣ
  *     - ����ʱ���ﵽ�趨ʱ����Զ�ִ��ĳ��������
  *     .
  *
  * <h2><center>&copy; COPYRIGHT 2009 �ൺ�����յ��������޹�˾</center></h2>
  */
#ifndef TIMER_INCLUDE_H
#define TIMER_INCLUDE_H

#ifndef TIMER_GLOBAL
#define TIMER_EXT extern
#else
#define TIMER_EXT
#endif

#include "typedefine.h"
#include "gcom.h"
#include "stm32f4xx.h"
#include "includes.h"

#define sec * 1

//ʱ���������
#define TMR1SEC_250MS  ((u16)4*1-1)    //��λ250ms
#define TMR2SEC_250MS  ((u16)4*2-1)    //��λ250ms
#define TMR5SEC_250MS  ((u16)4*5-1)    //��λ250ms
#define TMR10SEC_250MS ((u16)4*10-1)   //��λ250ms
#define TMR20SEC_250MS ((u16)4*20-1)   //��λ250ms
#define TMR30SEC_250MS ((u16)4*30-1)   //��λ250ms
#define TMR80SEC_250MS ((u16)4*80-1)   //��λ250ms
#define TMR1MIN_250MS  ((u16)1*60*4)   //��λ250ms
#define TMR2MIN_250MS  ((u16)2*60*4)   //��λ250ms
#define TMR3MIN_250MS  ((u16)3*60*4)   //��λ250ms
#define TMR5MIN_250MS  ((u16)5*60*4)   //��λ250ms
#define TMR6MIN_250MS  ((u16)6*60*4)   //��λ250ms
#define TMR10MIN_250MS ((u16)10*60*4)  //��λ250ms
#define TMR15MIN_250MS ((u16)15*60*4)  //��λ250ms
#define TMR60MIN_250MS ((u32)60*60*4)  //��λ250ms
#define TMR240MIN_250MS ((u32)240*60*4)  //��λ250ms
#define TMR360MIN_250MS ((u32)360*60*4)  //��λ250ms

#define TMRDOT5SEC ((u16)20*1-1)    
#define TMR1SEC    ((u16)40*1-1)    //��25msΪ��λ SEC:��һ��ʹ��if(0 != Tmr) Tmr--; else...���޼�һ��ʹ��if(0 != Tmr) Tmr--; if(0 == Tmr)...
#define TMR2SEC    ((u16)40*2-1)    //��25msΪ��λ
#define TMR3SEC    ((u16)40*3-1)
#define TMR4SEC    ((u16)40*4-1)
#define TMR5SEC    ((u16)40*5-1)             
#define TMR10SEC   ((u16)40*10-1)
#define TMR15SEC   ((u16)40*15-1)
#define TMR20SEC   ((u16)40*20-1)
#define TMR30SEC   ((u16)40*30-1)
#define TMR45SEC   ((u16)40*45-1)
#define TMR60SEC   ((u16)40*60-1)            
#define TMR105SEC  ((u16)40*105-1)         
#define TMR120SEC  ((u16)40*120-1)         
#define TMR180SEC  ((u16)40*180-1)    
#define TMR300SEC  ((u16)40*300-1)

#define TMR1MIN    ((u16)60*1)    //����Ϊ��λ
#define TMR2MIN    ((u16)60*2)    //����Ϊ��λ
#define TMR3MIN   ((u16)60*3)     //����Ϊ��λ
#define TMR5MIN   ((u16)60*5)     //����Ϊ��λ
#define TMR6MIN   ((u16)60*6)     //����Ϊ��λ
#define TMR8MIN   ((u16)60*8)     //����Ϊ��λ
#define TMR10MIN   ((u16)60*10)   //����Ϊ��λ
#define TMR15MIN   ((u16)60*15)   //����Ϊ��λ
#define TMR20MIN   ((u16)60*20)   //����Ϊ��λ
#define TMR30MIN   ((u16)60*30)   //����Ϊ��λ
#define TMR60MIN   ((u16)60*60)   //����Ϊ��λ
#define TMR120MIN  ((u16)60*120)  //����Ϊ��λ

#define TMR20SEC_2S   ((u16)10)    //��2��Ϊ��λ
#define TMR3MIN_2S    ((u16)30*3)    //��2��Ϊ��λ
#define TMR10MIN_2S   ((u16)30*10)   //��2��Ϊ��λ
  
//! ��ʱ����������С:�ֽ�
#define TIMER_COUNT         64
//! ���Զ���Ķ�ʱ�����������
#define TIMERTASK_COUNT      8
//! main��������ѭ��������:ms
#define MAINLOOPPERIOD      25
//! ��Ч��ʱ�����
#define INVALIDTIMER        255
//! �ж���ʱ���Ƿ���Ч�ĺ�
#define IsValidTimer(t)     ((t)<TIMER_COUNT)
//! �ж���ʱ���Ƿ���Ч�ĺ�
#define IsInvalidTimer(t)   ((t)>=TIMER_COUNT)
//! Ԥ�����ͨ�ö�ʱ������
#define TIMER_TYPE_COUNT    8  
//! ��ʱ����
#define TIMER_SHORT_RATE    60  

  


//! 25���붨ʱ������, @sa @link TimerType TIMER_TYPE0@endlink
#define TIMER_25MS  TIMER_TYPE0
//! 100���붨ʱ������, @sa @link TimerType TIMER_TYPE1@endlink
#define TIMER_100MS TIMER_TYPE1  
//! 1�붨ʱ������, @sa @link TimerType TIMER_TYPE2@endlink
#define TIMER_SEC   TIMER_TYPE2  
//! 5�붨ʱ������, @sa @link TimerType TIMER_TYPE3@endlink
#define TIMER_5SEC  TIMER_TYPE3  
//! 10�붨ʱ������, @sa @link TimerType TIMER_TYPE4@endlink
#define TIMER_10SEC TIMER_TYPE4  
//! 60�붨ʱ������, @sa @link TimerType TIMER_TYPE5@endlink
#define TIMER_MIN   TIMER_TYPE5  
//! 5���Ӷ�ʱ������, @sa @link TimerType TIMER_TYPE6@endlink
#define TIMER_5MIN  TIMER_TYPE6  
//! 10���Ӷ�ʱ������, @sa @link TimerType TIMER_TYPE7@endlink
#define TIMER_10MIN TIMER_TYPE7 

#define LED_BEEP_2MS_MASK       (u16)0x01  //!< LED�ͷ�������2ms��ʱ��־
#define NLED_2MS_MASK           (u16)0x02  //!< NLED��2ms��ʱ��־
#define CASSETTESWING_2MS_MASK  (u16)0x04  //!< Ƕ����ڷ���������2ms��ʱ��־
#define SWG_2MS_MASK            (u16)0x08  //!< �ڷ���������2ms��ʱ��־

//! ���붨ʱ����������9����Ԫ
TIMER_EXT u16 FastTimerBuffer[9];

#define Uhome_BusFreeTmr 0

#define SetFastTmrVal(id, val) (FastTimerBuffer[id] = val) //�趨���붨ʱ������ֵ
#define GetFastTmrVal(id) (FastTimerBuffer[id])            //��ѯ���붨ʱ������ֵ

#define Tick_1sec_Flg MainTimerFlag._1sec   //��ѭ����1sec��ʱ��־���Զ�ά��

TIMER_EXT u32 ThermoOnOffTimeSec;
TIMER_EXT u32 INVOnOffTimeSec;

//! ��ʱ��״̬��ö��ֵ
typedef enum{
    TIMER_IDLE=0,   //!< û�б�ָ��
    TIMER_STOP=1,   //!< ��ͣ
    TIMER_RUNNING=2 //!< ���ڼ���
} TimerState;

//! ��ʱ����ʱ��ö��ֵ
typedef enum{
    TIMER_SHORT_DISABLE=0, //!< ������ʱ�Ķ�ʱ��
    TIMER_SHORT_ENABLE=1   //!< ������ʱ�Ķ�ʱ��
} TimerShort;

//! ��ʱ���������ǵݼ���ʱ
typedef enum{
    TIMER_DEC=0,  //!< ������ʱ
    TIMER_INC=1   //!< �ݼ���ʱ
} TimerDirection;

//! ��ʱ�����ͣ�ԭʼ���ͣ�ȡ0..7
typedef enum{        //�Լ����ƣ�������Ҫ���嶨ʱ�������࣬���ĳ������������
    TIMER_TYPE0=0,   //!< ��ʱ����-0
    TIMER_TYPE1=1,   //!< ��ʱ����-1
    TIMER_TYPE2=2,   //!< ��ʱ����-2
    TIMER_TYPE3=3,   //!< ��ʱ����-3
    TIMER_TYPE4=4,   //!< ��ʱ����-4
    TIMER_TYPE5=5,   //!< ��ʱ����-5
    TIMER_TYPE6=6,   //!< ��ʱ����-6
    TIMER_TYPE7=7    //!< ��ʱ����-7
} TimerType;

typedef u16 GeneralTimer;    //!< ͨ�ö�ʱ������
typedef u16 FastTimer;       //!< ���ٶ�ʱ�����ͣ��˴���200us��ʱ��,������ʱ���ݼ�������

//! ϵͳʹ�õļ�����Ҫ�Ķ�ʱ��־
typedef struct
{
  u16 MS25:1;  //!< ��ѭ��25ms��ʱ��־
  u16 MS32:1;  //!< ��ѭ��32ms��ʱ��־
  u16 _1sec:1; //!< 1sec��ʱ��־ ��ѭ��ʹ�ã��Զ�ά��
  u16 MS1:5;   //!< 5��1ms��ʱ��־
  u16 MS2:8;   //!< 8��2ms��ʱ��־
} TimerFlag;

//��ѭ��25ms��ʱ��־
extern TimerFlag MainTimerFlag;
//ϵͳ�Ƿ���ʱ�ı�־
extern u8 TimeShortFlag;  
//ϵͳ���յ���ʱָ��
extern u8 TimeShortCmd;        



//��ʱ��ά��, ����Ӳ����ʱ���Ķ�ʱ�жϣ��������ʱ����λ
void updateTimer(void); 

//���붨ʱ��93

GeneralTimer getGTimer(TimerType Type,TimerDirection Direction,
    TimerShort ShortEnable,TimerState State,u16 value);
//�ͷŶ�ʱ��
void freeGTimer(GeneralTimer);
//���ö�ʱ��
void configGTimer(GeneralTimer tmr,TimerType Type,TimerDirection Direction,
    TimerShort ShortEnable,TimerState State,u16 value);
//��ѯ����ֵ
u16 getGTimerValue(GeneralTimer);
//���ü���ֵ
void setGTimerValue(GeneralTimer,u16);
//��ͣ����
void stopGTimer(GeneralTimer);
//�ָ�����
void startGTimer(GeneralTimer);


//�趨��ʱ����,����255��ʾ����ʧ�ܣ���������ֵ������ʱ������ִ����Ϻ��Զ����������
//ms: ��ʱʱ�䣬���룬��С�ֱ���Ϊ25��f����������param�����ݸ��������Ĳ�����short_enable���Ƿ�������ʱ
//u8 setTimerTask(u16 ms,u8 (*f)(u8 idx,u16 param),u16 param=0,u8 short_enable=0);
u8 setTimerTask(u16 ms,u8 (*f)(u8,u16),u16 param,u8 short_enable);
//ȡ���������ʱ����
void clearTimerTask(u8 idx);
//�����趨����ʱ��
void setTimerTaskValue(u8 idx,u16 ms);
//�趨�������
void setTimerTaskParam(u8 idx,u16 param);
//�ָ���ʱ����
void resumeTimerTask(u8 idx);
//����ʱ����
void suspendTimerTask(u8 idx);

void Tenms_Timer_Init(void);
#endif 
