
#include "rectangle_area_pro.h"


Qu_YU_JU QuYu_Juxing[Ju_Max];
uint8_t Juxing_QuYu_Cnt=0;
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_QuYu_Ju_Server 
 *  Description: 设置形区域服务函数
 *  Parameters :  
 *  Return     :  void *souce 从ID开始数据
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-11 			 
 * =====================================================================================
 */
void Tcp_QuYu_Ju_Server(uint8_t l_socket,uint8_t *Temp_data_buff,uint16_t Lens,uint16_t SN)
{
	uint8_t Cnt;
	if(*Temp_data_buff==0x00)
	{  //更新区域
	    #if rectangle_are_pro_printf
        eat_trace("seting_param id:0x0080-->收到更新矩形区域的信息.");
	    #endif
		if(*(Temp_data_buff+1) > Ju_Max)
		{
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Set_ID,1);
			return;
		}
		else
		{
			Juxing_QuYu_Cnt = *(Temp_data_buff+1);//区域个数  JU_Quyu
			Analyze_Juxing_QuYu(QuYu_Juxing,Temp_data_buff+2,*(Temp_data_buff+1),0);//按结构体储存当前区域
			file_write(Qu_Yu_Save_File_juxing,0,&Juxing_QuYu_Cnt,1);//写文件
			file_write(Qu_Yu_Save_File_juxing,1,QuYu_Juxing,Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]));//写文件
		}
	}
	else if(*Temp_data_buff==0x01)
	{ //追加区域
	    #if rectangle_are_pro_printf
        eat_trace("seting_param id:0x0080-->追加矩形区域");
	    #endif
		if((Juxing_QuYu_Cnt + *(Temp_data_buff+1))> Ju_Max)
		{
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Set_ID,1);
			return;
		}
		else
		{
			Analyze_Juxing_QuYu(QuYu_Juxing,Temp_data_buff+2,*(Temp_data_buff+1),Juxing_QuYu_Cnt);
			file_write(Qu_Yu_Save_File_juxing,1+Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]),QuYu_Juxing+Juxing_QuYu_Cnt,(*(Temp_data_buff+1))*sizeof(QuYu_Juxing[0]));//写文件
			//区域个数
			Juxing_QuYu_Cnt += *(Temp_data_buff+1);//区域个数
			file_write(Qu_Yu_Save_File_juxing,0,&Juxing_QuYu_Cnt,1);//写文件
		}
	}
	else if(*Temp_data_buff==0x02)
	{  //修改区域
	    #if rectangle_are_pro_printf
        eat_trace("seting_param id:0x0080-->修改矩形区域的信息");
	    #endif
		Analyze_Juxing_QuYu_XiuGai(Temp_data_buff+2,*(Temp_data_buff+1));//分析进行修改
	}
	Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Set_ID,0);
	#if 0
	for(Cnt=0;Cnt<Juxing_QuYu_Cnt;Cnt++)
	{
		eat_trace("矩形区域参数=ID:%u,属性:%0x,左上%d,左下%d,右上%d,右下%d",QuYu_Juxing[Cnt].Ju_ID,QuYu_Juxing[Cnt].Ju_Shuxing,QuYu_Juxing[Cnt].ZUO_Point_WeiDu,QuYu_Juxing[Cnt].ZUO_Point_JingDu,QuYu_Juxing[Cnt].YOU_Point_WeiDu,QuYu_Juxing[Cnt].YOU_Point_JingDu);
		eat_trace("区域限速:%u",QuYu_Juxing[Cnt].Max_Speed);
	}
	#endif
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

void Tcp_QuYu_Delet_Ju_Servn(uint8_t l_socket,u8 *Psouce,u16 SN)
{
	u8 Cnt,Cnt0;
	u8 Ofest=0;
	u32 Curen_ID=0;//删除的ID
	u8 Delet_QuYu_Cnt=0;
	u8 Same_State=EAT_FALSE;
	Qu_YU_JU Temp_Ju[10];
	if(Juxing_QuYu_Cnt!=0)
	{
		if(*Psouce==0)
		{
			Juxing_QuYu_Cnt=0;
			file_write(Qu_Yu_Save_File_juxing,0,&Juxing_QuYu_Cnt,1);//更新文件
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Delet_ID,0);//删除成功
            #if rectangle_are_pro_printf
            eat_trace("seting_param id:0x0080-->删除矩形区域成功");
            #endif
		}
		else
		{
			for(Cnt=0;Cnt<Juxing_QuYu_Cnt;Cnt++)
			{
				for(Cnt0=0;Cnt0<*Psouce;Cnt0++)
				{  
					Big_End_BytetoDword(Psouce+1+4*Cnt0,&Curen_ID);
					if(Curen_ID==QuYu_Juxing[Cnt].Ju_ID)
					{
						Same_State=EAT_TRUE;
						break;
					}
				}
				if(Same_State==EAT_TRUE)
				{
					Delet_QuYu_Cnt++;
					Same_State=EAT_FALSE;
				}
				else
				{
					memcpy(Temp_Ju+Ofest,QuYu_Juxing+Cnt,sizeof(QuYu_Juxing[0]));
					Ofest++;//当前地址
				}
			}		
			Juxing_QuYu_Cnt=Ofest;
			file_write(Qu_Yu_Save_File_juxing,0,&Juxing_QuYu_Cnt,1);//更新文件
		    if(Ofest!=0)
		    {
		    	memcpy(QuYu_Juxing,Temp_Ju,Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]));
				file_write(Qu_Yu_Save_File_juxing,1,QuYu_Juxing,Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]));//更新文件
		    }
			if(Delet_QuYu_Cnt==*Psouce)
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Delet_ID,0);//删除成功
                #if rectangle_are_pro_printf
                eat_trace("seting_param id:0x0080-->删除矩形区域成功");
                #endif
			}
			else
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Delet_ID,2);//删除不成功
                #if rectangle_are_pro_printf
                eat_trace("seting_param id:0x0080-->删除矩形区域失败 !");
                #endif
			}
		}
	}
	else
	{
		Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Delet_ID,2);//删除不成功
        #if rectangle_are_pro_printf
        eat_trace("seting_param id:0x0080-->删除矩形区域失败 !");
        #endif
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_YuanXing_QuYu
 *  Description:  下发的圆形区域解析为机构体 结构体数组中储存!!  区域个数为10
 *  Parameters :  *psouce //从ID开始的数据指针
 *  				Qu_Cnt 区域个数
 *					偏移量就是目前从数组前几项是用用的
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void Analyze_Juxing_QuYu(Qu_YU_JU *Dest_Struct,uint8_t *psouce,uint8_t Qu_Cnt,uint8_t Offest)
{
	uint8_t Cnt;
	uint16_t Date_Cnt=0;
	for(Cnt=0;Cnt<Qu_Cnt;Cnt++)
	{
		Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->Ju_ID));
		Date_Cnt +=4;
		
		Big_End_BytetoWord(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->Ju_Shuxing));
		Date_Cnt +=2;

		Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->ZUO_Point_WeiDu));
		Date_Cnt +=4;
		
		Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->ZUO_Point_JingDu));
		Date_Cnt +=4;
		
		Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->YOU_Point_WeiDu));
		Date_Cnt +=4;
		
		Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->YOU_Point_JingDu));
		Date_Cnt +=4;
		
		if((((Dest_Struct+Cnt+Offest)->Ju_Shuxing)>>0)&0x0001)
		{	
			memcpy((Dest_Struct+Cnt+Offest)->Time_On,psouce + Date_Cnt,6);// 拷贝数据
			Date_Cnt +=6;
			memcpy((Dest_Struct+Cnt+Offest)->Time_OFF,psouce + Date_Cnt,6);// 拷贝数据
			Date_Cnt +=6;
			
		}
		if((((Dest_Struct+Cnt+Offest)->Ju_Shuxing)>>1)&0x0001)
		{
		     Big_End_BytetoWord(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->Max_Speed));
			 Date_Cnt +=2;
			 (Dest_Struct+Cnt+Offest)->Time_O = *(psouce + Date_Cnt);//区域超时
			 Date_Cnt +=1;
		}
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_QuYu_Struct_Yuan
 *  Description: 将储存结构体变为数组写入文件
 *  Parameters :  Psource_Struct结构体指针
 *  			Pdest 目标数组
 *				OFest 数组长度
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void File_Analyze_QuYu_Struct_Ju(void)
{
    file_create(Qu_Yu_Save_File_juxing);
	file_read(Qu_Yu_Save_File_juxing,0,&Juxing_QuYu_Cnt,1);//读文件内信息
	file_read(Qu_Yu_Save_File_juxing,1,QuYu_Juxing,Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]));//读区域信息
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_YuanXing_QuYu_XiuGai
 *  Description: 处理修改信息
 *  Parameters :  Psource_Struct结构体指针
 *  			Pdest 目标数组
 *				OFest 数组长度
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-11			 
 * =====================================================================================
 */
void  Analyze_Juxing_QuYu_XiuGai(u8 *psouce,u8 Qu_Cnt)
{
	Qu_YU_JU Temp_Qu[10];//临时区域
	u8 Cnt,Cnt0;
	Analyze_Juxing_QuYu(Temp_Qu,psouce,Qu_Cnt,0);
	for(Cnt=0;Cnt<Qu_Cnt;Cnt++)
	{
		for(Cnt0=0;Cnt0< Juxing_QuYu_Cnt;Cnt0++)
		{
			if(Temp_Qu[Cnt].Ju_ID==QuYu_Juxing[Cnt0].Ju_ID)
			{
				memcpy(&QuYu_Juxing[Cnt0],&Temp_Qu[Cnt],sizeof(Temp_Qu[Cnt]));//复制改变结构体	
			}
		}
	}
	file_write(Qu_Yu_Save_File_juxing,1,QuYu_Juxing,Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]));//写修改后
}


