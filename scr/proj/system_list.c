

#include "system_list.h"
/*******************************************************************
** ������:     list_init
** ��������:   ��ʼ������
** ����:       [in]  Lp:        ����
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
** ������:     get_list_size
** ��������:   ��������
** ����:       [in]  Lp:        ����
** ����:       uin32_t:    ������
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
** ������:     is_list_enful
** ��������:   �����Ƿ�Ϊ��
** ����:       [in]  Lp :���� ,������󳤶�
** ����:       true:    ��
**             false:   ����
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
** ������:     is_list_empty
** ��������:   �����Ƿ�Ϊ��
** ����:       [in]  Lp :����
** ����:       true:   ��
**             false:   ����
********************************************************************/
int list_empty(struct list_head *head)
{
  return head->next == head;
}
/*******************************************************************
** ������:     list_add
** ��������:   ������β�����½��
** ����:       [in]  Lp lp_size:�����½ڵ�
** ����:       true:   ��
**             false:   ����
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
** ������:     list_del
** ��������:   ��ͷ�ڵ�ɾ������ͷ�ڵ�
** ����:       [in]  Lp :����
** ����:       true:    �ɹ�
**             false:   ʧ��
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
