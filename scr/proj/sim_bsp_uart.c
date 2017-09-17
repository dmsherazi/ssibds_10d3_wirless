/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Eat_uart.c
 *  Description: All the funchion of UART
 *        
 *       author: J.x.b  create at 2014-5-12
 * =====================================================================================
 */
#include "sim_bsp_uart.h"
/********************************************************************
 * Local Variables:  STATIC
 ********************************************************************/
static const EatUart_enum eat_uart_app=EAT_UART_1;
static const EatUart_enum eat_uart_debug =EAT_UART_USB;
static const EatUart_enum eat_uart_at =EAT_UART_NUM;//EAT_UART_1

static uint8_t bsp_mode_rev_buf[EAT_MODE_RX_BUF_LEN_MAX];//内核模块接收BUFFER
static uint8_t bsp_uart_rev_buf[EAT_UART_RX_BUF_LEN_MAX];//串口1结束buffer


/***************************************************************************
 * globle Functions
  Functions：;sim_bsp_init_uart0 
 ***************************************************************************/
void sim_bsp_init_uart1(void)
{
	EatUartConfig_st uart_config;
	if(eat_uart_open(eat_uart_app) == EAT_FALSE)
    {
		#if sim_bsp_uart_debug_printf
		eat_trace("eat_uart_open(eat_uart_app)--> open fail.");
		#endif
    }
	else
    {
        if( EAT_UART_USB != eat_uart_app )//usb port not need to config
        {
            uart_config.baud = EAT_UART_BAUD_115200;
            uart_config.dataBits = EAT_UART_DATA_BITS_8;
            uart_config.parity = EAT_UART_PARITY_NONE;
            uart_config.stopBits = EAT_UART_STOP_BITS_1;
            if(EAT_FALSE == eat_uart_set_config(eat_uart_app, &uart_config))
            {
				#if sim_bsp_uart_debug_printf
        		eat_trace("eat_uart_set_config(eat_uart_app)--> config fail.");
        		#endif
            }
           eat_uart_set_send_complete_event(eat_uart_app, EAT_FALSE);
        }
    }
}

void sim_bsp_init_uart2(void)
{
	EatUartConfig_st uart_config;
    if(eat_uart_open(EAT_UART_2) == EAT_FALSE)
    {
		#if sim_bsp_uart_debug_printf
        eat_trace("eat_uart_open(EAT_UART_2)--> open fail.");
        #endif
    }
	else
    {
        uart_config.baud =EAT_UART_BAUD_115200;// EAT_UART_BAUD_38400
        uart_config.dataBits = EAT_UART_DATA_BITS_8;
        uart_config.parity = EAT_UART_PARITY_NONE;
        uart_config.stopBits =EAT_UART_STOP_BITS_1;// EAT_UART_STOP_BITS_2
        if(EAT_FALSE == eat_uart_set_config(EAT_UART_2, &uart_config))
        {
			#if sim_bsp_uart_debug_printf
            eat_trace("eat_uart_set_config(EAT_UART_2)--> config fail.");
            #endif
        }
        eat_uart_set_send_complete_event(EAT_UART_2,EAT_FALSE);   
    }
}

void sim_bsp_init_dbg(void)//usb uart3
{
	EatUartConfig_st cfg =
    {
        EAT_UART_BAUD_115200,
        EAT_UART_DATA_BITS_8,
        EAT_UART_STOP_BITS_1,
        EAT_UART_PARITY_NONE
    };
   eat_uart_set_at_port(eat_uart_at);
   eat_uart_set_debug(eat_uart_debug);
   if( EAT_UART_USB == eat_uart_debug)
   {
        eat_uart_set_debug_config(EAT_UART_DEBUG_MODE_UART, NULL);
   }
   else
   {
        eat_uart_set_debug_config(EAT_UART_DEBUG_MODE_UART, &cfg);
   }
}
/***************************************************************************
 * Local Functions
 Functions：内核接收APP发送的数据;
 ***************************************************************************/
void mdm_rx_proc(const EatEvent_st* event)
{
	 int16_t ret=0;
	 memset(bsp_mode_rev_buf,0,EAT_MODE_RX_BUF_LEN_MAX);
     ret = eat_modem_read(bsp_mode_rev_buf, EAT_MODE_RX_BUF_LEN_MAX);
	 if(ret != 0)
	 {
		bsp_mode_rev_buf[ret] = '\0';
		#if sim_bsp_uart_debug_printf
        eat_trace("mdm_rx_proc --> string %s.",bsp_mode_rev_buf);
        #endif
	 }
	 mdm_rx_proc_cb(bsp_mode_rev_buf);//内核函数处理
}

/***************************************************************************
 * Local Functions
 Functions：接收串口数据收入存入SSI_rx_buf;
 ***************************************************************************/
 void sim_bsp_read_uart_data(const EatEvent_st* event)
 {
    static uint8_t data_cnt=0;
 	uint16_t ret=0;
 	uint8_t ii=0;
    EatUart_enum uart = event->data.uart.uart;
    memset(bsp_uart_rev_buf,0,EAT_UART_RX_BUF_LEN_MAX);
    ret = eat_uart_read(uart,bsp_uart_rev_buf,EAT_UART_RX_BUF_LEN_MAX);
    #if 0//sim_bsp_uart_debug_printf
    eat_trace("sim_bsp_read_uart_data --> string cnt %u.",ret);
    #endif  
    if(ret != 0)//主板通信通信
    {
        bsp_uart_rev_buf[ret] = '\0';
        if(uart==EAT_UART_1)
        {   
            #if 0//sim_bsp_uart_debug_printf
            eat_trace("sim_bsp_read_uart1_data --> string cnt %u.",ret);
        	HexToAsicc(bsp_uart1_rev_buf,ret);
            #endif	
        	uart1_receive_integrated_frame_proc(bsp_uart_rev_buf,ret);//通信数据
        }
        else if(uart==EAT_UART_2)
        {
            #if 0//sim_bsp_uart_debug_printf
            eat_trace("sim_bsp_read_uart_data --> string cnt %u.",ret);
            eat_trace("sim_bsp_read_uart2_data --> data hex !.");
			HexToAsicc(bsp_uart_rev_buf,ret);
            #endif
            if(*(bsp_uart_rev_buf)==0x7F)
            {
                #if 1//sim_bsp_uart_debug_printf
                eat_trace("sim_bsp_read_uart_data7e --> string cnt %u.",ret);
                eat_trace("sim_bsp_read_uart2_data7e --> data hex !.");
                HexToAsicc(bsp_uart_rev_buf,ret);
                #endif
                data_cnt+=1;
                for(ii=1;ii<ret;ii++)
                {
                    if(*(bsp_uart_rev_buf+ii)==0x7F)
                    {
                        data_cnt+=1;
                    }
                    else
                    {
                        break;
                        data_cnt=0;
                    }
                }
                if(data_cnt>=3)
                {
                    data_cnt=0;
                    task_msg_send(EAT_USER_0,EAT_USER_1,uart_485_data_send);
                }
                //test_result_send(0x01,0x00);
            }
            else
            {
                if(data_cnt!=0)data_cnt=0;
			    camer_rev_one_packge(bsp_uart_rev_buf,ret);//通信数据
			}
        }
        else if(uart==EAT_UART_USB)
        {
            #if sim_bsp_uart_debug_printf
            eat_trace("sim_bsp_read_uart3_data --> string %s.",ret);
            #endif
            usb_debug_proc_inf(bsp_uart_rev_buf);
        }
    }
 }
 
