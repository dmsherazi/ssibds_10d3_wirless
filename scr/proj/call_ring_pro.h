
#ifndef _CALL_RING_PROC_H
#define _CALL_RING_PROC_H

#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

#define call_ring_pro_debug_printf 1

#define tcp_dail_vdr_call_id 0x8400
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  Calling_Handle(u8 *pscr)
 *  Description:  ����AT��������
 *  Parameters :(u8 *pscr)Դ����ָ��
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2015-6-16 			 
 * =====================================================================================
 */
extern void call_ring_data_num_proc(uint8_t *pscr);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  void dail_voice_call(uint8_t *point_num)
 *  Description: ���������绰
 *  Parameters :(u8 *pscr)Դ����ָ��
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2015-6-16 			 
 * =====================================================================================
 */
extern void dail_voice_call(uint8_t *point_num);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: tcp_vdr_dail_call
 *  Description: TCP���������绰
 *  Parameters :(u8 *pscr)Դ����ָ��
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2015-6-16 			 
 * =====================================================================================
 */
extern void tcp_vdr_dail_call(uint8_t l_socket,uint16_t msg_svn,uint8_t *tcp_scr);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name       : phone_over_proc
 *  Description: �绰��ز�����ȡ
 *  Parameters : void
 *       author: J.x.b  create at 2014-5-28
 * =====================================================================================
 */
extern void phone_over_proc(u8 *at_buffer);

#endif

