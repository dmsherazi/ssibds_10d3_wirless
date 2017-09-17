/****************************************************
 * Include Files
 ****************************************************/ 
 
#include "send_heart_beat.h"



 /****************************************************
* External variables declaration
*****************************************************/


/*****************************************************
* Local variables declaration(Static)
******************************************************/

void Tcp_Heart_Beat(uint8_t socket)//发送心跳
{
	uint8_t Temp_buf=0;
	uint16_t msg_servn=0;
	Tcp_Message_PacKage_And_Send(socket,&msg_servn,Tcp_Heart_Beat_ID,&Temp_buf,0,EAT_FALSE,0,0);
}
void First_Server_Heat_Beat_Send(void)
{
	if(System_Flag.System_Flag_Bit.MainAutenSucess==EAT_TRUE)
	{
		if(System_Flag.System_Flag_Bit.Hart_Beat_Send == EAT_TRUE)
		{
 		     socket_close(Main_Link.mode_Socket);
		     Main_Link.link_staue =EAT_FALSE;
		     System_Flag.System_Flag_Bit.MainAutenSucess=EAT_FALSE;
		     #if send_heart_beat_printf
             eat_trace("First_Server_Heat_Beat_Send --> LinkFisrtServer");
		     #endif
             task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);//发送消息到USER_2
			 System_Flag.System_Flag_Bit.Hart_Beat_Send = EAT_FALSE;
		}
		else
		{
			Tcp_Heart_Beat(Main_Link.mode_Socket);//发送心跳
			System_Flag.System_Flag_Bit.Hart_Beat_Send = EAT_TRUE;
		}
	}
	else
	{
	    if(System_Flag.System_Flag_Bit.Hart_Beat_Send == EAT_TRUE)
		{
            socket_close(Main_Link.mode_Socket);
		    Main_Link.link_staue =EAT_FALSE;  
            #if send_heart_beat_printf
            eat_trace("First_Server_Heat_Beat_Send --> LinkFisrtServer");
            #endif
            task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);//发送消息到USER_2
			System_Flag.System_Flag_Bit.Hart_Beat_Send = EAT_FALSE;
		}
	}
}
void Second_Server_Heat_Beat_Send(void)
{
	if(System_Flag.System_Flag_Bit.SecondAutenSucess==EAT_TRUE)
	{
		if(System_Flag.System_Flag_Bit.Second_Hart_Beat_Send == EAT_TRUE)
		{
            socket_close(Second_Link.mode_Socket);
		    Second_Link.link_staue =EAT_FALSE;  
		    
		    System_Flag.System_Flag_Bit.SecondAutenSucess=EAT_FALSE;
			System_Flag.System_Flag_Bit.Second_Hart_Beat_Send = EAT_FALSE;
            task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);//发送消息到USER_2
		}
		else
		{
			Tcp_Heart_Beat(Second_Link.mode_Socket);//发送心跳
			System_Flag.System_Flag_Bit.Second_Hart_Beat_Send = EAT_TRUE;
		}
	}
	else
	{
		if(System_Flag.System_Flag_Bit.Second_Hart_Beat_Send == EAT_TRUE)
		{
            socket_close(Second_Link.mode_Socket);
		    Second_Link.link_staue =EAT_FALSE;  
		    
		    System_Flag.System_Flag_Bit.SecondAutenSucess=EAT_FALSE;
			System_Flag.System_Flag_Bit.Second_Hart_Beat_Send = EAT_FALSE;
            task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);//发送消息到USER_2
		}
	}
}

