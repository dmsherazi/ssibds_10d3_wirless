/****************************************************
 * Include Files
****************************************************/ 
 
 #include "usb_debug_proc.h"

 /****************************************************
* External variables declaration
*****************************************************/

/*****************************************************
* Local variables declaration(Static)
******************************************************/
void usb_debug_proc_inf(uint8_t *p_scr)
{
    uint8_t creat_len=0;
    uint32_t t1,t2;
    uint32_t debug_param_id=0;
    uint8_t data_buff[10]={0x02,0x00,0x01,0x00,0x03,0x01,0x00,0x00,0x00,0x00};
    sscanf(p_scr,"jxb_usb_debug_param:%u",&debug_param_id);
    #if usb_debug_printf 
    eat_trace("jxb_usb_debug_param:%u",debug_param_id);
    #endif
    create_send_rs485_frame();
    //data_delay_test =debug_param_id;
    switch(debug_param_id)
    {
        case dbc_read:
            t1 = eat_get_current_time();
            dbc_find_start(); 
            t2 = eat_get_duration_us(t1);
            #if usb_debug_printf 
            eat_trace("usb_debug_proc_inf --> using time is t=%u.",t2);
            #endif
            break;
        case can_analyze:
            t1 = eat_get_current_time();
            //analyze_can_data(60,can_id+8);
            t2 = eat_get_duration_us(t1);
            #if usb_debug_printf 
            eat_trace("usb_debug_proc_inf --> using time is t=%u.!!!!!!!",t2);
            #endif
            break;
        case can_data_updata:
            #if 0//APP_TRACE_DBG 
            eat_trace("usb_debug_proc_inf --> can_data_updata.");
            Enter_Uart_Data_Queue(&receive_can_data,can_id,728);
            task_msg_send(EAT_USER_0,EAT_USER_3,analyze_can_data_msg);//发送解析ua数据
            #endif
            #if usb_debug_printf 
            eat_trace("usb_debug_proc_inf --> can_data_send_proc.");
            board_can_inf_setting();
            #endif
            break;
       case demarcate_data:
       
            #if usb_debug_printf 
            eat_trace("demarcate_data_proc --> test!!!!.");
            #endif
            //demarcate_data_proc("61.183.229.50;60002;1000700"); 
            break;
       case read_demarcate:
            #if usb_debug_printf 
            eat_trace("read_demarcate_data_proc --> test!!!!.");
            #endif
            break;
        case creat_list:
            #if usb_debug_printf 
            eat_trace("usb_debug_proc_inf --> creat_list test!!!!.");
            #endif
             for(creat_len=0;creat_len<20;creat_len++)
             {
                 list_test_creat(creat_len);
             }
             break;
        case pint_list:
                //delet_node();
             spi_flash_test_write();
             break;
        case spi_flash_t:
            //sflash_io_init();
           //spi_flash_test_read1();
            //spi_flash_test();
            #if 1
            {
                FIL fil;       /* File object */
                FRESULT fr;    /* FatFs return code */
                /* Open a text file */
                fr = f_open(&fil, "0:message.txt", FA_CREATE_ALWAYS|FA_WRITE|FA_CREATE_NEW|FA_OPEN_ALWAYS);
                
                eat_trace("f_open -->f_open:%d",fr);

                /* Close the file */
                f_close(&fil);
            }
            #endif
            break;
        case fatfs_creat_t:
             //spi_flash_test_read2();
             sflash_file_create(spi_flash_fs_test);
             #if usb_debug_printf 
             eat_trace("sflash_file_create -->TEST");
             #endif
             break;
        case fatfs_write_t:
            {
             uint8_t write_buf[64]= "spi flash test one sector !" ;
             sflash_file_write(spi_flash_fs_test,0,write_buf,strlen(write_buf));
             sflash_file_write(spi_flash_fs_test,strlen(write_buf),write_buf,strlen(write_buf));
             #if usb_debug_printf 
             eat_trace("spi_flash_fs_test -->fatfs_write_t");
             #endif
             }
             break;
        case fatfs_read_t:
             {
             uint8_t read_buf[64]={0};
             sflash_file_read(spi_flash_fs_test,0,read_buf,strlen("spi flash test one sector !"));
             sflash_file_read(spi_flash_fs_test,strlen("spi flash test one sector !"),read_buf+strlen("spi flash test one sector !"),strlen("spi flash test one sector !"));
             #if usb_debug_printf 
             eat_trace("spi_flash_test -->fatfs_read_t:%s",read_buf);
             #endif
             }
             break;
       case fatfs_getsize_t:
            {
                 uint32_t size_data=0;
                 sflash_get_file_size(spi_flash_fs_test,&size_data);
                 #if usb_debug_printf 
                 eat_trace("sflash_get_file_size -->size_data:%d",size_data);
                 #endif
            }
            break;
        case take_picture_t:
             #if usb_debug_printf 
             eat_trace("usb_debug_proc_inf --> take_picture test !");
             #endif
             worning_sms_send("天线断开");
             
             check_md5_encryption("b43f99182bc5a38","11334566");
             //tcp_take_pictures_server(Main_Link.get_ip_statue,data_buff,10);//通道上一命名未执行完不接受下一条
             break;
        default:
            break;
    }
}


