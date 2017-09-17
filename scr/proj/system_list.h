/********************************************************************************
**
** �ļ���:     system_list.h
** ��Ȩ����:   j.x.b
** �ļ�����:   ʵ���������ݽṹ
**
*********************************************************************************
**             �޸���ʷ��¼
**===============================================================================
**| ����       | ����   |  �޸ļ�¼
**===============================================================================
**| 2016/07/26 | ������ |  �������ļ�
*********************************************************************************/

#ifndef LIST_SYSTEM
    #define LIST_SYSTEM

    #include "eat_interface.h"
    #define system_list_debug_printf 0

    
#define LIST_HEAD_INIT(name) { &(name), &(name) }   
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)
    
/** 
 * list_for_each - iterate over a list 
 * @pos: the &struct list_head to use as a loop counter. 
 * @head: the head for your list. 
 */ 
#define list_for_each(pos, head) for (pos = (head)->next; pos != (head); pos = pos->next) 

/** 
 * list_for_each_safe - iterate over a list safe against removal of list entry 
 * @pos: the &struct list_head to use as a loop counter. 
 * @n: another &struct list_head to use as temporary storage 
 * @head: the head for your list. 
 */ 
#define list_for_each_safe(pos, n, head) for (pos = (head)->next, n = pos->next; pos != (head);pos = n, n = pos->next)

/*******************************************************************
** ������:     list_init
** ��������:   ��ʼ������
** ����:       [in]  Lp:        ����
** ����:       true:    �ɹ�
**             false:   ʧ��
********************************************************************/
void INIT_LIST_HEAD(struct list_head *list);


/*******************************************************************
** ������:     is_list_enful
** ��������:   �����Ƿ�Ϊ��
** ����:       [in]  Lp :���� ,������󳤶�
** ����:       true:    ��
**             false:   ����
********************************************************************/
bool_t is_list_enful(struct list_head *list,uint32_t list_size_max);

/*******************************************************************
** ������:     is_list_empty
** ��������:   �����Ƿ�Ϊ��
** ����:       [in]  Lp :����
** ����:       true:   ��
**             false:   ����
********************************************************************/
int list_empty(struct list_head *head);
/*******************************************************************
** ������:     list_add
** ��������:   ������β�����½��
** ����:       [in]  Lp lp_size:�����½ڵ�
** ����:       true:   ��
**             false:   ����
********************************************************************/
extern void tail_list_add(struct list_head *new_node,struct list_head *list,uint32_t list_max);

/*******************************************************************
** ������:     list_del
** ��������:   ��ͷ�ڵ�ɾ������ͷ�ڵ�
** ����:       [in]  Lp :����
** ����:       true:    �ɹ�
**             false:   ʧ��
********************************************************************/
extern  bool_t list_del(struct list_head *entry);
extern void list_add(struct list_head *new_nod, struct list_head *head);

#endif

