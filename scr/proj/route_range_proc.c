
#include "route_range_proc.h"
Lu_Xian_Road LuXian_Date[LuXian_Max];

u8 Luxian_QuYu_Cnt=0;
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_QuYu_Duo_Server
 *  Description: 设置多边形区域服务函数
 *  Parameters :  Psource_Struct结构体指针
 *  			Pdest 目标数组
 *				OFest 数组长度
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-11			 
 * =====================================================================================
 */
void Tcp_LuXian_Server(uint8_t l_socket,u8 *Temp_data_buff,u16 Lens,u16 SN)
{
	u8 LuXian_Cnt;
	Lu_Xian_Road Temp_LuXian;
	if((Luxian_QuYu_Cnt+1) > LuXian_Max)
	{
		Tcp_VDR_Common_Replay(l_socket,SN,Tcp_LuXian_Set_ID,1);//设置成功
	}
	else
	{
		memset(&Temp_LuXian,0,sizeof(Temp_LuXian));
		Analyze_LuXian(&Temp_LuXian,Temp_data_buff);
		for(LuXian_Cnt=0;LuXian_Cnt<Luxian_QuYu_Cnt;LuXian_Cnt++)
		{
			if(LuXian_Date[LuXian_Cnt].LuXian_ID == Temp_LuXian.LuXian_ID)
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_LuXian_Set_ID,1);//ID重复设置错误
				return;
			}
		}
		memcpy(&LuXian_Date[Luxian_QuYu_Cnt],&Temp_LuXian,sizeof(Temp_LuXian));
		file_write(Qu_Yu_Save_File_Luxian,1+Luxian_QuYu_Cnt*sizeof(LuXian_Date[0]),&LuXian_Date[Luxian_QuYu_Cnt],sizeof(LuXian_Date[0]));//写修改后
		Luxian_QuYu_Cnt += 1;//区域个增加
		file_write(Qu_Yu_Save_File_Luxian,0,&Luxian_QuYu_Cnt,1);//写修改后
		Tcp_VDR_Common_Replay(l_socket,SN,Tcp_LuXian_Set_ID,0);//设置成功
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_QuYu_Delet_Ju_Servn 
 *  Description: 删除矩形形区域服务函数
 *  Parameters :  
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-11 			 
 * =====================================================================================
 */

void  Tcp_Delet_LuXian_Servn(uint8_t l_socket,u8 *Psouce,u16 SN)
{
	u8 Cnt,Cnt0;
	u32 Curen_ID=0;//删除的ID
	u8 Ofest=0;
	u8 Delet_Cnt=0;
	u8 Same_State=EAT_FALSE;
	Lu_Xian_Road Temp_Road[10];
	if(Luxian_QuYu_Cnt!=0)
	{
		if(*Psouce==0)
		{
			Luxian_QuYu_Cnt=0;
			file_write(Qu_Yu_Save_File_Luxian,0,&Luxian_QuYu_Cnt,1);//更新文件
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_LuXian_Delet_ID,0);//删除成功	
		}
		else
		{			
			for(Cnt=0;Cnt<Luxian_QuYu_Cnt;Cnt++)
			{
				for(Cnt0=0;Cnt0<*Psouce;Cnt0++)
				{  
					Big_End_BytetoDword(Psouce+1+4*Cnt0,&Curen_ID);
					if(Curen_ID==(LuXian_Date[Cnt].LuXian_ID))
					{
						Same_State=EAT_TRUE;
						break;
					}
				}
				if(Same_State==EAT_TRUE)
				{
					Delet_Cnt++;
					Same_State=EAT_FALSE;
				}
				else
				{
					memcpy(Temp_Road+Ofest,LuXian_Date+Cnt,sizeof(LuXian_Date[0]));
					Ofest++;//当前地址
				}
			}
			Luxian_QuYu_Cnt =Ofest;
			file_write(Qu_Yu_Save_File_Luxian,0,&Ofest,1);//更新文件
			if(Ofest!=0)
			{
				memcpy(LuXian_Date,Temp_Road,sizeof(LuXian_Date));
				file_write(Qu_Yu_Save_File_Luxian,1,LuXian_Date,sizeof(LuXian_Date));//更新文件
			}
			if(Delet_Cnt==*Psouce)
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_LuXian_Delet_ID,0);//删除成功	
			}
			else
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_LuXian_Delet_ID,2);//删除成功	
			}
		}
	}
	else
	{
		Tcp_VDR_Common_Replay(l_socket,SN,Tcp_LuXian_Delet_ID,2);//删除成功	
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_LuXian
 *  Description:  下发多边行区域 最多10个
 *  Parameters :  *psouce //从ID开始的数据指针
 *  				Qu_Cnt 区域个数
 *					Offest偏移量就是目前从数组前几项是用用的
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void Analyze_LuXian(Lu_Xian_Road *Dest_Struct,u8 *psouce)
{
	u16 Data_OFest=0;
	Big_End_BytetoDword(psouce+Data_OFest,&(Dest_Struct->LuXian_ID));
	Data_OFest +=4;
	
	Big_End_BytetoWord(psouce+Data_OFest,&(Dest_Struct->LuXian_Shuxing));
	Data_OFest +=2;
	if(((Dest_Struct->LuXian_Shuxing)>>0)&0x0001)
	{	
		memcpy(Dest_Struct->Time_On,psouce+Data_OFest,6);// 拷贝数据
		Data_OFest +=6;
		memcpy(Dest_Struct->Time_OFF,psouce+Data_OFest,6);// 拷贝数据
		Data_OFest +=6;
	}
	Big_End_BytetoWord(psouce+Data_OFest,&(Dest_Struct->Guai_Cnt));
	Data_OFest +=2;
	if((Dest_Struct->Guai_Cnt) > Guai_MAX)
	{
		(Dest_Struct->Guai_Cnt) = Guai_MAX;
	}
	Analyze_LuXian_GuaiDian(Dest_Struct->Guaidian,psouce+Data_OFest,Dest_Struct->Guai_Cnt);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_LuXian_GuaiDian
 *  Description:  解析生产拐点结构体体
 *  Parameters :  产生拐点数据
 *  				Qu_Cnt 区域个数
 *             	*Length //拐点数据长度
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void Analyze_LuXian_GuaiDian(Guai_Dian *Dest_Struct,u8 *psouce,u8 Qu_Cnt)
{
	u8 Cnt0=0;
	u16 Date_Cnt=0;
	for(Cnt0=0;Cnt0<Qu_Cnt;Cnt0++)
	{
		Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt0)->GuaiDian_ID));//拐点ID
		Date_Cnt +=4;
		
		Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt0)->LuDuan_ID));
		Date_Cnt +=4;
		
		Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt0)->GUAI_Point_WeiDu));
		Date_Cnt +=4;
		
        Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt0)->GUAI_Point_JingDu));
		Date_Cnt +=4;
		
		(Dest_Struct+Cnt0)->Lu_Kuan = *(psouce+Date_Cnt);//路宽
		Date_Cnt +=1;
		
		(Dest_Struct+Cnt0)->GUAI_Shuxing = *(psouce + Date_Cnt);//属性
		Date_Cnt +=1;
		if((((Dest_Struct+Cnt0)->GUAI_Shuxing)>>0)&0x01)
		{
			Big_End_BytetoWord(psouce + Date_Cnt,&((Dest_Struct+Cnt0)->Max_Time));
			Date_Cnt +=2;
			Big_End_BytetoWord(psouce + Date_Cnt,&((Dest_Struct+Cnt0)->Time_Buzu));
			Date_Cnt +=2;
			
		}
		if((((Dest_Struct+Cnt0)->GUAI_Shuxing)>>1)&0x01)
		{
			Big_End_BytetoWord(psouce + Date_Cnt,&((Dest_Struct+Cnt0)->Max_Speed));
			Date_Cnt +=2;
			(Dest_Struct+Cnt0)->ChaoSu_Time = *(psouce + Date_Cnt);
			Date_Cnt +=1;
		}
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: u8 PanDuan_Ju_QuYu(u8 *Pouce)
 *  Description:  矩形区域判断
 *  Parameters :  
 *  Return     :  u8增加 
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-10 			 
 * =====================================================================================
 */
void File_Luxian_Iint(void)
{
    file_create(Qu_Yu_Save_File_Luxian);
	file_read(Qu_Yu_Save_File_Luxian,0,&Luxian_QuYu_Cnt,1);//更新文件
	file_read(Qu_Yu_Save_File_Luxian,1,LuXian_Date,sizeof(LuXian_Date));//更新文件
}

