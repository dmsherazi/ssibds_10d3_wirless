

#ifndef _SUBCONTRACT_SERVN_NUM_H
    #define _SUBCONTRACT_SERVN_NUM_H

	#include <stdio.h>
	#include <string.h>
	#include"eat_interface.h"

#define multimedia_updata_id 0x0801
#define driver_record_updata_id 0x0700

/*全局流水号*/
extern uint16_t global_tcp_msg_servn; 
/*多媒体上传全局流水号*/
extern uint16_t multimedia_start_msg_servn;
/*行驶记录上传全局流水号*/
extern uint16_t driver_start_msg_servn;
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Protocol_Init
 *  Description:  注册数据初始化
 *  Parameters :  void     
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
extern void subcontract_servn_num_pro(uint16_t msg_id,uint16_t package_cnt,uint16_t *msg_sn_f,uint16_t *msg_sn_out);

    #endif

