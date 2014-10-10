#ifndef __DC_PROTOCOL_H
#define __DC_PROTOCOL_H

#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
//#include "semphr.h"
#include "DC_Config.h"
#include "DC_Timer.h"
#include "stm32f4xx.h"

typedef unsigned char BYTE;

/*   �߿���״̬�����޸� */
typedef enum 
{
	IDLE_STATUS      = 0x00,       /*	����/�ȴ�����״̬			*/
	SENDING_STATUS   = 0x01,			 /* ��ʼ����							*/ 
	WAIT_TO_RECEIVE  = 0x02,		   /*	�ȴ�����״̬					*/
	BEGIN_TO_RECEIVE = 0x03,		   /*	��ʼ����/ʶ������״̬	*/
	RECEIVE_ERROR    = 0x04,		   /*	�����ֽڳ���					*/
	RECEIVE_COMPLETED= 0x05,		   /*	�����ֽ����					*/
	CHECK_ERROR      = 0x06        /*	У�����							*/
}CONTROL_STATUS;

/*    ��������ȡ�ṹ��    */
typedef struct
{
	BYTE OOFlag;     //���ػ���־
	BYTE Mode;   	 //ģʽ
	BYTE Temperature;//�¶�
	BYTE WindSpeed;  //����
	BYTE WDFlag;     //�ڷ�
	BYTE HeatFlag;   //���ȱ�־
	BYTE HealthFlag; //������־
	BYTE FreshAir;   //�·�״̬
} MemoryStatus;

/*   ����һ֡���ֽڳ���    */
#define LENGTH_OF_SEND_FRAME 6
/*   ����һ֡���ֽڳ���    */
#define LENGTH_OF_RECEIVE_FRAME 5

/*	2s  = 10000 * 200us	*/
#define RESEND_TIME 	10000
/*	4min = 1200000 * 200us 	*/
#define SHUTDOWN_TIME  1200000

/*  ���޸�  �������ĵ�ַ����   */
#define MEMORY_BASE_ADDR   ((uint32_t) 0x20000000)
#define MEMORY_ADDR (MEMORY_BASE_ADDR + 0x00000000)

/*   �������Ĵ洢���ݽṹ   */
#define MEMORY_DATA ((MemoryStatus *)MEMORY_ADDR)

/*    
        ���ݸ�ʽ��LSB 
	ͨ��ԭ��
          ������ : 24ms �� 
	       0   : 12ms ��
	       1   :  4ms ��
	     ��� :  4ms ��
*/

#define GUIDANCE_MS 24
#define NUM0_MS 12
#define NUM1_MS 4
#define INTERNAL_MS 4

/*   bit defines    */
#define POS_D0  ((BYTE)0x01)
#define POS_D1  ((BYTE)0x02)
#define POS_D2  ((BYTE)0x04)
#define POS_D3  ((BYTE)0x08)
#define POS_D4  ((BYTE)0x10)
#define POS_D5  ((BYTE)0x20)
#define POS_D6  ((BYTE)0x40)
#define POS_D7  ((BYTE)0x80)

#define POS_D02 ((BYTE)0x07)
#define POS_D03 ((BYTE)0x0F)
#define POS_D04 ((BYTE)0x1F)
#define POS_D05 ((BYTE)0x3F)
#define POS_D06 ((BYTE)0x7F)
#define POS_D25 ((BYTE)0x3C)
#define POS_D45 ((BYTE)0x30)
#define POS_D67 ((BYTE)0xC0)

/*	ģʽ����	*/
#define AutoMode ((BYTE)0x00)
#define CoolMode ((BYTE)0x01)
#define WetMode  ((BYTE)0x02)
#define AirMode  ((BYTE)0x03)
#define HeatMode ((BYTE)0x04)
#define ComfMode ((BYTE)0x05)

#define DC_DelayMS(x)  vTaskDelay((x)/portTICK_RATE_MS);
/*	�ڻ�״̬	*/
typedef struct 
{
	__IO BYTE IndoorTemperature;  //�����¶�
	
	__IO BYTE IsHeating;          //���ȱ�־
	__IO BYTE InletGridFlag;      //����դ����
	__IO BYTE MachineType;        //����
	__IO BYTE ErrorInfo;          //������Ϣ
	
	__IO BYTE OnOffFlag;          //���ػ���־
	__IO BYTE WindSwingFlag;      //���Űڶ�
	__IO BYTE WaterFlag;          //ˮ��ˮ����־
	__IO BYTE HeaterFlag;         //���ȱ�־
	__IO BYTE Mode;
	
	__IO BYTE WindSpeed;
	__IO BYTE SetTemperature;    //�趨�¶�
	__IO BYTE RemoteControl;     //Զ�̿���
	__IO BYTE RemoteLock;        //Զ������
} MachineStatus;

void DC_Config(void);				/*	ֱ��Э���ʼ��	*/

/***************************************************
 * ������isOn --- �Ƿ�ػ���0���ػ�   1������
 *      mode --- ģʽ��     1������   2����ʪ   4������
 *        temperature --- �趨�¶ȣ�0 --- 16  ...... 14----30
 *	       windSpeed --- ���٣�0���Զ�    1���ͷ�    2���з�    3���߷� 
 *       PAN --- �Ƿ�ڷ�    0��ֹͣ�ڷ�    1�� ��ʼ�ڷ�
 ***************************************************/
void DC_Control(u8 isOn, u8 mode ,u8 temperature, u8 windSpeed,u8 PAN); 
#endif
