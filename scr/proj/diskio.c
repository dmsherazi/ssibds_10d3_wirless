/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: USB drive control */
#include "spi_flash_driver.h"	/* Example: ATA drive control */
//#include "sdcard.h"		/* Example: MMC/SDC contorl */

/* Definitions of physical drive number for each media */
#define ATA		0
#define MMC		1
#define USB		2


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :
		result = ATA_disk_initialize();
		// translate the reslut code here
        stat = result;
		return stat;
		// translate the reslut code here


	case MMC :
		//result = MMC_disk_initialize();

		// translate the reslut code here

		return RES_OK;

	case USB :
		//result = USB_disk_initialize();

		// translate the reslut code here

		return RES_OK;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :
		result = ATA_disk_status();

		// translate the reslut code here
        stat = result;
		return stat;

	case MMC :
		//result = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case USB :
		//result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		result = ATA_disk_read(buff, sector, count);

		// translate the reslut code here
        res = result;

		return res;

	case MMC :
		// translate the arguments here

		//result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return RES_OK;

	case USB :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return RES_OK;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		result = ATA_disk_write(buff, sector, count);

		// translate the reslut code here
        res = result;

		return res;

	case MMC :
		// translate the arguments here

		//result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return RES_OK;

	case USB :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return RES_OK;
	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (  
BYTE pdrv,  /* Physical drive nmuber (0..) */  
BYTE cmd,  /* Control code */ 
void *buff  /* Buffer to send/receive control data */  
) 
{  
    //DWORD nFrom,nTo;
    //int i;
    if(pdrv != 0)  
    return RES_WRPRT;   
    switch(cmd)  
    {   
        case CTRL_SYNC: //spi-flash不需要同步，这一项始终返回0   
        return RES_OK;     
        case GET_SECTOR_SIZE:   
        *((WORD *)buff)   = 4096;   //始终通过buff返回扇区大小就可以了   
        return RES_OK;     
        case GET_SECTOR_COUNT:
        *((DWORD *)buff)   = 2048;//始终通过buff返回总扇区数就可以了   
        return RES_OK;     
        case GET_BLOCK_SIZE://禁止擦除功能，这两项无意义 
         *((DWORD *)buff)   = 65536;
        return RES_OK;   
        case CTRL_ERASE_SECTOR:
        
        return RES_OK; 
        default :
        return RES_OK;     
        
     }
     
}

#endif
DWORD get_fattime(void)
{
    char result=0;
    DWORD time_curren=0,MID_DWORD=0;
	EatRtc_st rtc = {0};
	result = eat_get_rtc(&rtc);
	if(result)
	{   
	    MID_DWORD = rtc.year;
	    time_curren |=(MID_DWORD<<25);
        
	    MID_DWORD = rtc.mon;
	    time_curren |=(MID_DWORD<<21);
	    
	    MID_DWORD = rtc.day;
	    time_curren |=(MID_DWORD<<16);
	    
	    MID_DWORD = rtc.hour;
	    time_curren |=(MID_DWORD<<11);
	    
	    MID_DWORD = rtc.min;
	    time_curren |=(MID_DWORD<<5);


	    MID_DWORD = rtc.sec;
	    time_curren |=(MID_DWORD>>1);
    }
    return time_curren;
}
