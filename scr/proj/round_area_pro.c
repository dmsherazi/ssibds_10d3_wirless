
#include "round_area_pro.h"

Qu_YU_yun QuYu_Yuan[Yuan_Max];
uint8_t Yuan_QuYu_Cnt=0;
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_QuYu_Yuan_Server TCP
 *  Description:  区域下发圆形区域服务函数
 *  Parameters :  
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-11 			 
 * =====================================================================================
 */
void Tcp_QuYu_Yuan_Server(uint8_t l_socket,u8 *Temp_data_buff,u16 Lens,u16 SN)
{
	uint8_t Cnt;
	if(*Temp_data_buff==0x00)
	{  //更新区域
	    #if roud_area_pro_printf
        eat_trace("Tcp_QuYu_Yuan_Server --> 更新区域  !");
		#endif
		if(*(Temp_data_buff+1) > Yuan_Max)
		{
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Set_ID,1);//Y园设置成功
		}
		else
		{
			Yuan_QuYu_Cnt = *(Temp_data_buff+1);//区域个数
			Analyze_YuanXing_QuYu(QuYu_Yuan,Temp_data_buff+2,*(Temp_data_buff+1),0);//按结构体储存当前区域
			file_write(Qu_Yu_Save_File_yuan,1,QuYu_Yuan,Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]));//写文件
		}
	}
	else if(*Temp_data_buff==0x01)
	{ //追加区域
		if((Yuan_QuYu_Cnt +*(Temp_data_buff+1)) > Yuan_Max)
		{
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Set_ID,0);//Y园设置成功
			return;
		}
		else
		{
            #if roud_area_pro_printf
            eat_trace("Tcp_QuYu_Yuan_Server --> 收到追加圆形区域的信息  !");
            #endif
			Analyze_YuanXing_QuYu(QuYu_Yuan,Temp_data_buff+2,*(Temp_data_buff+1),Yuan_QuYu_Cnt);
			//区域个数
			file_write(Qu_Yu_Save_File_yuan,1+Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]),QuYu_Yuan+Yuan_QuYu_Cnt,(*(Temp_data_buff+1))*sizeof(QuYu_Yuan[0]));//写文件
			Yuan_QuYu_Cnt += *(Temp_data_buff+1);//区域个数
		}
	}
	else if(*Temp_data_buff==0x02)
	{  
        #if roud_area_pro_printf
        eat_trace("Tcp_QuYu_Yuan_Server --> 收到修改圆形区域的信息  !");
        #endif
		Analyze_YuanXing_QuYu_XiuGai(Temp_data_buff+2,*(Temp_data_buff+1));//分析进行修改
	}
	file_write(Qu_Yu_Save_File_yuan,0,&Yuan_QuYu_Cnt,1);//写文件
	Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Set_ID,0);//Y园设置成功
	#if 0
	for(Cnt=0;Cnt<Yuan_QuYu_Cnt;Cnt++)
	{
 		eat_trace("SSI_debug【圆形区域ID%u,属性%u,经度%u,纬度%u,半径%u,速度%u,时间%u】",QuYu_Yuan[Cnt].Yuan_ID,QuYu_Yuan[Cnt].Yuan_Shuxing,QuYu_Yuan[Cnt].Cemt_Point_WeiDu,QuYu_Yuan[Cnt].Cemt_Point_JingDu,QuYu_Yuan[Cnt].Banjing_R,QuYu_Yuan[Cnt].Max_Speed,QuYu_Yuan[Cnt].Time_O);
	}
	#endif
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_QuYu_Delet_Yuan_Servn 
 *  Description: 删除圆形区域服务函数
 *  Parameters :  
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-11 			 
 * =====================================================================================
 */

void Tcp_QuYu_Delet_Yuan_Servn(uint8_t l_socket,u8 *Psouce,u16 SN)
{
	u8 Cnt,Cnt0;
	u8 Ofest=0;
	u8 Delet_Yuan_Cnt=0;
	u32 Curen_ID=0;//删除的ID
	u8 Same_State=EAT_FALSE;
	Qu_YU_yun Temp_Yun[10];
	if(Yuan_QuYu_Cnt!=0)
	{
		if(*Psouce==0)
		{
			Yuan_QuYu_Cnt=0;
			file_write(Qu_Yu_Save_File_yuan,0,&Ofest,1);//更新文件
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Delet_ID,0);//删除成功
            #if roud_area_pro_printf
            eat_trace("Tcp_QuYu_Delet_Yuan_Servn --> 圆形区域删除成功  !");
            #endif

		}
		else
		{
			for(Cnt=0;Cnt<Yuan_QuYu_Cnt;Cnt++)
			{
				for(Cnt0=0;Cnt0<*Psouce;Cnt0++)
				{  
				    Big_End_BytetoDword(Psouce+1+4*Cnt0,&Curen_ID);
					if(Curen_ID==QuYu_Yuan[Cnt].Yuan_ID)
					{
						Same_State=EAT_TRUE;
						break;
					}
				}
				if(Same_State==EAT_TRUE)
				{
					Delet_Yuan_Cnt++;
					Same_State=EAT_FALSE;
				}
				else
				{
					memcpy(Temp_Yun+Ofest,QuYu_Yuan+Cnt,sizeof(QuYu_Yuan[0]));
					Ofest++;//当前地址
				}
			}
			Yuan_QuYu_Cnt=Ofest;
			file_write(Qu_Yu_Save_File_yuan,0,&Ofest,1);//更新文件
			if(Ofest!=0)
			{
				memcpy(QuYu_Yuan,Temp_Yun,Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]));
				file_write(Qu_Yu_Save_File_yuan,1,QuYu_Yuan,Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]));//更新文件
			}
			if(Delet_Yuan_Cnt==*Psouce)
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Delet_ID,0);//删除不成功
				
                #if roud_area_pro_printf
                eat_trace("Tcp_QuYu_Delet_Yuan_Servn --> 圆形区域删除成功  !");
                #endif
			}
			else
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Delet_ID,2);//删除成功
                #if roud_area_pro_printf
                eat_trace("Tcp_QuYu_Delet_Yuan_Servn --> 圆形区域删除失败  !");
                #endif
			}
		}
	}
	else
	{
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Delet_ID,2);//删除不成功
            #if roud_area_pro_printf
            eat_trace("Tcp_QuYu_Delet_Yuan_Servn --> 圆形区域删除失败  !");
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
void Analyze_YuanXing_QuYu(Qu_YU_yun *Dest_Struct,u8 *psouce,u8 Qu_Cnt,u8 Offest)
{
	u8 Cnt;
	u16 Date_Cnt=0;
	for(Cnt=0;Cnt<Qu_Cnt;Cnt++)
	{
	    Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->Yuan_ID));
		Date_Cnt +=4;
		
	    Big_End_BytetoWord(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->Yuan_Shuxing));
		Date_Cnt +=2;
		
	    Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->Cemt_Point_WeiDu));
		Date_Cnt +=4;

		
	    Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->Cemt_Point_JingDu));
		Date_Cnt +=4;
		
	    Big_End_BytetoDword(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->Banjing_R));
		Date_Cnt +=4;
		
		if((((Dest_Struct+Cnt+Offest)->Yuan_Shuxing)>>0)&0x0001)
		{	
			memcpy((Dest_Struct+Cnt+Offest)->Time_On,psouce + Date_Cnt,6);// 拷贝数据
			Date_Cnt +=6;
			memcpy((Dest_Struct+Cnt+Offest)->Time_OFF,psouce + Date_Cnt,6);// 拷贝数据
			Date_Cnt +=6;			
		}
		if((((Dest_Struct+Cnt+Offest)->Yuan_Shuxing)>>1)&0x0001)
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
 *  Name: Analyze_YuanXing_QuYu_XiuGai
 *  Description: 处理修改信息
 *  Parameters :  Psource_Struct结构体指针
 *  			Pdest 目标数组
 *				OFest 数组长度
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-11			 
 * =====================================================================================
 */
void  Analyze_YuanXing_QuYu_XiuGai(u8 *psouce,u8 Qu_Cnt)
{
	Qu_YU_yun Temp_Qu[10];//临时区域
	u8 Cnt,Cnt0;
	Analyze_YuanXing_QuYu(Temp_Qu,psouce,Qu_Cnt,0);
	for(Cnt=0;Cnt<Qu_Cnt;Cnt++)
	{
		for(Cnt0=0;Cnt0< Yuan_QuYu_Cnt;Cnt0++)
		{
			if(Temp_Qu[Cnt].Yuan_ID==QuYu_Yuan[Cnt0].Yuan_ID)
			{
				memcpy(&QuYu_Yuan[Cnt0],&Temp_Qu[Cnt],sizeof(Temp_Qu[Cnt]));//复制改变结构体	
			}
		}
	}
	file_write(Qu_Yu_Save_File_yuan,1,QuYu_Yuan,Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]));//写修改后
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Comimt_QuYu_File
 *  Description: 区域路线的文件系统
 *  Parameters :  Psource_Struct结构体指针
 *  			Pdest 目标数组
 *				OFest 数组长度
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-11			 
 * =====================================================================================
 */
void Yuan_Comimt_QuYu_File(void)
{
    file_create(Qu_Yu_Save_File_yuan);
	file_read(Qu_Yu_Save_File_yuan,0,&Yuan_QuYu_Cnt,1);
	file_read(Qu_Yu_Save_File_yuan,1,QuYu_Yuan,Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]));//读区域信息
}

