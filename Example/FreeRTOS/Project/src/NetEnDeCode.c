/*=========================================================
* �� �� ��: NetEnDeCode.c
* ��    Ȩ: �ൺ�����Ƽ����޹�˾  
* ��������: ����Э��2.0���ܽ�����س���

* �޸�����			�޸���				�޸�˵�� 
  2013-3-18       �����            ���������ܽ��ܻ������� 
========================================================= */
/**********************************************************
*ʹ��˵����
1.void f_GetCodeKey(unsigned char *pCodeKeyIndex,unsigned char i_CodeKeyNum)
            pCodeKeyIndex ��Ҫ������Կ��������Ҫ�������������׵�ַ��
            i_CodeKeyNum ��Կ����������Ϊu8_CipherkeyBuf ������Կ
2.void f_GetEncodeData(unsigned char *pEncodeData,
      		unsigned int i_EnCodeDataNum,unsigned char i_CodeKeyNum)  
       pEncodeData ��Ҫ�������ݣ�����Ϊ���������׵�ַ��
       i_CodeKeyNum ��Կ����
       i_EnCodeDataNum ��Ҫ������������
3.void f_GetDecodeData(unsigned char *pEncodeData,
      		unsigned int i_EnCodeDataNum,unsigned char i_CodeKeyNum)  
       pEncodeData ��Ҫ�������ݣ�����Ϊ���������׵�ַ��
       i_CodeKeyNum ��Կ����
       i_EnCodeDataNum ��Ҫ������������        
**********************************************************/
#include "NetEnDeCode.h"							/* ����ͷ�ļ� */
#include "Table_NetCode.h"							/* ������������� */

unsigned char u8_NetCodeKeyBuf[CODE_KEY_NUM];

/**********************************************************
;  ģ������:  void f_GetCodeKey(unsigned char *pCodeKeyIndex,
                   unsigned char i_CodeKeyNum)
;	ģ�鹦��:  ��ȡ������Կ 
;	���룺	pCodeKeyIndex ��Ҫ������Կ������i_CodeKeyNum ��Կ����
;	�����	u8_CipherkeyBuf ������Կ  
;**********************************************************/
void f_GetCodeKey(unsigned char i_CodeKeyIndex,unsigned char i_CodeKeyNum)
{
   unsigned int i;
  
   for (i=0; i< i_CodeKeyNum; i++)
   {
      u8_NetCodeKeyBuf[i] = TABLE_CODE_KEY[(unsigned  int)(i_CodeKeyIndex + i) % 256];
   }
}
/**********************************************************
;  ģ������:  void f_GetEncodeData(unsigned char *pEncodeData,
                     unsigned int i_DataNum,unsigned char i_CodeKeyNum)
;	ģ�鹦��:  ��ȡ�������� 
;	���룺	pEncodeData ��Ҫ�������ݣ�i_CodeKeyNum ��Կ����
            i_EnCodeDataNum ��Ҫ������������ 
;	�����	���ܺ�����  
;**********************************************************/
void f_GetEncodeData(unsigned char *pEncodeData,
      unsigned int i_EnCodeDataNum,unsigned char i_CodeKeyNum)
{
   unsigned int i;
   
   for(i=0; i< i_EnCodeDataNum; i++)
   {
     pEncodeData[i] ^= u8_NetCodeKeyBuf[i% i_CodeKeyNum];
   }
}
/**********************************************************
;  ģ������:  void f_GetDecodeData(unsigned char *pDecodeData,
                     unsigned int i_DeCodeDataNum,unsigned char i_CodeKeyNum)
;	ģ�鹦��:  ��ȡ�������� 
;	���룺	pEncodeData ��Ҫ�������ݣ�i_CodeKeyNum ��Կ����
            i_DeCodeDataNum ��Ҫ������������ 
;	�����	���ܺ�����  
;**********************************************************/
void f_GetDecodeData(unsigned char *pDecodeData,
      unsigned int i_DeCodeDataNum,unsigned char i_CodeKeyNum)
{
   unsigned int i;
   
   for(i=0; i< i_DeCodeDataNum; i++)
   {
     pDecodeData[i] ^= u8_NetCodeKeyBuf[i% i_CodeKeyNum];
   }
}