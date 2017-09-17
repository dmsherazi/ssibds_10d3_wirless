
#ifndef _IP_LOCK_PORC_H
#define _IP_LOCK_PORC_H

 /* 
* ===  NOTE ======================================================================
*  功能：实现IP锁定
*  实现方法：IP锁定参数保存在 _system_param_save_struct_ 用位域结构体实现
*  	uint32_t ip1_lock:1;//IP1锁定 默认关闭
*	uint32_t ip2_lock:1;//IP2锁定标志	默认关闭		   
*  
*  author	  :  J.x.b create at SSI_1D02 at 2017-8-31 			
* ================================================================================
*/
#include"eat_interface.h"



extern void ip_lock_data_proc(uint8_t *rev_data);

#endif

