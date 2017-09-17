#ifndef USB_DEBUG_H
	#define USB_DEBUG_H
	

/***********************************************
 *  Include Files
 ***********************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "eat_interface.h"
#define usb_debug_printf 1

#define spi_flash_fs_test "0:abc.bin"
enum
{
    dbc_read=1,
    can_analyze=2,
    can_data_updata=3,
    demarcate_data=4,
    read_demarcate=5,
    creat_list=6,
    pint_list=7,
    spi_flash_t=8,
    fatfs_creat_t=9,
    fatfs_write_t=10,
    fatfs_read_t =11,
    fatfs_getsize_t=12,
    take_picture_t=13
};
extern void usb_debug_proc_inf(uint8_t *p_scr);

#endif

