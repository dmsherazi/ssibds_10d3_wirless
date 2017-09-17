

#include "polygon_area_proc.h"


Qu_YU_Duo QuYu_Duo[Duo_Max];

uint8_t Duo_QuYu_Cnt=0;
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_QuYu_Duo_Server
 *  Description: ���ö�������������
 *  Parameters :  Psource_Struct�ṹ��ָ��
 *  			Pdest Ŀ������
 *				OFest ���鳤��
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-11			 
 * =====================================================================================
 */
void Tcp_QuYu_Duo_Server(uint8_t l_socket,u8 *Temp_data_buff,u16 Lens,u16 SN)
{
	u8 Temp_Cnt;
	static Qu_YU_Duo Duo_Temp;
    #if polygon_area_proc_printf 
    eat_trace("Tcp_QuYu_Duo_Server --> receive �����������Ϣ.");
    #endif
	if((Duo_QuYu_Cnt +1)> Duo_Max)
	{
		Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Duobian_Set_ID,1);
	}
	else
	{
		Analyze_Duobian_QuYu(&Duo_Temp,Temp_data_buff);
		for(Temp_Cnt = 0;Temp_Cnt<Duo_QuYu_Cnt;Temp_Cnt++)
		{
			if(Duo_Temp.Duo_ID == QuYu_Duo[Temp_Cnt].Duo_ID)
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Duobian_Set_ID,1);
				return;
			}
		}
		memcpy(&QuYu_Duo[Duo_QuYu_Cnt],&Duo_Temp,sizeof(Duo_Temp));
		file_write(Qu_Yu_Save_File_duobian,1+Duo_QuYu_Cnt*sizeof(QuYu_Duo[0]),&QuYu_Duo[Duo_QuYu_Cnt],sizeof(QuYu_Duo[0]));//д�޸ĺ�
		Duo_QuYu_Cnt+=1;
		file_write(Qu_Yu_Save_File_duobian,0,&Duo_QuYu_Cnt,1);//д�޸ĺ�
		Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Duobian_Set_ID,0);
	}
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

void  Tcp_QuYu_Delet_Duo_Servn(uint8_t l_socket,u8 *Psouce,u16 SN)
{
	uint8_t Cnt,Cnt0;
	uint8_t Delet_Duo_Cnt=0;
	uint32_t  Curen_ID=0;//ɾ����ID
	uint8_t Ofest=0;//ʣ��ĳ���
	uint8_t Same_State=EAT_FALSE;
	Qu_YU_Duo Temp_Duo[10];
	if(Duo_QuYu_Cnt!=0)
	{
		if(*Psouce==0)
		{
			Duo_QuYu_Cnt=0;
			file_write(Qu_Yu_Save_File_duobian,0,&Duo_QuYu_Cnt,1);//�����ļ�
			Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Duobian_Delet_ID,0);//ɾ���ɹ�	
		}
		else
		{
			for(Cnt=0;Cnt<Duo_QuYu_Cnt;Cnt++)
			{
				for(Cnt0=0;Cnt0<*Psouce;Cnt0++)
				{  
				    Big_End_BytetoDword(Psouce+1+4*Cnt0,&Curen_ID);
					if(Curen_ID==QuYu_Duo[Cnt].Duo_ID)
					{
						Same_State=EAT_TRUE;
						break;
					}
				}
				if(Same_State==EAT_TRUE)
				{
					Delet_Duo_Cnt++;
					Same_State=EAT_FALSE;
				}
				else
				{
					memcpy(Temp_Duo+Ofest,QuYu_Duo+Cnt,sizeof(QuYu_Duo[0]));
					Ofest++;//��ǰ��ַ
				}
			}
			Duo_QuYu_Cnt=Ofest;
			file_write(Qu_Yu_Save_File_duobian,0,&Duo_QuYu_Cnt,1);//�����ļ�
			if(Ofest!=0)
			{
				memcpy(QuYu_Duo,Temp_Duo,Duo_QuYu_Cnt*sizeof(QuYu_Duo[0]));
				file_write(Qu_Yu_Save_File_duobian,1,QuYu_Duo,Duo_QuYu_Cnt*sizeof(QuYu_Duo[0]));//�����ļ�
			}
			if(Delet_Duo_Cnt==*Psouce)
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Duobian_Delet_ID,0);//ɾ���ɹ�	
			}
			else
			{
				Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Duobian_Delet_ID,2);//ɾ�����ɹ�	
			}
		}
	}
	else
	{
		Tcp_VDR_Common_Replay(l_socket,SN,Tcp_QuYu_Duobian_Delet_ID,2);//ɾ�����ɹ�	
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_Duobian_QuYu
 *  Description:  �·���������� ���10��
 *  Parameters :  *psouce //��ID��ʼ������ָ��
 *  				Qu_Cnt �������
 *					ƫ��������Ŀǰ������ǰ���������õ�
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void Analyze_Duobian_QuYu(Qu_YU_Duo *Dest_Struct,u8 *psouce)
{
	uint16_t Cnt;
	uint16_t Date_Cnt=0;
	
	Big_End_BytetoDword(psouce + Date_Cnt,&(Dest_Struct->Duo_ID));
	Date_Cnt +=4;
	
	Big_End_BytetoWord(psouce + Date_Cnt,&(Dest_Struct->DUO_Shuxing));
	Date_Cnt +=2;
	if(((Dest_Struct->DUO_Shuxing)>>0)&0x0001)
	{	
		memcpy(Dest_Struct->Time_On,psouce+Date_Cnt,6);// ��������
		Date_Cnt +=6;
		memcpy(Dest_Struct->Time_OFF,psouce + Date_Cnt,6);// ��������
		Date_Cnt +=6;
	}
	if(((Dest_Struct->DUO_Shuxing)>>1)&0x0001)
	{		
        Big_End_BytetoWord(psouce + Date_Cnt,&(Dest_Struct->Max_Speed));
		Date_Cnt += 2;
		Dest_Struct->Time_O = *(psouce + Date_Cnt);//����ʱ
		Date_Cnt += 1;
	}
    Big_End_BytetoWord(psouce + Date_Cnt,&(Dest_Struct->Point_Cnt));
	Date_Cnt += 2;
	if((Dest_Struct->Point_Cnt) > Duo_Point_Max)
	{
		Dest_Struct->Point_Cnt = Duo_Point_Max;
	}
	for(Cnt=0;Cnt < Dest_Struct->Point_Cnt;Cnt++)
	{
	    Big_End_BytetoDword(psouce + Date_Cnt,&(Dest_Struct->Point_Savep[2*Cnt]));
		Date_Cnt += 4;
		
	    Big_End_BytetoDword(psouce + Date_Cnt,&(Dest_Struct->Point_Savep[2*Cnt+1]));
		Date_Cnt += 4;
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_QuYu_Struct_Duo
 *  Description: ������Ķ������Ϣ��ȡ
 *  Parameters :  Psource_Struct�ṹ��ָ��
 *  			Pdest Ŀ������
 *				OFest ���鳤��
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-59 			 
 * =====================================================================================
 */
 void File_Analyze_QuYu_Struct_Duo(void)
{
    file_create(Qu_Yu_Save_File_duobian);
	file_read(Qu_Yu_Save_File_duobian,0,&Duo_QuYu_Cnt,1);
	file_read(Qu_Yu_Save_File_duobian,1,QuYu_Duo,sizeof(QuYu_Duo));
}


