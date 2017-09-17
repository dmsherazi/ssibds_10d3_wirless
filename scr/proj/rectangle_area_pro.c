
#include "rectangle_area_pro.h"


Qu_YU_JU QuYu_Juxing[Ju_Max];
uint8_t Juxing_QuYu_Cnt=0;
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_QuYu_Ju_Server 
 *  Description: ���������������
 *  Parameters :  
 *  Return     :  void *souce ��ID��ʼ����
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-11 			 
 * =====================================================================================
 */
void Tcp_QuYu_Ju_Server(uint8_t l_socket,uint8_t *Temp_data_buff,uint16_t Lens,uint16_t SN)
{
	uint8_t Cnt;
	if(*Temp_data_buff==0x00)
	{  //��������
	    #if rectangle_are_pro_printf
        eat_trace("seting_param id:0x0080-->�յ����¾����������Ϣ.");
	    #endif
		if(*(Temp_data_buff+1) > Ju_Max)
		{
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Set_ID,1);
			return;
		}
		else
		{
			Juxing_QuYu_Cnt = *(Temp_data_buff+1);//�������  JU_Quyu
			Analyze_Juxing_QuYu(QuYu_Juxing,Temp_data_buff+2,*(Temp_data_buff+1),0);//���ṹ�崢�浱ǰ����
			file_write(Qu_Yu_Save_File_juxing,0,&Juxing_QuYu_Cnt,1);//д�ļ�
			file_write(Qu_Yu_Save_File_juxing,1,QuYu_Juxing,Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]));//д�ļ�
		}
	}
	else if(*Temp_data_buff==0x01)
	{ //׷������
	    #if rectangle_are_pro_printf
        eat_trace("seting_param id:0x0080-->׷�Ӿ�������");
	    #endif
		if((Juxing_QuYu_Cnt + *(Temp_data_buff+1))> Ju_Max)
		{
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Set_ID,1);
			return;
		}
		else
		{
			Analyze_Juxing_QuYu(QuYu_Juxing,Temp_data_buff+2,*(Temp_data_buff+1),Juxing_QuYu_Cnt);
			file_write(Qu_Yu_Save_File_juxing,1+Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]),QuYu_Juxing+Juxing_QuYu_Cnt,(*(Temp_data_buff+1))*sizeof(QuYu_Juxing[0]));//д�ļ�
			//�������
			Juxing_QuYu_Cnt += *(Temp_data_buff+1);//�������
			file_write(Qu_Yu_Save_File_juxing,0,&Juxing_QuYu_Cnt,1);//д�ļ�
		}
	}
	else if(*Temp_data_buff==0x02)
	{  //�޸�����
	    #if rectangle_are_pro_printf
        eat_trace("seting_param id:0x0080-->�޸ľ����������Ϣ");
	    #endif
		Analyze_Juxing_QuYu_XiuGai(Temp_data_buff+2,*(Temp_data_buff+1));//���������޸�
	}
	Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Set_ID,0);
	#if 0
	for(Cnt=0;Cnt<Juxing_QuYu_Cnt;Cnt++)
	{
		eat_trace("�����������=ID:%u,����:%0x,����%d,����%d,����%d,����%d",QuYu_Juxing[Cnt].Ju_ID,QuYu_Juxing[Cnt].Ju_Shuxing,QuYu_Juxing[Cnt].ZUO_Point_WeiDu,QuYu_Juxing[Cnt].ZUO_Point_JingDu,QuYu_Juxing[Cnt].YOU_Point_WeiDu,QuYu_Juxing[Cnt].YOU_Point_JingDu);
		eat_trace("��������:%u",QuYu_Juxing[Cnt].Max_Speed);
	}
	#endif
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_QuYu_Delet_Ju_Servn 
 *  Description: ɾ�����������������
 *  Parameters :  
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-11 			 
 * =====================================================================================
 */

void Tcp_QuYu_Delet_Ju_Servn(uint8_t l_socket,u8 *Psouce,u16 SN)
{
	u8 Cnt,Cnt0;
	u8 Ofest=0;
	u32 Curen_ID=0;//ɾ����ID
	u8 Delet_QuYu_Cnt=0;
	u8 Same_State=EAT_FALSE;
	Qu_YU_JU Temp_Ju[10];
	if(Juxing_QuYu_Cnt!=0)
	{
		if(*Psouce==0)
		{
			Juxing_QuYu_Cnt=0;
			file_write(Qu_Yu_Save_File_juxing,0,&Juxing_QuYu_Cnt,1);//�����ļ�
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Delet_ID,0);//ɾ���ɹ�
            #if rectangle_are_pro_printf
            eat_trace("seting_param id:0x0080-->ɾ����������ɹ�");
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
					Ofest++;//��ǰ��ַ
				}
			}		
			Juxing_QuYu_Cnt=Ofest;
			file_write(Qu_Yu_Save_File_juxing,0,&Juxing_QuYu_Cnt,1);//�����ļ�
		    if(Ofest!=0)
		    {
		    	memcpy(QuYu_Juxing,Temp_Ju,Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]));
				file_write(Qu_Yu_Save_File_juxing,1,QuYu_Juxing,Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]));//�����ļ�
		    }
			if(Delet_QuYu_Cnt==*Psouce)
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Delet_ID,0);//ɾ���ɹ�
                #if rectangle_are_pro_printf
                eat_trace("seting_param id:0x0080-->ɾ����������ɹ�");
                #endif
			}
			else
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Delet_ID,2);//ɾ�����ɹ�
                #if rectangle_are_pro_printf
                eat_trace("seting_param id:0x0080-->ɾ����������ʧ�� !");
                #endif
			}
		}
	}
	else
	{
		Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_JuXing_Delet_ID,2);//ɾ�����ɹ�
        #if rectangle_are_pro_printf
        eat_trace("seting_param id:0x0080-->ɾ����������ʧ�� !");
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
			memcpy((Dest_Struct+Cnt+Offest)->Time_On,psouce + Date_Cnt,6);// ��������
			Date_Cnt +=6;
			memcpy((Dest_Struct+Cnt+Offest)->Time_OFF,psouce + Date_Cnt,6);// ��������
			Date_Cnt +=6;
			
		}
		if((((Dest_Struct+Cnt+Offest)->Ju_Shuxing)>>1)&0x0001)
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
 *  Name: Analyze_QuYu_Struct_Yuan
 *  Description: ������ṹ���Ϊ����д���ļ�
 *  Parameters :  Psource_Struct�ṹ��ָ��
 *  			Pdest Ŀ������
 *				OFest ���鳤��
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void File_Analyze_QuYu_Struct_Ju(void)
{
    file_create(Qu_Yu_Save_File_juxing);
	file_read(Qu_Yu_Save_File_juxing,0,&Juxing_QuYu_Cnt,1);//���ļ�����Ϣ
	file_read(Qu_Yu_Save_File_juxing,1,QuYu_Juxing,Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]));//��������Ϣ
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
void  Analyze_Juxing_QuYu_XiuGai(u8 *psouce,u8 Qu_Cnt)
{
	Qu_YU_JU Temp_Qu[10];//��ʱ����
	u8 Cnt,Cnt0;
	Analyze_Juxing_QuYu(Temp_Qu,psouce,Qu_Cnt,0);
	for(Cnt=0;Cnt<Qu_Cnt;Cnt++)
	{
		for(Cnt0=0;Cnt0< Juxing_QuYu_Cnt;Cnt0++)
		{
			if(Temp_Qu[Cnt].Ju_ID==QuYu_Juxing[Cnt0].Ju_ID)
			{
				memcpy(&QuYu_Juxing[Cnt0],&Temp_Qu[Cnt],sizeof(Temp_Qu[Cnt]));//���Ƹı�ṹ��	
			}
		}
	}
	file_write(Qu_Yu_Save_File_juxing,1,QuYu_Juxing,Juxing_QuYu_Cnt*sizeof(QuYu_Juxing[0]));//д�޸ĺ�
}


