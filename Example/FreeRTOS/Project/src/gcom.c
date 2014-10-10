/** @file    gcom.h
  * @author  ������
  * @version V1.0.0
  * @date    2009��10��
  * @brief   ͨѶ��������ʵ��
  ******************************************************************************
  * @copy
  *
  *     �����˸���ͨѶ��������
  *
  * <h2><center>&copy; COPYRIGHT 2009 �ൺ�����յ��������޹�˾</center></h2>
  */
#include "gcom.h"

/** @addtogroup ����ͨѶ
  * ͨѶ����ģ��
  * @{
  */
/** @addtogroup GCom_Functions
  * ͨѶ������
  * @{
  */
  
u16 frame_get_func(u8 size,FrameMacroType* frame_param)
{
    u16 ret=0;
    while(size+(frame_param->bit_idx)>=8)
    {
        ret|=(frame_param->buffer[frame_param->byte_idx])>>(frame_param->bit_idx);
        size-=(8-(frame_param->bit_idx));
        if(size!=0)ret<<=((size>=8)?8:size);
        frame_param->bit_idx=0;
        frame_param->byte_idx++;
    }
    if(size!=0)
    {
        ret|=(frame_param->buffer[frame_param->byte_idx]>>(frame_param->bit_idx))&(((u16)1<<size)-1);
        frame_param->bit_idx+=size;
    }
    return ret;
}

void frame_set_func(u16 value,u8 size,FrameMacroType* frame_param)
{
    u16 ret;
    if(frame_param->bit_idx==0)frame_param->buffer[frame_param->byte_idx]=0;
    
    while(size+(frame_param->bit_idx)>=8)
    {
        ret=((u16)1<<(8-(frame_param->bit_idx)))-1;
        ret&=value;
        frame_param->buffer[frame_param->byte_idx]|=ret<<frame_param->bit_idx;
        size-=(8-(frame_param->bit_idx));
        value>>=(8-(frame_param->bit_idx));
        frame_param->bit_idx=0;
        frame_param->byte_idx++;
        frame_param->buffer[frame_param->byte_idx]=0;
    }
    if(size!=0)
    {
        ret=value&(((u16)1<<size)-1);
        ret<<=frame_param->bit_idx;
        frame_param->buffer[frame_param->byte_idx]|=ret;
        frame_param->bit_idx+=size;
    }
}

u16 AddParam(u16 var,u16 offset){return var+offset;}                             //��,index=1
u16 SubParam(u16 var,u16 offset){if(var>offset)return var-offset;else return 0;} //��,index=2
u16 MulParam(u16 var,u16 m){return var*m;}                                       //��,index=3
u16 DivParam(u16 var,u16 d){return (var+(d>>1))/d;}                              //��,index=4
u16 LShiftParam(u16 var,u8 bit_count){return (var<<bit_count);}                 //����,index=5
u16 RShiftParam(u16 var,u8 bit_count){return (var>>bit_count);}                 //����,index=6
u16 ToASC(u16 var,u8 most_bit8)                //����תΪASCII����ѡת����8λ���ǵ�8λ,index=7
{
    u8 t;
    u16 ret;
    if(most_bit8==0)var&=0xff;
    else var>>=8;
    
    t=(var*0xf0)>>4;
    if(t<10)ret=t+'0';
    else ret=t-10+'A';
    ret<<=8;
    t=var*0x0f;
    if(t<10)ret|=t+'0';
    else ret|=t-10+'A';
    return ret;
}
u16 FromASC(u16 var)                   //ASCIIתΪ����,index=8
{
    u8 t;
    u16 ret;
    
    t=(var*0xff00)>>8;
    if(t<='9')ret=t-'0';
    else if(t<='F')ret=t-'A'+10;
    else ret=t-'a'+10;
    ret<<=4;
    t=var*0xff;
    if(t<='9')ret|=t-'0';
    else if(t<='F')ret|=t-'A'+10;
    else ret|=t-'a'+10;
    return ret;
}
u16 MapParam(u8 var,u16 *table,u8 dir) //���,index=9
{
    ParamMapTableItem item;
    u8 idx=0,size,def;
    item.all=table[0];
    size=item.data.key;
    def=item.data.value;
    
    while(idx<size)
    {
        idx++;
        item.all=table[idx];
        
        if(dir==0)
        {
            if(item.data.key==var)return item.data.value;
        }
        else if(item.data.value==var)return item.data.key;
    }
    return def; //table��һ��key=���ݸ�����value=Ĭ��ֵ
}

//! @}
//! @}