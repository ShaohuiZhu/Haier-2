#define TIMER_GLOBAL
/** @file timer.c
  * @author  ������
  * @version V1.0.0
  * @date    2009��10��
  * @brief   ��ʱ��ʵ���ļ�.
  ******************************************************************************
  * @copy
  *
  *     ��ʱ��ģ��ռ��һ��Ӳ����ʱ����Դ�����ö�ʱ�жϣ��ж�����250΢�롣
  *     �ڴ�ģ����ʵ���ṩ�����ඨʱ����
  *     - ͨ�ö�ʱ�����һ�������񣬶Զ�ʱ�������Ǻܸߵ�Ӧ�á��ṩ�������ԣ�
  *       - ��ʱ���ȣ�25ms,100ms,1sec,5sec,10sec,1min,5min��10min.
  *       - ����ģʽ���ݼ��������ݼӼ�����
  *       - ��ʱ���ܣ�����ѡ��֧����ʱ�����߲�֧����ʱ����ʱ�������Ե��ڡ�
  *       - ��ͣ���ܣ��������κ�ʱ����ͣ��ʱ�������¼�ʱ��
  *       - �趨�Ͳ�ѯ��ʱֵ�����ڵݼ���ʱ�������趨��ʱ���ȣ��ݼӼ�ʱ�ļ�ʱ���ȶ���0xffff��
  *     - ���ٶ�ʱ����Զ�ʱ����Ҫ��ߣ���ӦѸ�ٵĶ�ʱ���󣬾��Ⱦ�Ϊ250us���ݼ�������������ʱ��������ͣ
  *     - ����ʱ���ﵽ�趨ʱ����Զ�ִ��ĳ��������
  *     .
  *
  * <h2><center>&copy; COPYRIGHT 2009 �ൺ�����յ��������޹�˾</center></h2>
  */
#include "timer.h"
#include "wifi.h"
#ifdef DOXYGEN
#define _static
#define _extern
#else
#define _static static
#define _extern extern
#endif
 

//! ��ѭ��25ms��ʱ��־
TimerFlag MainTimerFlag = {0, 0};
//! ϵͳ�Ƿ���ʱ�ı�־
u8 TimeShortFlag = 0;
//! ϵͳ���յ���ʱָ��
u8 TimeShortCmd = 0;

 
//! ͨ�ö�ʱ��ר�����ݽṹ
struct
{
    u16 Counter;         //!< ��������
    u8 State:2;          //!< ��ʱ״̬��TimerState���ͣ���ֱ�Ӳ���TimerState����ԭ����Ϊ�˽�ʡRAM�ռ�
    u8 Type:4;           //!< ��ʱ���ȣ�TimerType���ͣ���ֱ�Ӳ���TimerType����ԭ����Ϊ�˽�ʡRAM�ռ�
    u8 Direction:1;      //!< �ݼӻ��ǵݼ���TimerDirection���ͣ���ֱ�Ӳ���TimerDirection����ԭ����Ϊ�˽�ʡRAM�ռ�
    u8 ShortEnable:1;    //!< ��ʱ��־��TimerShort���ͣ���ֱ�Ӳ���TimerShort����ԭ����Ϊ�˽�ʡRAM�ռ�
} TimerBuffer[TIMER_COUNT] = {
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
    {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

//! ����ʱ��������ģֵ
//! ����ͨ�ö�ʱ�����ͣ����鵥Ԫ����ֵ * 25ms = ��ʱ��λ
const u16 TimerPattern[TIMER_TYPE_COUNT+1] = {1,4,40,200,400,2400,12000,24000,0};
//! ��׼��ʱר�ýṹ��һ��ṹ����ʱ�ṹ
struct
{
    u16 Counter;  //!< ʱ���ļ���ֵ
    u16 Value;    //!< ʱ��������ֵ
} TimerBase[TIMER_TYPE_COUNT] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},       //!<����ʱ������
  TimerBase_Short[TIMER_TYPE_COUNT] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; //!<��ʱʱ������


//! ����˺֧꣬�ֶ�ʱ����ȡ���˺꣬����֧��
#define TIMERTASK_ENABLED
#ifdef TIMERTASK_ENABLED
//! ��ʱ���������ݽṹ�ͱ���
struct
{
    u16 Counter_25ms:12; //!< ����ļ�ʱ��
    u16 Enabled:1;       //!< �Ƿ���Ч
    u16 Running:1;       //!< �Ƿ��������б�ʶ
    u16 ShortEnable:1;   //!< �Ƿ������ʱ
    u16 Res:1;           //!< ������λ
    u16 Param;           //!< ִ������ʱ��������Ĳ���
    u8 (*Handler)(u8,u16);//!< ������ָ��
} TimerTasks[TIMERTASK_COUNT]={
    {0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};
#endif
/*****************************************************************************************
 * ������Tenms_Timer_Config����
 * ��������ʱ����ʼ������������������Ҫ���ý����жϵ�ʱ��������ο�mcu�ֲᣬ
				 Ŀǰֻ�������趨��ʱ�䣬����10ms
 * ���룺��
 * �������
 * ����: ��
*****************************************************************************************/
void Tenms_Timer_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 199;			   
	TIM_TimeBaseStructure.TIM_Prescaler = 83;    	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM7, ENABLE);
}
/***************************************************************************************
 * ������Tenms_Timer_NVIC_Config����
 * �������ж��������
 * ���룺��
 * �������
 * ����: ��
****************************************************************************************/
void Tenms_Timer_NVIC_Config(void) 
{
	NVIC_InitTypeDef NVIC_InitStruct; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 

	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn; 
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;  
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStruct); 
}

void Tenms_Timer_Init(void)
{
	Tenms_Timer_Config();
	Tenms_Timer_NVIC_Config();
}

/****************************************************************************
 * ������TIM7_IRQHandler����
 * �������жϴ�����
 * ���룺��
 * �������
 * ����: ��
****************************************************************************/
void TIM7_IRQHandler(void)
{
	static u8 tmr25ms = 0,tmr32ms = 0, tmr2ms = 0, tmr1ms = 0;
	if(TIM_GetFlagStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
    
    //! @sp4 1. ά����ѭ��25ms��ʱ��־���Լ�2ms��ʱ��־, �μ�@ref MainTimerFlag
    if(0 != tmr1ms) tmr1ms--;
    else
    {
        if(0 != tmr2ms ) tmr2ms = 0; else {MainTimerFlag.MS2 = 255; tmr2ms  =  2 - 1;}    
        if(0 != tmr25ms) tmr25ms-- ; else {MainTimerFlag.MS25 =  1; tmr25ms = 25 - 1;}
        if(0 != tmr32ms) tmr32ms-- ; else {MainTimerFlag.MS32 =  1; tmr32ms = 32 - 1;}    
        MainTimerFlag.MS1 = 0x1F; tmr1ms = 1 * 5 - 1;
    }
		if(0 != FastTimerBuffer[0]) FastTimerBuffer[0]--;
    if(0 != FastTimerBuffer[1]) FastTimerBuffer[1]--;
    if(0 != FastTimerBuffer[2]) FastTimerBuffer[2]--;
    if(0 != FastTimerBuffer[3]) FastTimerBuffer[3]--;
    if(0 != FastTimerBuffer[4]) FastTimerBuffer[4]--;
    if(0 != FastTimerBuffer[5]) FastTimerBuffer[5]--;
    if(0 != FastTimerBuffer[6]) FastTimerBuffer[6]--;
    if(0 != FastTimerBuffer[7]) FastTimerBuffer[7]--;
    if(0 != FastTimerBuffer[8]) FastTimerBuffer[8]--;
	}
}



//! @brief  ��ʱ��ά��,25msִ��һ��.
//! @param  ��
//! @retval ��
void updateTimer(void)
{
    //! <h3>ʵ�ֹ��̣�</h3>
    u16 idx;
    u8 temp;
    static struct
    {
        u8 Delay:7;
        u8 Flag:1;
    } TimeShort_Struct={0,0};
    static u8 tmr1sec;
    
    Tick_1sec_Flg = 0;
    
    if(0 != tmr1sec) tmr1sec--;                //1sʱ����ʱ������
    else
    {
        if(ThermoOnOffTimeSec < (U32_MAX - 60))
        {
            if(0 == TimeShortFlag) ThermoOnOffTimeSec++; else ThermoOnOffTimeSec += 60;
        }
        if(INVOnOffTimeSec < (U32_MAX - 60))
        {
            if(0 == TimeShortFlag) INVOnOffTimeSec++; else INVOnOffTimeSec += 60;
        }
        Tick_1sec_Flg = 1; tmr1sec = 40 * 1 - 1;
    }

    //! 3. ά��ʱ������
    //! <br/>@sp4����ʱ���ļ���ֵcounter�ݼӣ���������ʱ��, counter��1��������ʱʱ��, counter����ʱ������
    //! <br/>@sp4����ֵcounter�ﵽ�򳬹�ģֵ @ref TimerPattern ��ʱ�����������ֵcounter��ģֵ @ref TimerPattern ���̸���ʱ��������value��������������ֵcounter��
    for(idx = 0;idx < TIMER_TYPE_COUNT;++idx)
    if(TimerPattern[idx] != 0)
    {
        //����ʱ��
        TimerBase[idx].Counter++;
        if(TimerBase[idx].Counter >= TimerPattern[idx])
        {
            TimerBase[idx].Counter = 0;
            TimerBase[idx].Value = 1;
        }
        else TimerBase[idx].Value = 0;
        
        //��ʱʱ��
        if(TimeShortFlag != 0)
        {
            TimerBase_Short[idx].Counter += TIMER_SHORT_RATE;
            if(TimerBase_Short[idx].Counter >= TimerPattern[idx])
            {
                TimerBase_Short[idx].Value = TimerBase_Short[idx].Counter / TimerPattern[idx];
                TimerBase_Short[idx].Counter -= TimerBase_Short[idx].Value * TimerPattern[idx];
            }
            else TimerBase_Short[idx].Value = 0;
        }
        else 
        {
            TimerBase_Short[idx].Counter = 0;
            TimerBase_Short[idx].Value = 0;
        }
    }
    else break;
    
    //! 4. ͨ�ö�ʱ��������ά��
    //! <br/>@sp4���ά���Ѿ�ָ�ɣ�����û����ͣ�Ķ�ʱ����
    //! <br/>@sp4@sp4�ٲ���Ҫ��ʱ�Ķ�ʱ��
    //! <br/>@sp4@sp4@sp4�ݼ���ʱ������ֵÿ�μ�ȥ��Ӧ����ʱ��������ֵ��ֱ����Ϊֹ��
    //! <br/>@sp4@sp4@sp4�ݼ���ʱ������ֵÿ�μ�����Ӧ����ʱ��������ֵ��ֱ��0xffffΪֹ��
    //! <br/>@sp4@sp4����Ҫ��ʱ�Ķ�ʱ��
    //! <br/>@sp4@sp4@sp4�ݼ���ʱ������ֵÿ�μ�ȥ��Ӧ��ʱʱ��������ֵ��ֱ����Ϊֹ��
    //! <br/>@sp4@sp4@sp4�ݼ���ʱ������ֵÿ�μ�����Ӧ��ʱʱ��������ֵ��ֱ��0xffffΪֹ��
    for(idx = 0;idx < TIMER_COUNT;++idx)
    if(TimerBuffer[idx].State == TIMER_RUNNING &&       //��ʱ������ת��
       TimerPattern[TimerBuffer[idx].Type] != 0)        //��ʱ�������Ѿ�����
    {
        
        if((TimeShortFlag == 0 || TimerBuffer[idx].ShortEnable == TIMER_SHORT_DISABLE) &&
        TimerBase[TimerBuffer[idx].Type].Value != 0)
        {
            //����ʱ��ʱ����ά��
            if(TimerBuffer[idx].Direction == TIMER_DEC)
            {
                //�ݼ���ʱ����0ֹͣ
                if(TimerBuffer[idx].Counter > TimerBase[TimerBuffer[idx].Type].Value)
                    TimerBuffer[idx].Counter -= TimerBase[TimerBuffer[idx].Type].Value;
                else TimerBuffer[idx].Counter=0;
            }
            else
            {
                //�ݼӼ�ʱ����65535ֹͣ
                if(TimerBuffer[idx].Counter < ~(TimerBase[TimerBuffer[idx].Type].Value))
                    TimerBuffer[idx].Counter += TimerBase[TimerBuffer[idx].Type].Value;
                else TimerBuffer[idx].Counter = 0xffff;
            }
        }
        else if(TimeShortFlag != 0 && TimerBuffer[idx].ShortEnable  == TIMER_SHORT_ENABLE &&
            TimerBase_Short[TimerBuffer[idx].Type].Value != 0)
        {
            //��ʱ��ʱ����ά��
            if(TimerBuffer[idx].Direction == TIMER_DEC)
            {
               //�ݼ���ʱ����0ֹͣ
                if(TimerBuffer[idx].Counter > TimerBase_Short[TimerBuffer[idx].Type].Value)
                    TimerBuffer[idx].Counter -= TimerBase_Short[TimerBuffer[idx].Type].Value;
                else TimerBuffer[idx].Counter = 0;
            }
            else
            {
                //�ݼӼ�ʱ����65535ֹͣ
                if(TimerBuffer[idx].Counter < ~(TimerBase_Short[TimerBuffer[idx].Type].Value))
                    TimerBuffer[idx].Counter += TimerBase_Short[TimerBuffer[idx].Type].Value;
                else TimerBuffer[idx].Counter = 0xffff;
            }
        }
    }

    //! 5. ά����ʱ���񣬴˹��ܿ���TIMERTASK_ENABLED������
    //! <br/>@sp4ά�����������еģ���Ч�Ķ�ʱ����
    //! <br/>@sp4@sp4��������Ķ�ʱ����һ���ﵽָ��ʱ�䣬�������������
    //! <br/>@sp4@sp4������������ֵ������������Ժ���ִ�У��ͷ���Դ
    //! <br/>@sp4@sp4���������ط���ֵ�����ָ����񣬵���ͣ��ʱ��Ҫ�����ִ�У�������������
#ifdef TIMERTASK_ENABLED
    for(idx = 0;idx < TIMERTASK_COUNT;++idx)
    if(TimerTasks[idx].Enabled!=0 && TimerTasks[idx].Running!=0)
    {
        //���¶�ʱ��
        if(TimeShortFlag == 0 || TimerTasks[idx].ShortEnable == 0)
        {
            if(TimerTasks[idx].Counter_25ms!=0)TimerTasks[idx].Counter_25ms--;
        }
        else if(TimerTasks[idx].Counter_25ms >= TIMER_SHORT_RATE)TimerTasks[idx].Counter_25ms-=TIMER_SHORT_RATE;
        else TimerTasks[idx].Counter_25ms = 0;
        
        //ִ�ж�ʱ����
        if(TimerTasks[idx].Counter_25ms==0)
        {
            temp=0;
            if(TimerTasks[idx].Handler!=0)
                temp=TimerTasks[idx].Handler(idx,TimerTasks[idx].Param);
                
            if(temp==0)
            {
                TimerTasks[idx].Enabled=0;
                TimerTasks[idx].Running=0;
                TimerTasks[idx].ShortEnable=0;
                TimerTasks[idx].Res=0;
                TimerTasks[idx].Param=0;
                TimerTasks[idx].Handler=0;
            }
            else TimerTasks[idx].Running=0;
        }
    }
#endif
}

/**
  * @brief ����ͨ�ö�ʱ��.
  * @param type : ��ʱ���Ķ�ʱ�������͡���ȡһ����ֵ��
  *   @arg@ref TIMER_25MS
  *   @arg@ref TIMER_100MS 
  *   @arg@ref TIMER_SEC 
  *   @arg@ref TIMER_5SEC  
  *   @arg@ref TIMER_10SEC 
  *   @arg@ref TIMER_MIN
  *   @arg@ref TIMER_5MIN
  *   @arg@ref TIMER_10MIN
  * @param  direction : ָ����ʱ���ǵݼӼ�ʱ���ǵݼ���ʱ�� ��ȡһ����ֵ��
  *     @arg@ref TIMER_DEC : ��ʾ�ݼӼ�ʱt
  *     @arg@ref TIMER_INC : ��ʾ�ݼ���ʱ
  * @param  shortEnable : ָ����ʱ���Ƿ������ʱ��
  *  <br>@sp4����ȡ@ref TIMER_SHORT_ENABLE ��ʾ������ʱ������ȡ@ref TIMER_SHORT_DISABLE ��ʾ������ʱ
  * @param  state ��ָ����ʱ����״̬������ȡ����ֵ��
  *     @arg@ref TIMER_IDLE����ʾû�б�ָ�ɣ�һ������²��ô�ֵ��һ��ʹ�ñ�ʾ���ͷŸö�ʱ����
  *     @arg@ref TIMER_STOP����ʾ��ͣ
  *     @arg@ref TIMER_RUNNING����ʾ��������
  * @param  value : ָ����ʱ�������ĳ�ʼֵ����ʾ���ٸ���ʱ���ȼ������ʱ�������
  * @retval ���䵽�Ķ�ʱ�����
  */
GeneralTimer getGTimer(TimerType type,TimerDirection direction,
    TimerShort shortEnable,TimerState state,u16 value)
{
    /** <h3>ʵ�ֹ��̣�</h3> */
    u16 idx;
    
    /** @arg 1.�� state = TIMER_IDLE��������Ч�������� */
    if(state == TIMER_IDLE)return INVALIDTIMER;
    
    /** @arg 2.����������ʱ�����������ҵ�һ��TIMER_IDLE״̬�Ķ�ʱ�����ͷ����������š�
      * - Ҫ���� @ref configGTimer ����
      */
    for(idx = 0;idx < TIMER_COUNT; ++idx)
    {
        if(TimerBuffer[idx].State == TIMER_IDLE)
        {
            configGTimer(idx,type,direction,shortEnable,state,value); 
            return (GeneralTimer)idx;
        }
    }
    
    /** @arg 3.�Ҳ���TIMER_IDLE״̬�Ķ�ʱ�����ͷ�����Ч�������� */
    return INVALIDTIMER;
}

/**
  * @brief �ͷ�ͨ�ö�ʱ��
  * @param tmr : Ҫ�ͷŵĶ�ʱ�����
  * @retval ��
  */
void freeGTimer(GeneralTimer tmr)
{
    if(tmr < TIMER_COUNT)
    {
        TimerBuffer[tmr].State = TIMER_IDLE;
        TimerBuffer[tmr].Direction = TIMER_DEC;
        TimerBuffer[tmr].ShortEnable = TIMER_SHORT_DISABLE;
        TimerBuffer[tmr].Type = TIMER_TYPE0;
        TimerBuffer[tmr].Counter = 0;
    }
}

/**
  * @brief ����ͨ�ö�ʱ��
  * @param tmr : Ҫ���õĶ�ʱ�����
  * @param type : ��ʱ���Ķ�ʱ�������͡���ȡһ����ֵ��
  *   @arg@ref TIMER_25MS
  *   @arg@ref TIMER_100MS 
  *   @arg@ref TIMER_SEC 
  *   @arg@ref TIMER_5SEC  
  *   @arg@ref TIMER_10SEC 
  *   @arg@ref TIMER_MIN
  *   @arg@ref TIMER_5MIN
  *   @arg@ref TIMER_10MIN
  * @param  direction : ָ����ʱ���ǵݼӼ�ʱ���ǵݼ���ʱ�� ��ȡһ����ֵ��
  *     @arg@ref TIMER_DEC : ��ʾ�ݼӼ�ʱt
  *     @arg@ref TIMER_INC : ��ʾ�ݼ���ʱ
  * @param  shortEnable : ָ����ʱ���Ƿ������ʱ��
  *  <br>@sp4����ȡ@ref TIMER_SHORT_ENABLE ��ʾ������ʱ������ȡ@ref TIMER_SHORT_DISABLE ��ʾ������ʱ
  * @param  state ��ָ����ʱ����״̬������ȡ����ֵ��
  *     @arg@ref TIMER_IDLE����ʾû�б�ָ�ɣ�һ������²��ô�ֵ��һ��ʹ�ñ�ʾ���ͷŸö�ʱ����
  *     @arg@ref TIMER_STOP����ʾ��ͣ
  *     @arg@ref TIMER_RUNNING����ʾ��������
  * @param  value : ָ����ʱ�������ĳ�ʼֵ����ʾ���ٸ���ʱ���ȼ������ʱ�������
  * @retval ��
  */
void configGTimer(GeneralTimer tmr,TimerType type,TimerDirection direction,
    TimerShort shortEnable,TimerState state,u16 value)
{
    if(tmr<TIMER_COUNT)
    {
        if(state == TIMER_IDLE)
        {
            freeGTimer(tmr);
            return;
        }
        else TimerBuffer[tmr].State = state;
        TimerBuffer[tmr].Direction = direction;
        TimerBuffer[tmr].Type = type;
        TimerBuffer[tmr].ShortEnable = shortEnable;
        TimerBuffer[tmr].Counter = value;
    }
}

/**
  * @brief ��ѯͨ�ö�ʱ������ֵ
  * @param tmr : Ҫ��ѯ�Ķ�ʱ�����
  * @retval ��ǰ����ֵ
  */
u16 getGTimerValue(GeneralTimer tmr)
{
    if(tmr < TIMER_COUNT)return TimerBuffer[tmr].Counter;
    else return 0;
}
/**
  * @brief �趨ͨ�ö�ʱ������ֵ
  * @param tmr : Ҫ���õĶ�ʱ�����
  * @param time : ����ֵ
  * @retval ��
  */
void setGTimerValue(GeneralTimer tmr, u16 time)
{
    if(tmr < TIMER_COUNT)TimerBuffer[tmr].Counter = time;
}

/**
  * @brief �ָ���ʱ������
  * @param tmr : Ҫ�����Ķ�ʱ�����
  * @retval ��
  */
void startGTimer(GeneralTimer tmr)
{
    if(tmr < TIMER_COUNT)
    if(TimerBuffer[tmr].State != TIMER_IDLE)TimerBuffer[tmr].State = TIMER_RUNNING;
}

/**
  * @brief ��ͣ��ʱ������
  * @param tmr : Ҫ�����Ķ�ʱ�����
  * @retval ��
  */
void stopGTimer(GeneralTimer tmr)
{
    if(tmr < TIMER_COUNT)
    if(TimerBuffer[tmr].State != TIMER_IDLE)TimerBuffer[tmr].State = TIMER_STOP;    
}

#ifdef TIMERTASK_ENABLED

/**
  * @brief �趨��ʱ����,��������ֵ������ʱ������ִ����Ϻ��Զ����������, ����255��ʾ����ʧ��
  * @param ms : ִ��������ǰ��ʱ�����ֵ
  * @param f :������ָ��, ����Ϊu8 (*)(u8,u16)
  * @param param :��������Ҫ�Ĳ���
  * @param short_enable :�Ƿ�֧����ʱ, �����㣬��ʾ������ʾ������ֵ����ʾ������ʱ
  * @retval ��ʱ���������ֵ
  */
u8 setTimerTask(u16 ms,u8 (*f)(u8,u16),u16 param,u8 short_enable)
{
    u8 idx;
    
    for(idx=0; idx<TIMERTASK_COUNT; ++idx)
    {
        if(TimerTasks[idx].Enabled==0)
        {
            TimerTasks[idx].Enabled=1;
            TimerTasks[idx].Counter_25ms=(ms+12)/25;
            TimerTasks[idx].Running=1;
            TimerTasks[idx].ShortEnable=short_enable;
            TimerTasks[idx].Res=0;
            TimerTasks[idx].Param=param;
            TimerTasks[idx].Handler=f;
            return idx;
        }
    }
    return 255;
}

/**
  * @brief ȡ���������ʱ����
  * @param idx : ��ʱ���������ֵ
  * @retval �� 
  */
void clearTimerTask(u8 idx)
{
    if(idx<TIMERTASK_COUNT)
    {
        TimerTasks[idx].Enabled=0;
        TimerTasks[idx].Running=0;
        TimerTasks[idx].ShortEnable=0;
        TimerTasks[idx].Res=0;
        TimerTasks[idx].Param=0;
        TimerTasks[idx].Handler=0;
        TimerTasks[idx].Counter_25ms=0;
    }
}

/**
  * @brief �����趨����ʱ��
  * @param idx : ��ʱ���������ֵ
  * @param ms : ִ��������ǰ��ʱ�����ֵ
  * @retval ��
  */
void setTimerTaskValue(u8 idx,u16 ms)
{
    if(idx<TIMERTASK_COUNT && TimerTasks[idx].Enabled!=0)
    {
        TimerTasks[idx].Counter_25ms=(ms+12)/25;
        TimerTasks[idx].Running=1;
    }
}

/**
  * @brief �趨�������
  * @param idx : ��ʱ���������ֵ
  * @param param : �������Ĳ���ֵ
  * @retval ��
  */
void setTimerTaskParam(u8 idx,u16 param)
{
    if(idx<TIMERTASK_COUNT && TimerTasks[idx].Enabled!=0)
        TimerTasks[idx].Param=param;
}

/**
  * @brief �ָ���ʱ����
  * @param idx : ��ʱ���������ֵ
  * @retval ��
  */
void resumeTimerTask(u8 idx)
{
    if(idx<TIMERTASK_COUNT && TimerTasks[idx].Enabled!=0)
        TimerTasks[idx].Running=1;
}

/**
  * @brief ����ʱ����
  * @param idx : ��ʱ���������ֵ
  * @retval ��
  */
void suspendTimerTask(u8 idx)
{
    if(idx<TIMERTASK_COUNT && TimerTasks[idx].Enabled!=0)
        TimerTasks[idx].Running=0;
}

#endif