

#include "ip_lock_proc.h"


void ip_lock_data_proc(uint8_t *rev_data)
{
	uint16_t ip1_lock_state=0;	//ip1����״̬
	uint16_t ip2_lock_state=0;	//ip2����״̬
	uint8_t data_offest=0;//����ƫ����
	
	Big_End_BytetoWord((rev_data+data_offest), &ip1_lock_state); //ip1����״̬
	data_offest+=2;
	data_offest+=6;
	Big_End_BytetoWord((rev_data+data_offest), &ip2_lock_state); //ip2����״̬

	/*IP1����*/
	if(ip1_lock_state==0x00)
	{
		if(system_param.param_save.statue.ip1_lock==0x01)
		{
			system_param.param_save.statue.ip1_lock=0x00;
		}
	}
	else if(ip1_lock_state==0x01)
	{
		if(system_param.param_save.statue.ip1_lock==0x00)
		{
			system_param.param_save.statue.ip1_lock=0x01;
		}
	}

	/*IP2����*/
	if(ip2_lock_state==0x00)
	{
		if(system_param.param_save.statue.ip2_lock==0x01)
		{
			system_param.param_save.statue.ip2_lock=0x00;
		}
	}
	else if(ip2_lock_state==0x01)
	{
		if(system_param.param_save.statue.ip2_lock==0x00)
		{
			system_param.param_save.statue.ip2_lock=0x01;
		}
	}
}

