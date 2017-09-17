
#include "round_area_pro.h"

Qu_YU_yun QuYu_Yuan[Yuan_Max];
uint8_t Yuan_QuYu_Cnt=0;
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_QuYu_Yuan_Server TCP
 *  Description:  �����·�Բ�����������
 *  Parameters :  
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-11 			 
 * =====================================================================================
 */
void Tcp_QuYu_Yuan_Server(uint8_t l_socket,u8 *Temp_data_buff,u16 Lens,u16 SN)
{
	uint8_t Cnt;
	if(*Temp_data_buff==0x00)
	{  //��������
	    #if roud_area_pro_printf
        eat_trace("Tcp_QuYu_Yuan_Server --> ��������  !");
		#endif
		if(*(Temp_data_buff+1) > Yuan_Max)
		{
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Set_ID,1);//Y԰���óɹ�
		}
		else
		{
			Yuan_QuYu_Cnt = *(Temp_data_buff+1);//�������
			Analyze_YuanXing_QuYu(QuYu_Yuan,Temp_data_buff+2,*(Temp_data_buff+1),0);//���ṹ�崢�浱ǰ����
			file_write(Qu_Yu_Save_File_yuan,1,QuYu_Yuan,Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]));//д�ļ�
		}
	}
	else if(*Temp_data_buff==0x01)
	{ //׷������
		if((Yuan_QuYu_Cnt +*(Temp_data_buff+1)) > Yuan_Max)
		{
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Set_ID,0);//Y԰���óɹ�
			return;
		}
		else
		{
            #if roud_area_pro_printf
            eat_trace("Tcp_QuYu_Yuan_Server --> �յ�׷��Բ���������Ϣ  !");
            #endif
			Analyze_YuanXing_QuYu(QuYu_Yuan,Temp_data_buff+2,*(Temp_data_buff+1),Yuan_QuYu_Cnt);
			//�������
			file_write(Qu_Yu_Save_File_yuan,1+Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]),QuYu_Yuan+Yuan_QuYu_Cnt,(*(Temp_data_buff+1))*sizeof(QuYu_Yuan[0]));//д�ļ�
			Yuan_QuYu_Cnt += *(Temp_data_buff+1);//�������
		}
	}
	else if(*Temp_data_buff==0x02)
	{  
        #if roud_area_pro_printf
        eat_trace("Tcp_QuYu_Yuan_Server --> �յ��޸�Բ���������Ϣ  !");
        #endif
		Analyze_YuanXing_QuYu_XiuGai(Temp_data_buff+2,*(Temp_data_buff+1));//���������޸�
	}
	file_write(Qu_Yu_Save_File_yuan,0,&Yuan_QuYu_Cnt,1);//д�ļ�
	Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Set_ID,0);//Y԰���óɹ�
	#if 0
	for(Cnt=0;Cnt<Yuan_QuYu_Cnt;Cnt++)
	{
 		eat_trace("SSI_debug��Բ������ID%u,����%u,����%u,γ��%u,�뾶%u,�ٶ�%u,ʱ��%u��",QuYu_Yuan[Cnt].Yuan_ID,QuYu_Yuan[Cnt].Yuan_Shuxing,QuYu_Yuan[Cnt].Cemt_Point_WeiDu,QuYu_Yuan[Cnt].Cemt_Point_JingDu,QuYu_Yuan[Cnt].Banjing_R,QuYu_Yuan[Cnt].Max_Speed,QuYu_Yuan[Cnt].Time_O);
	}
	#endif
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_QuYu_Delet_Yuan_Servn 
 *  Description: ɾ��Բ�����������
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
	u32 Curen_ID=0;//ɾ����ID
	u8 Same_State=EAT_FALSE;
	Qu_YU_yun Temp_Yun[10];
	if(Yuan_QuYu_Cnt!=0)
	{
		if(*Psouce==0)
		{
			Yuan_QuYu_Cnt=0;
			file_write(Qu_Yu_Save_File_yuan,0,&Ofest,1);//�����ļ�
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Delet_ID,0);//ɾ���ɹ�
            #if roud_area_pro_printf
            eat_trace("Tcp_QuYu_Delet_Yuan_Servn --> Բ������ɾ���ɹ�  !");
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
					Ofest++;//��ǰ��ַ
				}
			}
			Yuan_QuYu_Cnt=Ofest;
			file_write(Qu_Yu_Save_File_yuan,0,&Ofest,1);//�����ļ�
			if(Ofest!=0)
			{
				memcpy(QuYu_Yuan,Temp_Yun,Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]));
				file_write(Qu_Yu_Save_File_yuan,1,QuYu_Yuan,Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]));//�����ļ�
			}
			if(Delet_Yuan_Cnt==*Psouce)
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Delet_ID,0);//ɾ�����ɹ�
				
                #if roud_area_pro_printf
                eat_trace("Tcp_QuYu_Delet_Yuan_Servn --> Բ������ɾ���ɹ�  !");
                #endif
			}
			else
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Delet_ID,2);//ɾ���ɹ�
                #if roud_area_pro_printf
                eat_trace("Tcp_QuYu_Delet_Yuan_Servn --> Բ������ɾ��ʧ��  !");
                #endif
			}
		}
	}
	else
	{
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Yuan_Delet_ID,2);//ɾ�����ɹ�
            #if roud_area_pro_printf
            eat_trace("Tcp_QuYu_Delet_Yuan_Servn --> Բ������ɾ��ʧ��  !");
            #endif
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_YuanXing_QuYu
 *  Description:  �·���Բ���������Ϊ������ �ṹ�������д���!!  �������Ϊ10
 *  Parameters :  *psouce //��ID��ʼ������ָ��
 *  				Qu_Cnt �������
 *					ƫ��������Ŀǰ������ǰ���������õ�
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
			memcpy((Dest_Struct+Cnt+Offest)->Time_On,psouce + Date_Cnt,6);// ��������
			Date_Cnt +=6;
			memcpy((Dest_Struct+Cnt+Offest)->Time_OFF,psouce + Date_Cnt,6);// ��������
			Date_Cnt +=6;			
		}
		if((((Dest_Struct+Cnt+Offest)->Yuan_Shuxing)>>1)&0x0001)
		{
			Big_End_BytetoWord(psouce + Date_Cnt,&((Dest_Struct+Cnt+Offest)->Max_Speed));
			Date_Cnt +=2;			
			(Dest_Struct+Cnt+Offest)->Time_O = *(psouce + Date_Cnt);//����ʱ
			Date_Cnt +=1;			
		}
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_YuanXing_QuYu_XiuGai
 *  Description: �����޸���Ϣ
 *  Parameters :  Psource_Struct�ṹ��ָ��
 *  			Pdest Ŀ������
 *				OFest ���鳤��
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-11			 
 * =====================================================================================
 */
void  Analyze_YuanXing_QuYu_XiuGai(u8 *psouce,u8 Qu_Cnt)
{
	Qu_YU_yun Temp_Qu[10];//��ʱ����
	u8 Cnt,Cnt0;
	Analyze_YuanXing_QuYu(Temp_Qu,psouce,Qu_Cnt,0);
	for(Cnt=0;Cnt<Qu_Cnt;Cnt++)
	{
		for(Cnt0=0;Cnt0< Yuan_QuYu_Cnt;Cnt0++)
		{
			if(Temp_Qu[Cnt].Yuan_ID==QuYu_Yuan[Cnt0].Yuan_ID)
			{
				memcpy(&QuYu_Yuan[Cnt0],&Temp_Qu[Cnt],sizeof(Temp_Qu[Cnt]));//���Ƹı�ṹ��	
			}
		}
	}
	file_write(Qu_Yu_Save_File_yuan,1,QuYu_Yuan,Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]));//д�޸ĺ�
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Comimt_QuYu_File
 *  Description: ����·�ߵ��ļ�ϵͳ
 *  Parameters :  Psource_Struct�ṹ��ָ��
 *  			Pdest Ŀ������
 *				OFest ���鳤��
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-11			 
 * =====================================================================================
 */
void Yuan_Comimt_QuYu_File(void)
{
    file_create(Qu_Yu_Save_File_yuan);
	file_read(Qu_Yu_Save_File_yuan,0,&Yuan_QuYu_Cnt,1);
	file_read(Qu_Yu_Save_File_yuan,1,QuYu_Yuan,Yuan_QuYu_Cnt*sizeof(QuYu_Yuan[0]));//��������Ϣ
}

