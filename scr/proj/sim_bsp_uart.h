#ifndef _SIM_BSP_H
	#define _SIM_BSP_H
	

/***********************************************
 *  Include Files
 ***********************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "eat_interface.h"
#define sim_bsp_uart_debug_printf 0

#define EAT_UART_RX_BUF_LEN_MAX 2048
#define EAT_MODE_RX_BUF_LEN_MAX 1024

extern void sim_bsp_init_uart1(void);//系统初始化
extern void sim_bsp_init_dbg(void);//usb uart3;
extern void sim_bsp_init_uart2(void);

#endif

