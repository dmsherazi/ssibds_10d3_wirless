/****************************************************
 * Include Files
 ****************************************************/ 
 
#include "receive_data_proc_list.h"


 /****************************************************
* External variables declaration
*****************************************************/
    
struct list_head  first_ip_data_pack;//第一IP数据链表
struct list_head  second_ip_data_pack;//第二IP数据链表




/*****************************************************
* Local variables declaration(Static)
******************************************************/

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: receive_pro_list_init
 *  Description: 初始化tcp接收链表
 *  Parameters : 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
void receive_pro_list_init(void)
{
    INIT_LIST_HEAD(&first_ip_data_pack);
    INIT_LIST_HEAD(&second_ip_data_pack);
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: receive_pro_list_add
 *  Description: z增加TCP的内容
 *  Parameters : 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
void receive_pro_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt)
{
   receive_data_pack_queue *mem_prt;
   if(is_list_enful(list_parm,tcp_recive_list_max) ==EAT_TRUE)
   {
       mem_prt = (receive_data_pack_queue *)list_entry(list_parm->next,receive_data_pack_queue,re_list);
       list_del(list_parm->next);
       eat_mem_free(mem_prt);
   }
   mem_prt =(receive_data_pack_queue *)eat_mem_alloc(sizeof(receive_data_pack_queue));
   memset(mem_prt,0,sizeof(receive_data_pack_queue));
   mem_prt->data_len = data_cnt;
   memcpy(mem_prt->tcp_data_buf,buff,data_cnt);
   tail_list_add(&(mem_prt->re_list),list_parm,tcp_recive_list_max);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: receive_pro_list_del
 *  Description: 删除节点
 *  Parameters : 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
void receive_pro_list_del(int16_t socket_id,struct list_head *list_parm)
{
    receive_data_pack_queue *mem_prt=NULL;
    if(list_empty(list_parm)!=EAT_TRUE)
    {
        mem_prt = (receive_data_pack_queue *)list_entry(list_parm->next,receive_data_pack_queue,re_list);
		analyze_tcp_type_setup(socket_id,mem_prt->tcp_data_buf,mem_prt->data_len);
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

