/*=========================================================
* �� �� ��: NetEnDeCode.h
* ��    Ȩ: �ൺ�����Ƽ����޹�˾  
* ��������: ���ļ���غ������궨�� 

* �޸�����			�޸���				�޸�˵�� 
* 2013-03-18		����� 			��ɻ������� 
========================================================= */

#ifndef _CIPHER_H
#define _CIPHER_H

#define  CODE_KEY_NUM     			16                 //��Կ���� 



// ����Ϊ�ڲ��������� 
void f_GetCodeKey(unsigned char i_CodeKeyIndex,unsigned char i_CodeKeyNum);
void f_GetEncodeData(unsigned char *pEncodeData,
      unsigned int i_EnCodeDataNum,unsigned char i_CodeKeyNum);
void f_GetDecodeData(unsigned char *pDecodeData,
      unsigned int i_DeCodeDataNum,unsigned char i_CodeKeyNum);
                   
#endif