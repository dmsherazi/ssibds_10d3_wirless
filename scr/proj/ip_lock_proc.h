
#ifndef _IP_LOCK_PORC_H
#define _IP_LOCK_PORC_H

 /* 
* ===  NOTE ======================================================================
*  ���ܣ�ʵ��IP����
*  ʵ�ַ�����IP�������������� _system_param_save_struct_ ��λ��ṹ��ʵ��
*  	uint32_t ip1_lock:1;//IP1���� Ĭ�Ϲر�
*	uint32_t ip2_lock:1;//IP2������־	Ĭ�Ϲر�		   
*  
*  author	  :  J.x.b create at SSI_1D02 at 2017-8-31 			
* ================================================================================
*/
#include"eat_interface.h"



extern void ip_lock_data_proc(uint8_t *rev_data);

#endif

