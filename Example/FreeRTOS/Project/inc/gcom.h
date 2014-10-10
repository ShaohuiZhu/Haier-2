/** @file    gcom.h
  * @author  ������
  * @version V1.0.0
  * @date    2009��10��
  * @brief   ͨѶ����ͷ�ļ�
  ******************************************************************************
  * @copy
  *
  *     �����˸���ͨѶ���õ����ͣ��������궨��
  *
  * <h2><center>&copy; COPYRIGHT 2009 �ൺ�����յ��������޹�˾</center></h2>
  */
#ifndef GCOM_INCLUDE_H
#define GCOM_INCLUDE_H
#include "typedefine.h"

/** @addtogroup ����ͨѶ
  * ͨѶ����ģ��
  * @{
  */

/** @addtogroup GCom_Type
  * ͨѶ�������Ͷ���
  * @{
  */
  
/******************************************************************************
*
*             ��ʽ1 ���� ͨѶģ�鴦��֡ͷ֡β�����������ݷ�ɢ����
*
*******************************************************************************/

/**
  * @Brief ͨѶ����������ָ�� 
  * @param ComID �� ͨѶ ID
  * @param FrameID ��֡���� ID
  * @param DRT �����(@ref COM_UNPACK)���ǽ��(@ref COM_UNPACK)
  * @param buffer : ��������ݻ�����
  * @param size ��֡����
  * @retval ��
  */
typedef void (*Com_DataModule)(u8 ComID,u8 FrameID,u8 DRT,void* buffer,u8 size);

/**
  * @brief ������ͨѶ�����ݣ���ص�ģ��
  */
typedef struct
{
    Com_DataModule Module; //!< ģ���Ӧ�Ĵ���������ָ��
    u8 FrameID;            //!< ֡���� ID
    u8 DRT;                //!< ���(@ref COM_UNPACK)�����(@ref COM_UNPACK)��������(@ref COM_BOTH)
} ComModuleItem;

/** @} */

/** @addtogroup GCom_Macro
  * ͨѶ����궨��
  * @{
  */
  
//ͨѶ���ݷ���
#define COM_PACK    0    //!< ���ݽ��
#define COM_UNPACK  1    //!< ���ݴ��
#define COM_BOTH    2    //!< ���ݽ���ʹ��

/** 
  * @def DECLARE_COMMODULE 
  * �ڲ�ͬģ��ͷ�ļ����������ڵĴ�����ģ�� 
  * @param name : ģ�����ƣ����к��е�name������ͬ��
  * - @ref DECLARE_COMMODULE
  * - @ref IMPLEMENT_COMMODULE
  * - @ref ADD_COMMODULE
  */
#define DECLARE_COMMODULE(name) void name##_commodule(u8 ComID,u8 FrameID,u8 DRT,void* buffer,u8 size);

/** 
  * @def IMPLEMENT_COMMODULE 
  * �ڲ�ͬģ��ʵ���ļ���ʵ�ֵĴ�����ģ��(��ʼ) 
  * @param name : ģ�����ƣ����к��е�name������ͬ��
  * - @ref DECLARE_COMMODULE
  * - @ref IMPLEMENT_COMMODULE
  * - @ref ADD_COMMODULE
  */
#define IMPLEMENT_COMMODULE(name) void name##_commodule(u8 COMID,u8 FRAMEID,u8 DRT,void* BUFFER,u8 SIZE){
    
/** 
  * @def END_COMMODULE 
  * �ڲ�ͬģ��ʵ���ļ���ʵ�ֵĴ�����ģ��(����) 
  */
#define END_COMMODULE }

/** 
  * @def CREATE_COMMODULE_TABLE
  * ��ͨѶʵ���ļ��н���ͨѶ���ݴ����������ǼǱ�,�Ǽ���ͨѶ�����ݣ���ص�ģ��
  * @param ComID ��ComID�Ǹ�ͨѶ�����Ψһ��ʶ��������ϵͳ��һ��ͨѶģ��ֻ��һ����ʶ�������໥�����ظ���
  * ����@ref PACKDATA ��@ref UNPACKDATA �е�ComID����ͬ�ġ�
  */
#define CREATE_COMMODULE_TABLE(ComID) \
static u8 Com##ComID##_idx; \
static const ComModuleItem Com##ComID##_ModuleTable[]={
        
/** 
  * @def END_COMMODULE_TABLE 
  * ��ɴ����������ǼǱ�  
  */
#define END_COMMODULE_TABLE {0,0,0}};
    
/** 
  * @def ADD_COMMODULE 
  * �ڴ�������������������  
  * @param name : ģ�����ƣ����к��е�name������ͬ��
  * - @ref DECLARE_COMMODULE
  * - @ref IMPLEMENT_COMMODULE
  * - @ref ADD_COMMODULE
  * @param frameID : ֡����ID,ͬһ��ͨѶ��֡ID�ǲ��ظ��ģ�����Ӧ���շ�˫����һ�����ݽ�����
  * @param drt ����ʶ������֡�����ڱ�ģ��Ĵ��(@ref COM_UNPACK)�����(@ref COM_UNPACK)��������(@ref COM_BOTH)������
  * ֡ID��Ӧ��ѡ���Ĳ�����������@ref IMPLEMENT_COMMODULE�������ĺ�����ʵ�֡�
  */
#define ADD_COMMODULE(name,frameID,drt) {name##_commodule,frameID,drt},

/**
  * @def PACKDATA 
  * ͨѶ�˿ڷ��ͷ�����ǰ��Ҫ���õĴ������,
  * �����յ����ݴ����ע��Bufferֻ��ͨѶ���ݻ�������������֡ͷ��֡β���� 
  * @param comID : ComID�Ǹ�ͨѶ�����Ψһ��ʶ��������ϵͳ��һ��ͨѶģ��ֻ��һ����ʶ�������໥�����ظ���
  * ����@ref CREATE_COMMODULE_TABLE ��@ref UNPACKDATA �е�ComID����ͬ�ġ�
  * @param frameID : ֡����ID,ͬһ��ͨѶ��֡ID�ǲ��ظ��ģ�����Ӧ���շ�˫����һ�����ݽ�����
  * @param size ��֡����
  */
#define PACKDATA(comID,frameID,buffer,size) \
{                                                                                                  \
    Com##comID##_idx=0;                                                                            \
    while(Com##comID##_ModuleTable[Com##comID##_idx].Module!=0)                                    \
    {                                                                                              \
        if((Com##comID##_ModuleTable[Com##comID##_idx].FrameID==frameID) &&                        \
           (Com##comID##_ModuleTable[Com##comID##_idx].DRT==COM_PACK ||                            \
            Com##comID##_ModuleTable[Com##comID##_idx].DRT==COM_BOTH))                             \
            Com##comID##_ModuleTable[Com##comID##_idx].Module(comID,frameID,COM_PACK,buffer,size); \
        Com##comID##_idx++;                                                                        \
    }                                                                                              \
}
        
/**
  * @def UNPACKDATA
  * ͨѶ�˿ڷ��ͷ�����ǰ��Ҫ���õĽ������,
  * �����յ����ݽ����ע��Bufferֻ��ͨѶ���ݻ�������������֡ͷ��֡β����
  * @param comID : ComID�Ǹ�ͨѶ�����Ψһ��ʶ��������ϵͳ��һ��ͨѶģ��ֻ��һ����ʶ�������໥�����ظ���
  * ����@ref CREATE_COMMODULE_TABLE ��@ref PACKDATA �е�ComID����ͬ�ġ�
  * @param frameID : ֡����ID,ͬһ��ͨѶ��֡ID�ǲ��ظ��ģ�����Ӧ���շ�˫����һ�����ݽ�����
  * @param size ��֡����
  */
#define UNPACKDATA(comID,frameID,buffer,size) \
{                                                                                                    \
    Com##comID##_idx=0;                                                                              \
    while(Com##comID##_ModuleTable[Com##comID##_idx].Module!=0)                                      \
    {                                                                                                \
        if((Com##comID##_ModuleTable[Com##comID##_idx].FrameID==frameID) &&                          \
           (Com##comID##_ModuleTable[Com##comID##_idx].DRT==COM_UNPACK ||                            \
            Com##comID##_ModuleTable[Com##comID##_idx].DRT==COM_BOTH))                               \
            Com##comID##_ModuleTable[Com##comID##_idx].Module(comID,frameID,COM_UNPACK,buffer,size); \
        Com##comID##_idx++;                                                                          \
    }                                                                                                \
}

//! @}

/******************************************************************************
*
*                ��ʽ2 ���� ͨѶģ����ȫ������
*
*******************************************************************************/

/******************************************************************************
*
*                         ָ������������
* 
******************************************************************************/

/** @addtogroup GCom_Type
  * ͨѶ�������Ͷ���
  * @{
  */
 
/** ͨѶ�˿��յ��Ŀ���ָ��ṹ */
typedef struct
{
    u8 Value:7;      //!< ��ǰ����Чָ��
    u8 Changed:1;    //!< �����Ƿ�ı���
} COMMINST;

/** ָ����±�־ */
typedef struct
{
    u8 Myself:1; //!< ��ͨѶ�յ���ָ���Ҫ������Ӧ�ڲ�ָ��
    u8 Valid:1;  //!< ��ͨѶ��Ч�����Զ�д�������
    u8 Other:7;  //!< �ڲ��ո�����ָ���ͨѶ��Ҫ����Ӧ����
} CmdModifiedType;

typedef union
{
     //table��һ��key=���ݸ�����value=Ĭ��ֵ
     //���ֵ127
    u16 all;
    struct
    {
        u8 key;
        u8 value:7;
        u8 dir:1;
    } data;
} ParamMapTableItem;


/*********************************************************************************
*
*                          ͨѶ���������
*
*********************************************************************************/

//����ת������
typedef u16 (*TransformParam)(void);     //ת������ָ������
typedef struct
{
    u8 *buffer;
    TransformParam transform;
    //far u16 **table;
    u8 byte_idx;
    u8 bit_idx;
} FrameMacroType;

//! @}

/** @addtogroup GCom_Functions
  * ͨѶ������
  * @{
  */
u16 AddParam(u16 var,u16 offset);        //�Ӽ�,index=1
u16 SubParam(u16 var,u16 offset);        //�Ӽ�,index=2
u16 MulParam(u16 var,u16 m);             //�˷�,index=3
u16 DivParam(u16 var,u16 d);             //����,index=4
u16 LShiftParam(u16 var,u8 bit_count);   //��λ,index=5
u16 RShiftParam(u16 var,u8 bit_count);   //��λ,index=6
u16 ToASC(u16 var,u8 most_bit8);         //����תΪASCII����ѡת����8λ���ǵ�8λ,index=7
u16 FromASC(u16 var);                    //ASCIIתΪ����,index=8
u16 MapParam(u8 var,u16 *table,u8 dir);  //���,index=9


u16 frame_get_func(u8 size,FrameMacroType* frame_param);
void frame_set_func(u16 value,u8 size,FrameMacroType* frame_param);

//! @}

/** @addtogroup GCom_Macro
  * ͨѶ����궨��
  * @{
  */
#define frame_start(buffer,offset,transform,table) FrameMacroType frame_param={buffer+offset,transform,table,0,0}
#define frame_change_flag(flag) CmdModifiedType* this_frame_change_flag=&flag
#define frame_get(com_var,size) com_var=frame_get_func(size,&frame_param)
#define frame_set(com_val,size) frame_set_func(com_val,size,&frame_param)
#define frame_blank(size) \
    { \
        frame_param.bit_idx+=size;\
        if(frame_param.bit_idx>7) \
        { \
            frame_param.byte_idx+=(frame_param.bit_idx>>3);\
            frame_param.bit_idx -=((frame_param.bit_idx>>3)<<3); \
        } \
    }
#define frame_set_blank(size,val) \
    { \
        if(val==0)frame_set_func(0,size,&frame_param); \
        else frame_set_func((1<<size)-1,size,&frame_param); \
    }
#define frame_end()
#define frame_chk(old_var,new_var,flag) \
    { \
        if(old_var!=new_var) \
        { \
            flag=1; \
            if(this_frame_change_flag->Other==0)this_frame_change_flag->Myself=1; \
        } \
    }
#define frame_cmd_chk(var_cmd,var_com) \
    { \
        if(var_com.Value!=var_cmd) \
        { \
            var_com.Changed=1; \
            if(this_frame_change_flag->Other==0)this_frame_change_flag->Myself=1; \
        } \
    }
#define frame_get_cmd_chk(var_cmd,com_var,size) \
    { \
        frame_get(com_var.Value,size); \
        if(com_var.Value!=var_cmd) \
        { \
            com_var.Changed=1; \
            if(this_frame_change_flag->Other==0)this_frame_change_flag->Myself=1; \
        } \
    }
#define frame_get_cmd(com_var,size) frame_get(com_var.Value,size)

//! @}
//! @}
    
#endif