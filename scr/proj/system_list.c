

#include "system_list.h"
/*******************************************************************
** 函数名:     list_init
** 函数描述:   初始化链表
** 参数:       [in]  Lp:        链表
********************************************************************/
void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}
static  void __list_add(struct list_head *new_nod,struct list_head *prev,struct list_head *next)
{
	next->prev = new_nod;
	new_nod->next = next;
	new_nod->prev = prev;
	prev->next = new_nod;
}
void list_add(struct list_head *new_nod, struct list_head *head)
{
	__list_add(new_nod, head, head->next);
}
void list_add_tail(struct list_head *new_nod, struct list_head *head)
{
	__list_add(new_nod, head->prev, head);
}

/*******************************************************************
** 函数名:     get_list_size
** 函数描述:   求链表长度
** 参数:       [in]  Lp:        链表
** 返回:       uin32_t:    链表长度
********************************************************************/
static uint32_t get_list_size(struct list_head *list)
{
    uint32_t list_cnt =0;
    struct list_head *pos;
    list_for_each(pos,list)
    {
         list_cnt++; 
    }
    return list_cnt;
}
/*******************************************************************
** 函数名:     is_list_enful
** 函数描述:   链表是否为满
** 参数:       [in]  Lp :链表 ,链表最大长度
** 返回:       true:    满
**             false:   不满
********************************************************************/
bool_t is_list_enful(struct list_head *list,uint32_t list_size_max)
{
    if(list_size_max == get_list_size(list))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/*******************************************************************
** 函数名:     is_list_empty
** 函数描述:   链表是否为空
** 参数:       [in]  Lp :链表
** 返回:       true:   空
**             false:   不空
********************************************************************/
int list_empty(struct list_head *head)
{
  return head->next == head;
}
/*******************************************************************
** 函数名:     list_add
** 函数描述:   在链表尾插入新结点
** 参数:       [in]  Lp lp_size:链表，新节点
** 返回:       true:   空
**             false:   不空
********************************************************************/
void tail_list_add(struct list_head *new_node,struct list_head *list,uint32_t list_max)
{
   uint32_t list_size=0;
   list_size = get_list_size(list);
   if(list_size==list_max)
   {
       #if system_list_debug_printf
       eat_trace("tail_list_add --> list_size:%d",list_size);
       #endif
   }
   else
   {
       list_add_tail(new_node,list);
       #if system_list_debug_printf
       eat_trace("tail_list_add --> add_ok");
       #endif
   }
}
/*******************************************************************
** 函数名:     list_del
** 函数描述:   从头节点删除更新头节点
** 参数:       [in]  Lp :链表
** 返回:       true:    成功
**             false:   失败
********************************************************************/
static  void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}
bool_t list_del(struct list_head *entry)
{
    uint32_t list_size=0;
    list_size = get_list_size(entry);
    if(list_size == 0)
    {
        #if system_list_debug_printf
        eat_trace("list_del --> list_size:%d",list_size);
        #endif
        return 0;
    }
    else
    {
        
        #if system_list_debug_printf
        eat_trace("list_del --> delet_nod:%d",list_size);
        #endif
        __list_del(entry->prev, entry->next);
    }
}
