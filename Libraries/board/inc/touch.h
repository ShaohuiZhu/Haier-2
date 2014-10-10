/*
 ******************************************************************************
 *              COPYRIGHT 2013 �ֿƵ��ӹ�����
 *
 *�ļ���touch.h
 *���ߣ��ֿƵ��ӹ�����
 *���ڣ�2013.12.20
 *�汾��V1.0 
 *������lcd touch����ͷ�ļ��� 
 ******************************************************************************
 */
#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "stm32f4xx.h"

extern void Touch_Init(void);
extern uint8_t Touch_Detected(void);
extern uint16_t Get_TouchADX(void);
extern uint16_t Get_TouchADY(void);

#endif /* __TOUCH_H__ */
