
#include "driver_ic_card_update.h"

driver_ic_card_data ic_card_data;
uint8_t IC_temp[8]={0x01,0x14,0x11,0x25,0x07,0x40,0x30,0x00};//前8字节
uint8_t Driver_Name[8]="南斗";
uint8_t Ecode[20]="500382197611243839";
uint8_t FaZhengJiGou[32]="武汉南斗六星";
uint8_t Effect_Data[4]={0x20,0x17,0x09,0x27};
const u8 YunDan[]="SSI_BDS_1D02_xh12345";
void driver_ic_card_data_report(uint8_t *buffer,uint8_t Dection)
{
    uint16_t msg_svn=0;
	ic_card_data.IC_Data_buf[0]=Dection;
	memcpy(ic_card_data.IC_Data_buf+1,Real_Time_Location_Inf.Curren_Time,6);
	if(Dection==0x01)
	{
		ic_card_data.IC_Data_buf[7]=0;//姓名长度
		ic_card_data.IC_Data_buf[8]=4;//姓名长度
		memcpy(ic_card_data.IC_Data_buf+9,Driver_Name,4);//驾驶员姓名
		memcpy(ic_card_data.IC_Data_buf+13,buffer,20);//从业资格证编号
		memcpy(Ecode,buffer,18);
		ic_card_data.IC_Data_buf[33]=(uint8_t)strlen(FaZhengJiGou);//发证机构个名称
		memcpy(ic_card_data.IC_Data_buf+34,FaZhengJiGou,(uint8_t)strlen(FaZhengJiGou));//发证机构复制
		memcpy(ic_card_data.IC_Data_buf+34+(uint8_t)strlen(FaZhengJiGou),Effect_Data,4);//复制时间
		ic_card_data.Lens =38+(uint8_t)strlen(FaZhengJiGou);//数据总长度	
	}
	if(Dection==0x02)
	{
		ic_card_data.Lens=7;
	}
	if(System_Flag.System_Flag_Bit.SecondAutenSucess==EAT_TRUE)
	{
        Tcp_Message_PacKage_And_Send(Second_Link.mode_Socket,&msg_svn,tcp_ic_replay_id,ic_card_data.IC_Data_buf,ic_card_data.Lens,EAT_FALSE,0,0);
    }
    if(System_Flag.System_Flag_Bit.MainAutenSucess==EAT_TRUE)
    {
        Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msg_svn,tcp_ic_replay_id,ic_card_data.IC_Data_buf,ic_card_data.Lens,EAT_FALSE,0,0);
    }
}
void tcp_driver_data_ask_proc(uint8_t socket)
{
   uint16_t msg_svn=0;
   Tcp_Message_PacKage_And_Send(socket,&msg_svn,tcp_ic_replay_id,ic_card_data.IC_Data_buf,ic_card_data.Lens,EAT_FALSE,0,0);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: TCP_DianZi_YunDan_Update
 *  Description:  电子运单
 *  Parameters :  void     
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
void TCP_DianZi_YunDan_Update(void)
{
	u8 YuDan[64]={0};
	u16 Lens;
	Lens=strlen(YunDan);
	Big_End_DwordtoByte(Lens,YuDan+1);
	memcpy(YuDan+4,YunDan,Lens);
}


