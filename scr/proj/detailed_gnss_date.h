
#ifndef _DETAILED_GNSS_DATA_H
#define _DETAILED_GNSS_DATA_H


#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

#define detial_gnss_data_debug_printf 1

#define detail_gnss_file L"D:\\detail_gnss.bin"

#define data_penetrate_id 0x0900

/*详细定位数据清参数*/
extern uint8_t detial_data_befor_clean;
extern uint16_t detai_data_file_offest;
extern uint16_t detail_gnss_cnt ;

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  ask_board_gnss_data(uint8_t data_cmd)
 *  Description:请求GANSS数据
 *  Input:   cmd 
 *  Output:void
 *  author: J create at 2015-4-24
 * =====================================================================================
 */
extern void ask_board_gnss_data(uint8_t data_cmd);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:   void gnss_updata_handle(uint8_t *penetrate_data,uint16_t tempLens) 
 *  Description: GNSS数据保存
 *  Input:   speed 
 *  Output: 1
 *  author: J create at 2015-4-24
 * =====================================================================================
 */
extern void gnss_updata_handle(uint8_t *penetrate_data,uint16_t tempLens);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  GNSS_updata_handle
 *  Description:GNSS数据上传
 *  Input:   speed 
 *  Output: 1
 *  author: J create at 2015-4-24
 * =====================================================================================
 */
 extern void detail_gnss_data_updata_ctrl(void);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  ask_board_gnss_data(uint8_t data_cmd)
 *  Description:请求GANSS数据
 *  Input:   cmd 
 *  Output:void
 *  author: J create at 2015-4-24
 * =====================================================================================
 */
extern void ask_board_gnss_data(uint8_t data_cmd);

#endif

