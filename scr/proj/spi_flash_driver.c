/* 
 * ===  FUNCTION  ======================================================================
 *  Name: spi_flash_drinver.c
 *  Description: spionon spi flash driver 
 *        
 *       author: J.x.b  create at 2014-5-12
 * =====================================================================================
 */
/******************************************************************************************
软件驱动程序(C8051F020)
SST25VF016B 16 Mbit(2M x 8) 串行Flash存储器
(2007年4月19日)
关于此程序:
此程序为用户使用该FLASH提供了一个完整功能函数库,用户可根据自己的需要使用这些函数 
------------------------------------------------------------------
init                    初始化时钟,进入模式0
Send_Byte               用MOSI发送一字节(上升沿)
Get_Byte                用MISO接收一字节(下降沿)
Poll_SO                 使用MISO作为AAI模式下的RY/BY#输出
芯片的功能函数包:
 ------------------------------------------------------------------
Read_Status_Register    读取状态寄存器
EWSR                    使能写状态寄存器
WRSR                    改写状态寄存器
WREN                    写使能
WRDI                    写禁止
EBSY                    允许使用MISO 作为AAI模式下RY/BY#的输出脚
DBSY                    禁止用MISO输出RY/BY#
Read_ID                 读取厂商ID或芯片ID
Jedec_ID_Read           读取全部ID
Read                    读取一个字节,返回byte(最大25 MHz CLK的频率)
Read_Cont               读取连续地址内的数据(最大25 MHz时钟频率)
HighSpeed_Read          读取一个字节(最大50Mhz时钟频率)并返回byte
HighSpeed_Read_Cont     连续读取(最大50 MHz时钟频率)
Byte_Program            写一个字节数据
Auto_Add_IncA           初始化Auto Address Increment(AAI)
Auto_Add_IncB           AAI初始化后进入Auto_Address_Increment(AAI)
Auto_Add_IncA_EBSY      带EBSY的初始化Auto Address Increment(AAI) 
Auto_Add_IncB_EBSY      带EBSY的Auto_Address_Increment (AAI)
Chip_Erase              擦除整个芯片
Sector_Erase            擦除一个扇区(4 KB)
Block_Erase_32K         擦除一块32 KByte的区域
Block_Erase_64K         擦除一块64 KByte的区域
Wait_Busy               等待空闲(状态寄存器的BUSY位为0)
Wait_Busy_AAI           AAI模式下等待空闲
WREN_Check              检查WEL是否被置位
WREN_AAI_Check          检查WEL和AAI模式位被置位
          
************************************************************************/

#include "spi_flash_driver.h"

FATFS FatFs;


/************************************************************************/
/* 程序名称:    sflash_io_init                            */
/*                                  */
/* 程序功能:    用来将时钟线初始化为低. 必须在将设备设置为模式0之前调用*/
/* 输入    :    无                          */
/*                                  */
/* 输出    :                          */
/************************************************************************/
static  void _delay_us(uint32_t delay)
{
	if(delay==0)delay=3;
	delay = 3*delay;
 	while(delay--);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  sflash_CS 
 *  Description:  片选使能与清除
 *  Parameters :    
 *  Return     :   
 *  author     :  J.x.b create at TQ-TVM at 2016-4-10          
 * =====================================================================================
 */
void set_sflash_CS(void)
{
	eat_gpio_setup(SFLASH_CS, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);
}
void clr_sflash_CS(void)
{
	eat_gpio_setup(SFLASH_CS, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: sflash_SI
 *  Description:  单片机输出高低电平
 *  Parameters :    
 *  Return     :   
 *  author     :  J.x.b create at TQ-TVM at 2016-4-10          
 * =====================================================================================
 */
void set_sflash_SI(void)
{
	eat_gpio_setup(SFLASH_SI, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);
}

void clr_sflash_SI(void)
{
	eat_gpio_setup(SFLASH_SI, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: sflash_WP
 *  Description:  写保护使能
 *  Parameters :    
 *  Return     :   
 *  author     :  J.x.b create at TQ-TVM at 2016-4-10          
 * =====================================================================================
 */
void set_sflash_WP(void)
{
	eat_gpio_setup(SFLASH_WP, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);
}
void clr_sflash_WP(void)
{
	eat_gpio_setup(SFLASH_WP, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);
}

/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_CLK
*  Description:  时钟
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
void set_sflash_CLK(void)
{
	eat_gpio_setup(SFLASH_CLK, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);
}    
void clr_sflash_CLK(void)
{
	eat_gpio_setup(SFLASH_CLK, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_CLK
*  Description:  时钟
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
void set_sflash_REST(void)
{
	eat_gpio_setup(SFLASH_REST, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);
}
void clr_sflash_REST(void)
{
	eat_gpio_setup(SFLASH_REST, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: get_sflash_DO
*  Description:  获取FLASHid
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
uint8_t  get_sflash_DO(void)
{
    EatGpioLevel_enum IoValue;
	eat_gpio_setup(SFLASH_SO, EAT_GPIO_DIR_INPUT, EAT_GPIO_LEVEL_LOW);
    IoValue = eat_gpio_read(SFLASH_SO);
    return(IoValue);
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_send_Byte
*  Description:  发送一个字节
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
void sflash_send_Byte(uint8_t outDat)
{
    uint8_t i = 0;    
    clr_sflash_CS();
    for (i = 0; i < 8; i++)
    {
        if ((outDat & 0x80) == 0x80) /* check if MSB is high */
        {
            set_sflash_SI();
        }
        else
        {
            clr_sflash_SI();     /* if not, set to low */
        }
        set_sflash_CLK();        /* toggle clock high */
        outDat = (outDat << 1);  /* shift 1 place for next bit */
        //_delay_us(1);
        clr_sflash_CLK();        /* toggle clock low */
     }
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_get_Byte
*  Description:  读取一个字节
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
uint8_t sflash_get_Byte(void)
{
    uint8_t i = 0, in = 0, temp = 0;  
    clr_sflash_CS();
    for (i = 0; i < 8; i++)
    {
        in = (in << 1);          /* shift 1 place to the left or shift in 0 */
        temp = get_sflash_DO();  /* save input */
        set_sflash_CLK();        /* toggle clock high */
        if (temp == 1)           /* check to see if bit is high */
        {
            in |= 0x01;          /* if high, make bit high */
        }
        //_delay_us(1);
        clr_sflash_CLK();        /* toggle clock low */
        
    }   
    return in;
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_read_StatusReg
*  Description:  读状态寄存器
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
uint8_t sflash_read_StatusReg(void)
{
    uint8_t byte = 0;
    clr_sflash_CS();                           /* enable device */
    sflash_send_Byte(Sflash_ReadStatusReg);    /* send Read Status Register command */
    byte = sflash_get_Byte();                  /* receive byte */
    set_sflash_CS();                           /* disable device */
    return byte;
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_wait_Busy
*  Description:  读状态寄存器
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
eat_bool sflash_wait_Busy(void)
{
    /*  waste time until not busy WEL & Busy bit all be 1 (0x03).   */
    while (sflash_read_StatusReg() == 0x03)
    {
        if(sflash_read_StatusReg()==0x01)
        {
            return EAT_FALSE;
        }
    }
    return EAT_TRUE;
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_write_StatusReg
*  Description:  写寄存器状态
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
void sflash_write_StatusReg(unsigned char byte)
{
    clr_sflash_CS();                          /* enable device */
    sflash_send_Byte(Sflash_WriteStatusReg);  /* select write to status register */
    sflash_send_Byte(byte);                   /* data that will change the status(only bits 2,3,7 can be written) */
    set_sflash_CS();                          /* disable the device */
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_write_Enable
*  Description:  写使能
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
void sflash_write_Enable(void)
{
    clr_sflash_CS();                           /* enable device */
    sflash_send_Byte(Sflash_WriteEnable);      /* send W25P_Write_Enable command */
    set_sflash_CS();                           /* disable device */
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_Powerdown
*  Description:  关机低功耗
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
void sflash_Powerdown(void)
{
    clr_sflash_CS();                          /* enable device */
    sflash_send_Byte(Sflash_PowerDown);       /* send W25P_PowerDown command 0xB9 */
    set_sflash_CS();                          /* disable device */
    _delay_us(6);                             /* remain CS high for tPD = 3uS */
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_Powerdown
*  Description:  关机低功耗
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
void sflash_ReleasePowerDown(void)
{
    clr_sflash_CS();                           /* enable device */
    sflash_send_Byte(Sflash_ReleasePowerDown); /* send W25P_PowerDown command 0xAB */
    set_sflash_CS();                           /* disable device */
    _delay_us(6);                              /* remain CS high for tRES1 = 3uS */
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_Read_DeviceID
*  Description:  读取制造商ID
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
uint8_t sflash_Read_DeviceID(void)
{
    uint8_t byte;
    clr_sflash_CS();                     /* enable device */
    sflash_send_Byte(Sflash_DeviceID);   /* send read device ID command (ABh) */
    sflash_send_Byte(0);                 /* send address */
    sflash_send_Byte(0);                 /* send address */
    sflash_send_Byte(0);                 /* send 3_Dummy address */
    byte = sflash_get_Byte();            /* receive Device ID byte */
    set_sflash_CS();                     /* disable device */
    _delay_us(4);                        /* remain CS high for tRES2 = 1.8uS */
    return byte;
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_Read_DeviceID
*  Description:  读取制造商ID
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
uint32_t sflash_Read_ManufactDeviceID(uint8_t ID_Addr)
{
    uint32_t IData16;
    clr_sflash_CS();                           /* enable device */
    sflash_send_Byte(Sflash_ManufactDeviceID); /* send read ID command (90h) */
    sflash_send_Byte(0x00);                    /* send address */
    sflash_send_Byte(0x00);                    /* send address */
    sflash_send_Byte(ID_Addr);                 /* send W25Pxx selectable ID address 00H or 01H */
    IData16 = sflash_get_Byte()<<8;            /* receive Manufature or Device ID byte */
    IData16 |= sflash_get_Byte();              /* receive Device or Manufacture ID byte */
    set_sflash_CS();                           /* disable device */
    return IData16;
}

uint32_t sflash_Read_ID3(void)
{
    uint32_t IData16;
    clr_sflash_CS();                          /* enable device */
    sflash_send_Byte(0x9f);                   /* send read ID command (90h) */
        
    IData16 = sflash_get_Byte()<<8;           /* receive Manufature or Device ID byte */
    IData16 |= sflash_get_Byte();             /* receive Device or Manufacture ID byte */
    set_sflash_CS();                          /* disable device */
        
    return IData16;
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sflash_Read_Byte
*  Description: 读取一个字节
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
uint8_t sflash_Read_Byte(uint32_t Dst_Addr) 
{
    uint8_t byte = 0; 
    clr_sflash_CS();                                                /* enable device */
    sflash_send_Byte(Sflash_ReadData);                              /* read command */
    sflash_send_Byte((unsigned char)((Dst_Addr & 0xFFFFFF) >> 16));/* send 3 address bytes */
    sflash_send_Byte((unsigned char)((Dst_Addr & 0xFFFF) >> 8));
    sflash_send_Byte((unsigned char)(Dst_Addr & 0xFF));
    byte = sflash_get_Byte();
    set_sflash_CS();                                                /* disable device */
    return byte;                                                    /* return one byte read */
}

uint8_t sflash_FastRead_Byte(uint32_t Dst_Addr) 
{
    uint8_t byte = 0; 
    clr_sflash_CS();                                   /* enable device */
    sflash_send_Byte(Sflash_FastReadData);             /* fast read command */
    sflash_send_Byte(((Dst_Addr & 0xFFFFFF) >> 16));  /* send 3 address bytes */
    sflash_send_Byte(((Dst_Addr & 0xFFFF) >> 8));
    sflash_send_Byte(Dst_Addr & 0xFF);
    sflash_send_Byte(0xFF);                            /*dummy byte*/
    byte = sflash_get_Byte();
    set_sflash_CS();                                   /* disable device */
    return byte;                                       /* return one byte read */
}

void sflash_Erase_Chip(void)
{
    clr_sflash_CS();                                          /* enable device */
    sflash_write_Enable();                                    /* set WEL */
    clr_sflash_CS();
    sflash_wait_Busy();
    clr_sflash_CS();
    sflash_send_Byte(Sflash_ChipErase);                       /* send Chip Erase command */
    set_sflash_CS();                                          /* disable device */
}
    
eat_bool sflash_Erase_Sector(uint32_t sector_cnt)
{
    eat_bool result;
    uint32_t Dst_Addr;
    Dst_Addr=(sector_cnt<<12);
    clr_sflash_CS();                                          /* enable device */
    sflash_write_Enable();                                    /* set WEL */
    set_sflash_CS();
    result=sflash_wait_Busy();
    if(result==EAT_FALSE)
    {
        return EAT_FALSE;
    }
    clr_sflash_CS();
    sflash_send_Byte(Sflash_SectorErase);                     /* send Sector Erase command */
    sflash_send_Byte((uint8_t)((Dst_Addr & 0xFFFFFF) >> 16));/* send 3 address bytes */
    sflash_send_Byte((uint8_t)((Dst_Addr & 0xFFFF) >> 8));
    sflash_send_Byte((uint8_t)Dst_Addr & 0xFF);
    set_sflash_CS();                                          /* disable device */
    result=sflash_wait_Busy();
    return result;
}
    
void sflash_Verify(uint8_t byte, uint8_t cor_byte)
{
    if (byte != cor_byte)
    {
            //while(1);
    }
}
    
void  sflash_write_Disable(void)
{
    clr_sflash_CS();                        /* enable device */
    sflash_send_Byte(Sflash_WriteDisable);  /* send W25P_Write_Disable command */
    set_sflash_CS();                        /* disable device */
}
    
void sflash_write_nBytes(uint32_t Dst_Addr, uint8_t *upper_128, uint16_t nBytes_128)
{   
    uint16_t i=0;
    clr_sflash_CS();                                  /* enable device */
    sflash_write_Enable();                            /* set WEL */
    clr_sflash_CS();
    sflash_send_Byte(Sflash_PageProgram);/* send Byte Program command */
    sflash_send_Byte(((Dst_Addr & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sflash_send_Byte(((Dst_Addr & 0xFFFF) >> 8));
    sflash_send_Byte(Dst_Addr & 0xFF);
    for (i = 0; i < nBytes_128; i++)
    {
        sflash_send_Byte(*(upper_128+i));/* send byte to be programmed */
    }   
    set_sflash_CS(); /* disable device */    
}
void sflash_read_nBytes(uint32_t Dst_Addr, uint8_t *upper_128, uint32_t nBytes_128)
{
    uint32_t i = 0;
    #if APP_TRACE_DBG 
    eat_trace("sflash_read_nBytes -->sec_addrr:%d,size:%d",Dst_Addr,nBytes_128);
    #endif           
    clr_sflash_CS();                                /* enable device */
    sflash_send_Byte(Sflash_ReadData);              /* read command */
    sflash_send_Byte(((Dst_Addr & 0xFFFFFF) >> 16));/* send 3 address bytes */
    sflash_send_Byte(((Dst_Addr & 0xFFFF) >> 8));
    sflash_send_Byte(Dst_Addr & 0xFF);
    for (i = 0; i < nBytes_128; i++)                /* read until no_bytes is reached */
    {
        *(upper_128+i)= sflash_get_Byte();          /* receive byte and store at address 80H - FFH */
    }
    set_sflash_CS();                                /* disable device */    
}
void sflash_fastread_nBytes(uint32_t Dst_Addr, uint8_t *upper_128, uint32_t nBytes_128)
{
    uint32_t i = 0;
            
    clr_sflash_CS();                                 /* enable device */
    sflash_send_Byte(Sflash_FastReadData);           /* read command */
    sflash_send_Byte(((Dst_Addr & 0xFFFFFF) >> 16));/* send 3 address bytes */
    sflash_send_Byte(((Dst_Addr & 0xFFFF) >> 8));
    sflash_send_Byte(Dst_Addr & 0xFF);
    sflash_send_Byte(0xFF);                          /*dummy byte*/
    for (i = 0; i < nBytes_128; i++)                 /* read until no_bytes is reached */
    {
        *(upper_128+i) = sflash_get_Byte();          /* receive byte and store at address 80H - FFH */
    }
    set_sflash_CS();                                 /* disable device */
}
    
void sflash_io_init(void)
{
    
    eat_pin_set_mode(SFLASH_WP, EAT_PIN_MODE_GPIO);
    eat_gpio_setup(SFLASH_WP, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);

    eat_pin_set_mode(SFLASH_CLK, EAT_PIN_MODE_GPIO);
    eat_gpio_setup(SFLASH_CLK, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);
    
    eat_pin_set_mode(SFLASH_CS, EAT_PIN_MODE_GPIO);
    eat_gpio_setup(SFLASH_CS, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);
    
    eat_pin_set_mode(SFLASH_SO, EAT_PIN_MODE_GPIO);
    eat_gpio_setup(SFLASH_SO, EAT_GPIO_DIR_INPUT, EAT_GPIO_LEVEL_HIGH);

    eat_pin_set_mode(SFLASH_REST, EAT_PIN_MODE_GPIO);
    eat_gpio_setup(SFLASH_REST, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);
    
    eat_pin_set_mode(SFLASH_SI, EAT_PIN_MODE_GPIO);
    eat_gpio_setup(SFLASH_SI, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);

    sflash_write_Enable(); 
}    



void spi_flash_test_write(void)
{
    
    uint8_t read_buf[4096]={0};
    uint32_t address = 0;
    uint8_t write_buf[4096]= "abcdeftghklmnabcdeftghklmnabcdeftghklmnabcdeftghklmnspi spi flash test read one sector!abcdeftghklmnabcdeftghklmnabcdeftghklmnabcdeftghklmnspi " ;
    uint32_t t1,t2;
    
    t1 = eat_get_current_time();
    ATA_disk_write(write_buf,0,1);
    t2 = eat_get_duration_us(t1);
    #if APP_TRACE_DBG 
    eat_trace("ATA_disk_write --> using time t=%u.!",t2);
    #endif
    #if APP_TRACE_DBG 
    eat_trace("spi_flash_test_write -->spi_flash_test_write:%s",write_buf);
    #endif
    #if 0
    sflash_Erase_Sector(address);
    sflash_write_nBytes(address,write_buf,256);
    #if APP_TRACE_DBG 
    eat_trace("spi_flash_test -->sflash_write_nBytes:%s",write_buf);
    #endif
    memset(read_buf,0,256);
    sflash_read_nBytes(address,read_buf,strlen(write_buf));
    #if APP_TRACE_DBG 
    eat_trace("spi_flash_test -->sflash_read_nBytes00:%s",read_buf);
    #endif
    
    memset(read_buf,0,256);
    sflash_read_nBytes(address,read_buf,strlen(write_buf));
    #if APP_TRACE_DBG 
    eat_trace("spi_flash_test -->sflash_read_nBytes11:%s",read_buf);
    #endif


    sflash_Erase_Sector(address);
    memset(read_buf,0,256);
    sflash_write_nBytes(address,"abcdeftghklmnabcdeftghklmnabcdeftghklmnabcdeftghklmn",strlen("abcdeftghklmnabcdeftghklmnabcdeftghklmnabcdeftghklmn"));
    
    memset(read_buf,0,256);
    sflash_read_nBytes(address,read_buf,strlen("abcdeftghklmnabcdeftghklmnabcdeftghklmnabcdeftghklmn"));
    #if APP_TRACE_DBG 
    eat_trace("spi_flash_test -->sflash_read_nBytes:%s",read_buf);
    #endif
    #endif
}


void spi_flash_test_read1(void)
{

    uint8_t read_buf[4096]={0};
    uint32_t address = 16752640;
    uint8_t write_buf[4096]= "2222222222222222222222222222222222222222222222222222222222222222" ;

    ATA_disk_write(write_buf,0,1);
    #if APP_TRACE_DBG 
    eat_trace("spi_flash_test_write -->spi_flash_test_write:%s",write_buf);
    #endif
}
void spi_flash_test_read2(void)
{
    uint8_t read_buf[4096]={0};
    uint32_t t1,t2;
    t1 = eat_get_current_time();
    ATA_disk_read(read_buf,0,1);
    t2 = eat_get_duration_us(t1);
    #if APP_TRACE_DBG 
    eat_trace("ATA_disk_read --> using time t=%u.!",t2);
    #endif
    #if APP_TRACE_DBG 
    eat_trace("spi_flash_test_read -->spi_flash_test_read19sect");
    HexToAsicc(read_buf,4096);
    #endif
}


/* 
* ===  FUNCTION  ======================================================================
*  Name: RAM_disk_status
*  Description: Fatfs 文件系统spi FLASH一直处于可以处理状态
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-7-10          
* =====================================================================================
*/
int ATA_disk_status(void)
{
    return 0;
}

/* 
* ===  FUNCTION  ======================================================================
*  Name: RAM_disk_initialize
*  Description: Fatfs 文件系统初始化函数
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at ssi1d03 at 2016-7-10          
* =====================================================================================
*/
int ATA_disk_initialize(void)
{
    sflash_io_init();
    return 0;
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: RAM_disk_read
*  Description: Fatfs 文件系读每个扇区。sector<<12 4K的吧倍数
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-ssi1d03 at 2016-7-10          
* =====================================================================================
*/
int ATA_disk_read(uint8_t *buff, uint32_t sector, uint32_t count)
{
    sflash_fastread_nBytes((sector<<12),buff,(count<<12));
    return 0;
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: RAM_disk_write
*  Description: Fatfs 文件系统写扇区。
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at ssi1d03 at 2016-7-10          
* =====================================================================================
*/
int ATA_disk_write(const uint8_t *buff, uint32_t sector,uint32_t count)
{
    uint32_t i = 0,page_cn=0;
    uint32_t sec_addrr = 0;
    uint32_t t1,t2;
    for(i=0; i<count;i++)
    {
        if(sflash_Erase_Sector(sector)==EAT_FALSE)
        {
            return RES_ERROR;
        }
        sec_addrr = (sector<<12);
        for(page_cn=0;page_cn<16;page_cn++)
        {
            sflash_write_nBytes(sec_addrr,(uint8_t*)buff,256);
            buff += 256;
            sec_addrr += 256;
            _delay_us(1500);
        }
        sector++;
    }
    return RES_OK;
}

/**********************************SPI flash 文件系统操作*******************************/
void spi_flash_format(void)
{
    FRESULT fr;    /* FatFs return code */
    /* Register work area to the default drive */
    fr=f_mount(&FatFs, "0:", 0);
    eat_trace("spi_flash_format -->f_mount:%d",fr);
    
    fr = f_mkfs("0:",1,0);
    eat_trace("spi_flash_format -->f_mkfs:%d",fr);
}
void sflash_file_init(void)
{
    FRESULT result;
    disk_initialize(0);
    result=f_mount(&FatFs, "0:", 0);
    if(result != FR_OK)
    {
        #if APP_TRACE_DBG 
        eat_trace("sflash_file_init -->init error id:%d!!",result);
        #endif
    }
    else
    {
        #if APP_TRACE_DBG 
        eat_trace("sflash_file_init -->init sucess!");
        #endif
    }
}

void sflash_file_create(const char *FileName)
{
    FIL fsrc;      
    FRESULT result;
    result = f_open(&fsrc,FileName,FA_CREATE_ALWAYS);
    if(result != FR_OK)
    {
        #if APP_TRACE_DBG 
        eat_trace("sflash_file_create -->f_open error id:%d!!",result);
        #endif
    }
    else
    {
        #if APP_TRACE_DBG 
        eat_trace("sflash_file_create -->sucess!");
        #endif
    }
    f_close(&fsrc);
}
void sflash_get_file_size(const char *FileName,uint32_t *Size)
{
    FIL fsrc;      
    FRESULT result;
    result = f_open(&fsrc,FileName,FA_READ);
    if(result != FR_OK)
    {
        #if APP_TRACE_DBG 
        eat_trace("sflash_file_create -->error id:%d!!",result);
        #endif
        f_close(&fsrc);
    }
    else
    {
        *Size = f_size(&fsrc);
        #if APP_TRACE_DBG 
        eat_trace("sflash_file_create -->sucess!");
        #endif
        f_close(&fsrc);
    }
}
void sflash_file_write(const char *FileName,int32_t Ofest,void *writeBuf, uint32_t Len)
{
    FIL fsrc;      
    uint32_t write_len=0;
    FRESULT result,seek_result,write_result,syns_result;
    result = f_open(&fsrc,FileName,FA_WRITE);
    if(result != FR_OK)
    {
        #if APP_TRACE_DBG 
        eat_trace("sflash_file_write -->f_open error id:%d!!",result);
        #endif
        f_close(&fsrc);
    }
    else
    {        
        seek_result = f_lseek (&fsrc,Ofest);//偏移指针
        if(seek_result != FR_OK)
        {
            #if APP_TRACE_DBG 
            eat_trace("sflash_file_write -->f_lseek error id:%d!!",seek_result);
            #endif
            f_close(&fsrc);
        }
        else
        {        
            write_result = f_write(&fsrc,writeBuf,Len,&write_len);
            if(write_result != FR_OK)
            {
                #if APP_TRACE_DBG 
                eat_trace("sflash_file_write -->f_lseek error id:%d!!",write_result);
                #endif
                f_close(&fsrc);
            }
            else
            { 
                syns_result=f_sync(&fsrc);
                if(syns_result!=FR_OK)
                {
                    #if APP_TRACE_DBG 
                    eat_trace("sflash_file_write -->f_sync error id:%d!!",syns_result);
                    #endif
                    f_close(&fsrc);
                }
                else
                {
                    f_close(&fsrc);
                    #if APP_TRACE_DBG 
                    eat_trace("sflash_file_write -->write sucess!!");
                    #endif
                }
            }
        }
    }
}
void sflash_file_read(const char *FileName,int32_t Ofest,void *Readbuffer, uint32_t Len)
{
    FIL fsrc;      
    uint32_t read_len=0;
    FRESULT result,seek_result,read_result;
    result = f_open(&fsrc,FileName,FA_READ);
    if(result != FR_OK)
    {
        #if APP_TRACE_DBG 
        eat_trace("sflash_file_read -->error id:%d!!",result);
        #endif
        f_close(&fsrc);
    }
    else
    {        
        seek_result = f_lseek (&fsrc,Ofest);//偏移指针
        if(seek_result != FR_OK)
        {
            #if APP_TRACE_DBG 
            eat_trace("sflash_file_read -->f_lseek error id:%d!!",seek_result);
            #endif
            f_close(&fsrc);
        }
        else
        {        
            read_result = f_read(&fsrc,Readbuffer,Len,&read_len);
            if(read_result != FR_OK)
            {
                #if APP_TRACE_DBG 
                eat_trace("sflash_file_read -->f_read error id:%d!!",read_result);
                #endif
                f_close(&fsrc);
            }
            else
            { 
                #if APP_TRACE_DBG 
                eat_trace("sflash_file_read --> sucess !");
                #endif
            }
        }
    }    
}

