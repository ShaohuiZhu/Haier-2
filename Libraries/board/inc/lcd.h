/*
 ******************************************************************************
 *              COPYRIGHT 2013 �ֿƵ��ӹ�����
 *
 *�ļ���lcd.h
 *���ߣ��ֿƵ��ӹ�����
 *���ڣ�2013.12.20
 *�汾��V1.0 
 *������lcd����ͷ�ļ��� 
 ******************************************************************************
 */
#ifndef __LCD_H__
#define __LCD_H__
#include "stm32f4xx.h"
#include "sdram.h"


#define LCD_FRAME_BUFFER       SDRAM_BANK_ADDR
#define BUFFER_OFFSET          ((uint32_t)0x130000)


void     LCD_Init                   (void);
void     LCD_DisplayOn              (void);
void     LCD_DisplayOff             (void);

#endif /* __LCD_H__ */
