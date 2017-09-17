
#ifndef _SPI_FLASH_DRIVER_H
    #define _SPI_FLASH_DRIVER_H

/********************************************************************
 * Include Files
 ********************************************************************/
#include "eat_interface.h"



/*flash 操作命令定义定义*/
#define Sflash_WriteEnable	    0x06 //Write Enable (WREN 06h)
#define Sflash_WriteDisable	    0x04 //Write Disable(WRDI 04h)
#define Sflash_ReadStatusReg	0x05 //Read Status Register (RDSR1 05h)
#define Sflash_WriteStatusReg	0x01 // Write Registers (WRR 01h)
#define Sflash_ReadData		    0x03 //Read (3- or 4-byte address)
#define Sflash_FastReadData	    0x0B //Fast Read (3- or 4-byte address)
#define Sflash_PageProgram	    0x02// 0x32 //Page Program (3- or 4-byte address)
#define Sflash_SectorErase	    0x20//0x// 64KB Erase 64 kB or 256 kB (3- or 4-byte address)
#define Sflash_ChipErase		0xC7  //Bulk Erase (alternate command)
#define Sflash_PowerDown		0xB9  //Deep Power Down
#define Sflash_ReleasePowerDown	0xAB  //
#define Sflash_DeviceID		    0x9F
#define Sflash_ManufactDeviceID	0x90


/*flash 引脚接线定义*/
#define SFLASH_CS EAT_PIN42_ROW2
#define SFLASH_SO EAT_PIN41_ROW3 //SO#
#define SFLASH_WP EAT_PIN40_ROW4 //WP#
#define SFLASH_SI EAT_PIN47_COL4 //SI#
#define SFLASH_CLK EAT_PIN48_COL3//CLK
#define SFLASH_REST EAT_PIN51_COL0//REST



extern void sflash_io_init(void);
extern void sflash_write_nBytes(uint32_t Dst_Addr, uint8_t *upper_128, uint16_t nBytes_128);
extern void sflash_read_nBytes(uint32_t Dst_Addr, uint8_t *upper_128, uint32_t nBytes_128);
extern void sflash_Erase_Chip(void);
extern eat_bool sflash_Erase_Sector(uint32_t Dst_Addr);
extern void spi_flash_test_read1(void);
extern void spi_flash_test_read2(void);
extern void spi_flash_test_write(void);

/*file sytem */
extern int ATA_disk_status(void);
extern int ATA_disk_initialize(void);
extern int ATA_disk_read(uint8_t *buff, uint32_t sector, uint32_t count);
extern int ATA_disk_write(const uint8_t *buff, uint32_t sector,uint32_t count);


extern void sflash_file_init(void);
extern void sflash_file_create(const char *FileName);
extern void sflash_get_file_size(const char *FileName,uint32_t *Size);
extern void sflash_file_write(const char *FileName,int32_t Ofest,void *writeBuf, uint32_t Len);
extern void sflash_file_read(const char *FileName,int32_t Ofest,void *Readbuffer, uint32_t Len);
extern void spi_flash_format(void);

#endif



