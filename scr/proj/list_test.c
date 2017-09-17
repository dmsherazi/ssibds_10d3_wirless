
#include "list_test.h"



struct list_head  test_list;

void list_test_creat(const uint8_t cnt)
{
    uint8_t data_buff[128]={0};
    test_list_data *mem_prt=NULL;
    sprintf(data_buff,"list_test->data_cnt:%u!",cnt);
    if(is_list_enful(&test_list,list_max_lenght)!= EAT_TRUE)
    {
        mem_prt =(test_list_data *)eat_mem_alloc(sizeof(test_list_data));
        
        mem_prt->data_len = cnt;
        memcpy(mem_prt->data_test,data_buff,strlen(data_buff));
        tail_list_add(&(mem_prt->list_data),&test_list,list_max_lenght);
    }
}

void delet_node(void)
{
    test_list_data *mem_prt=NULL;
    if(list_empty(&test_list)!=EAT_TRUE)
    {
        mem_prt = list_entry(test_list.next,test_list_data,list_data);
        #if list_debug 
        eat_trace("delet_node--> data_len:%d.",mem_prt->data_len);
        eat_trace("delet_node--> data_test:%s.",mem_prt->data_test);
        #endif
        list_del(test_list.next);
        eat_mem_free(mem_prt);
    }
    else
    {
        #if list_debug 
        eat_trace("delet_node--> list is empty.");
        #endif
    }
}
