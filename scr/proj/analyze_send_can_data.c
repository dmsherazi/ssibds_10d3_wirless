
/****************************************************
 * Include Files
****************************************************/ 
 
 #include "analyze_send_can_data.h"

 /****************************************************
* External variables declaration
*****************************************************/

analysised_can_param_struct can_data_send;
/***待发送解析CAN数据链表***/
struct list_head analysised_can_list;
/***待发送解析CAN数据链表***/
uint16_t can_data_send_serial_num=0;
uint16_t second_can_data_send_serial_num=0;

uint16_t can_update_fre_zero=30;
uint16_t can_update_fre_one=2;

/*****************************************************
* Local variables declaration(Static)
******************************************************/
const Can_data_struct bit_array[64]={{0,7},{0,6},{0,5},{0,4},{0,3},{0,2},{0,1},{0,0},\
                                     {1,7},{1,6},{1,5},{1,4},{1,3},{1,2},{1,1},{1,0},  
                                     {2,7},{2,6},{2,5},{2,4},{2,3},{2,2},{2,1},{2,0},
                                     {3,7},{3,6},{3,5},{3,4},{3,3},{3,2},{3,1},{3,0},
                                     {4,7},{4,6},{4,5},{4,4},{4,3},{4,2},{4,1},{4,0},
                                     {5,7},{5,6},{5,5},{5,4},{5,3},{5,2},{5,1},{5,0},
                                     {6,7},{6,6},{6,5},{6,4},{6,3},{6,2},{6,1},{6,0},
                                     {7,7},{7,6},{7,5},{7,4},{7,3},{7,2},{7,1},{7,0}};

/*****************************************************************************
* Function :analyze_can_data
* Description:  analyze can data form dbc array 
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
/*****************************************************************************
*二分查找 
*算法思想：1、将数组排序(从小到大)；2、每次跟中间的数mid比较，如果相等可以直接返回， 
*如果比mid大则继续查找大的一边，否则继续查找小的一边。 
*输入：排序好的数组 - sSource[]，数组大小 - array_size，查找的值 - key 
*返回：找到返回相应的位置，否则返回-1 
*****************************************************************************/
int binary_search(DbcDataSave_Struct *sSource,int16_t low,int16_t high,uint32_t key)  
{  
    int16_t mid=0;
    if (low>high)  
        return -1;  
  
    mid=(low+high)/2;  
    if (((sSource->DbcParmSaveArray[mid].message_id)&0x1FFFFFFF) == key)  
        return mid;  
  
    if (((sSource->DbcParmSaveArray[mid].message_id)&0x1FFFFFFF) > key)
        return binary_search(sSource,low,mid-1,key);  
    else  
        return binary_search(sSource,mid+1,high,key);  
  
}

void analyze_can_data(uint16_t id_num,uint8_t *data_buf)
{
    uint8_t temp_cnt=0;
    uint8_t *p_point=NULL;
    uint32_t temp_message=0;
    uint32_t temp_can_data=0;//
    uint32_t raw_value=0;
    float32_t physical_value;
    uint8_t temp_signal_cnt=0;
    uint8_t temp_bit_cnt=0;
    int post_offest=0;
    uint8_t temp_uint8_buffer[4]={0};
    p_point = data_buf;
    can_data_send.param_num =0;
    for(temp_cnt=0;temp_cnt<id_num;temp_cnt++)
    {
       Big_End_BytetoDword(p_point,&temp_message);
       p_point+=4;
       temp_message =(uint32_t)(temp_message&0x1FFFFFFF);
       post_offest=binary_search(&DbcDataSave,0,(int16_t)(DbcDataSave.save_message_num),temp_message);  
       if(post_offest>=0)
       {
           for(temp_signal_cnt=0;temp_signal_cnt<DbcDataSave.DbcParmSaveArray[post_offest].signal_num;temp_signal_cnt++)
           {
                raw_value=0;
                physical_value=0;
                temp_can_data=0;
                Big_End_WordtoByte(DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].signal_id,can_data_send.param_buff+6*can_data_send.param_num);
                for(temp_bit_cnt=0;temp_bit_cnt<DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].signal_size;temp_bit_cnt++)
                {
                    if((((*(p_point+bit_array[DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].start_bit+temp_bit_cnt].byte_num))>>(bit_array[DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].start_bit+temp_bit_cnt].offest))&0x01)==EAT_TRUE)
                    {
                        temp_can_data|=(0x00000001<<(DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].signal_size-temp_bit_cnt-1));
                    }
                    else
                    {
                        temp_can_data&=(~(0x00000001<<(DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].signal_size-temp_bit_cnt-1)));
                    }
                }
                if(DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].signal_size > 8)
                {
                    if(DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].byte_order==0)// dbd 中的小端指的是与 ＣＡＮ８ＢＹＴＥ　０－８相反；
                     {
                          temp_can_data = temp_can_data<<(32-DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].signal_size); 
                          Big_End_DwordtoByte(temp_can_data,temp_uint8_buffer);
                          byete_change_int32(EAT_TRUE,temp_uint8_buffer,&raw_value);
                     }
                     else
                     {
                        raw_value=temp_can_data;
                     }
                }
                else
                {
                    raw_value = temp_can_data;
                }
                physical_value =(float32_t)(raw_value*DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].factor + DbcDataSave.DbcParmSaveArray[post_offest].signal_array[temp_signal_cnt].offset);
                FloatToByte(physical_value,can_data_send.param_buff+6*can_data_send.param_num+2);
                can_data_send.param_num +=1;
           }
       }
       else
       {
           #if  APP_TRACE_DBG 
           eat_trace("analyze_can_data -> BinSearch error!!!! ");
           #endif
       }
       p_point+=8;       
    }
}
/*****************************************************************************
* Function :can_data_send_proc
* Description:  update can fade data idle
* Parameters :
* Returns:
* NOTE  extern 
*     Nothing
*****************************************************************************/
void can_data_send_proc(int16_t socket,analysised_can_param_struct *psouce)
{
    uint16_t package_cnt=0;
    uint16_t package_num=0;
    uint16_t package_surpus=0;
    uint8_t  send_data_buf[1024]={0};
    package_surpus = (psouce->param_num)%Max_para_Send;//剩余量
    if(package_surpus!=0)
    {
        package_num = (psouce->param_num)/Max_para_Send + 1;
    }
    else
    {
        package_num = (psouce->param_num)/Max_para_Send ;
    }
    #if APP_TRACE_DBG 
    eat_trace("can_data_send_proc-->package_num:%u.",package_num);
    #endif
    if(package_num > 1)
    {
        for(package_cnt=1;package_cnt<=package_num;package_cnt++)
        {
            if(package_cnt == 1)//第一包
            {
                 memcpy(send_data_buf,psouce->curren_time,6);
                 Big_End_WordtoByte(psouce->param_num,send_data_buf+6);
                 memcpy(send_data_buf+8,psouce->param_buff,One_Package_Max);
                 if(socket==Main_Link.mode_Socket)
                 {
                    Tcp_Message_PacKage_And_Send(socket,&can_data_send_serial_num,tcp_can_data_send_id,send_data_buf,One_Package_Max + 8,EAT_TRUE,package_num,package_cnt);//分包数目
                 }
                 else if(socket==Second_Link.mode_Socket)
                 {
                    Tcp_Message_PacKage_And_Send(socket,&second_can_data_send_serial_num,tcp_can_data_send_id,send_data_buf,One_Package_Max + 8,EAT_TRUE,package_num,package_cnt);//分包数目
                 }
            }
            else if(package_cnt == package_num)//第二包
            {
                memcpy(send_data_buf,(psouce->param_buff)+(package_cnt-1)*One_Package_Max,package_surpus*6);
                if(socket==Main_Link.mode_Socket)
                {
                    Tcp_Message_PacKage_And_Send(socket,&can_data_send_serial_num,tcp_can_data_send_id,send_data_buf,6*package_surpus,EAT_TRUE,package_num,package_cnt);//分包数目
                }
                else if(socket==Second_Link.mode_Socket)
                {
                    Tcp_Message_PacKage_And_Send(socket,&second_can_data_send_serial_num,tcp_can_data_send_id,send_data_buf,6*package_surpus,EAT_TRUE,package_num,package_cnt);//分包数目
                }
            }
            else
            {
                memcpy(send_data_buf,(psouce->param_buff) + (package_cnt-1)*One_Package_Max,One_Package_Max);
                if(socket==Main_Link.mode_Socket)
                {
                    Tcp_Message_PacKage_And_Send(socket,&can_data_send_serial_num,tcp_can_data_send_id,send_data_buf,One_Package_Max,EAT_TRUE,package_num,package_cnt);//分包数目
                }
                else if(socket==Second_Link.mode_Socket)
                {
                    Tcp_Message_PacKage_And_Send(socket,&second_can_data_send_serial_num,tcp_can_data_send_id,send_data_buf,One_Package_Max,EAT_TRUE,package_num,package_cnt);//分包数目
                }
            }
        }
    }
    else
    {
        memcpy(send_data_buf,psouce->curren_time,6);
        Big_End_WordtoByte(psouce->param_num,send_data_buf+6);
        memcpy(send_data_buf+8,psouce->param_buff,6*(psouce->param_num));
        Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&can_data_send_serial_num,tcp_can_data_send_id,send_data_buf,(psouce->param_num)*6+8,EAT_FALSE,0,0);//分包数目
    }
}

/***发送CAN数据***/
void can_send_pro_list_init(void)
{
    INIT_LIST_HEAD(&analysised_can_list);
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: void receive_can_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt)
*  Description:  增加CAN接收数据链表
*  Parameters :  void     
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
* =====================================================================================
*/
void can_send_pro_list_add(struct list_head *list_parm)
{
   analysised_can_param_list *mem_prt;
   if(is_list_enful(list_parm,can_send_list_max) ==EAT_TRUE)
   {
        mem_prt = (analysised_can_param_list *)list_entry(list_parm->next,analysised_can_param_list,re_list);
        list_del(list_parm->next);
        eat_mem_free(mem_prt);
   }
   mem_prt =(analysised_can_param_list *)eat_mem_alloc(sizeof(analysised_can_param_list));
   memcpy(&(mem_prt->analysised_can_param),&can_data_send,sizeof(can_data_send));
   tail_list_add(&(mem_prt->re_list),list_parm,can_send_list_max);
}

void can_send_update_pro(int16_t t_socket,struct list_head *list_parm)
{
    analysised_can_param_list *mem_prt=NULL;
    if(list_empty(list_parm)!=EAT_TRUE)
    {
       mem_prt = (analysised_can_param_list *)list_entry(list_parm->next,analysised_can_param_list,re_list);
       can_data_send_proc(t_socket,&(mem_prt->analysised_can_param));//发送CAN数据 
    }
    else
    {
       #if 0//list_debug 
       eat_trace("receive_pro_list_del--> list is empty.");
       #endif
    }
}

void can_send_pro_list_del(struct list_head *list_parm)
{
    analysised_can_param_list *mem_prt=NULL;
    if(list_empty(list_parm)!=EAT_TRUE)
    {
        mem_prt = (analysised_can_param_list *)list_entry(list_parm->next,analysised_can_param_list,re_list);
        list_del(list_parm->next);
        eat_mem_free(mem_prt);
    }
    else
    {
        #if 0//list_debug 
        eat_trace("receive_pro_list_del--> list is empty.");
        #endif
    }
}
/*****************************************************************************
* Function :idle_update_can_fade_data
* Description:  update can fade data idle
* Parameters :
* Returns:
* NOTE  extern 
*     Nothing
*****************************************************************************/
void idle_update_can_data_proc(int16_t d_socket)
{
    static uint8_t main_link_send_cnt=0;
    static uint8_t second_link_send_cnt=0;
    if(d_socket==Main_Link.mode_Socket)
    {
        if(System_Flag.System_Flag_Bit.MainAutenSucess==EAT_TRUE)
        {
            if(System_Flag.System_Flag_Bit.can_data_send_state==EAT_FALSE)
            {   
               if(main_link_send_cnt!=0)
               {
                  main_link_send_cnt=0;
               }
               can_send_update_pro(d_socket,&analysised_can_list);
               System_Flag.System_Flag_Bit.can_data_send_state=EAT_TRUE;
            }
            else
            {
               main_link_send_cnt++;
               if(main_link_send_cnt == 10)
               {
                  main_link_send_cnt = 0;
                  System_Flag.System_Flag_Bit.can_data_send_state = EAT_FALSE;
               }
            }
            Sim_State.state_byte_struct.can_data_send_bit = ~Sim_State.state_byte_struct.can_data_send_bit;
         }
         else
         {
             if(System_Flag.System_Flag_Bit.can_data_send_state=EAT_TRUE)
             {
                System_Flag.System_Flag_Bit.can_data_send_state=EAT_FALSE;
             }
             if(Sim_State.state_byte_struct.can_data_send_bit=EAT_TRUE)
             {
                Sim_State.state_byte_struct.can_data_send_bit=EAT_FALSE;
             }
         }
    }
    else if(d_socket==Second_Link.mode_Socket)
    {
        if(System_Flag.System_Flag_Bit.SecondAutenSucess==EAT_TRUE)
        {
            if(System_Flag.System_Flag_Bit.second_can_data_send_state==EAT_FALSE)
            {   
                if(main_link_send_cnt!=0)
                {
                   main_link_send_cnt=0;
                }
                can_send_update_pro(d_socket,&analysised_can_list);
                System_Flag.System_Flag_Bit.second_can_data_send_state=EAT_TRUE;
            }
            else
            {
                second_link_send_cnt++;
                if(second_link_send_cnt == 10)
                {
                   second_link_send_cnt = 0;
                   System_Flag.System_Flag_Bit.second_can_data_send_state = EAT_FALSE;
                }
            }
            Sim_State.state_byte_struct.can_data_send_bit = ~Sim_State.state_byte_struct.can_data_send_bit;
         }
         else
         {
            if(System_Flag.System_Flag_Bit.second_can_data_send_state=EAT_TRUE)
            {
               System_Flag.System_Flag_Bit.second_can_data_send_state=EAT_FALSE;
            }
            if(Sim_State.state_byte_struct.can_data_send_bit=EAT_TRUE)
            {
               Sim_State.state_byte_struct.can_data_send_bit=EAT_FALSE;
            }
          }
    }
}

