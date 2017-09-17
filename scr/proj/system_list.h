/********************************************************************************
**
** 文件名:     system_list.h
** 版权所有:   j.x.b
** 文件描述:   实现链表数据结构
**
*********************************************************************************
**             修改历史记录
**===============================================================================
**| 日期       | 作者   |  修改记录
**===============================================================================
**| 2016/07/26 | 姜晓博 |  创建该文件
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
** 函数名:     list_init
** 函数描述:   初始化链表
** 参数:       [in]  Lp:        链表
** 返回:       true:    成功
**             false:   失败
********************************************************************/
void INIT_LIST_HEAD(struct list_head *list);


/*******************************************************************
** 函数名:     is_list_enful
** 函数描述:   链表是否为满
** 参数:       [in]  Lp :链表 ,链表最大长度
** 返回:       true:    满
**             false:   不满
********************************************************************/
bool_t is_list_enful(struct list_head *list,uint32_t list_size_max);

/*******************************************************************
** 函数名:     is_list_empty
** 函数描述:   链表是否为空
** 参数:       [in]  Lp :链表
** 返回:       true:   空
**             false:   不空
********************************************************************/
int list_empty(struct list_head *head);
/*******************************************************************
** 函数名:     list_add
** 函数描述:   在链表尾插入新结点
** 参数:       [in]  Lp lp_size:链表，新节点
** 返回:       true:   空
**             false:   不空
********************************************************************/
extern void tail_list_add(struct list_head *new_node,struct list_head *list,uint32_t list_max);

/*******************************************************************
** 函数名:     list_del
** 函数描述:   从头节点删除更新头节点
** 参数:       [in]  Lp :链表
** 返回:       true:    成功
**             false:   失败
********************************************************************/
extern  bool_t list_del(struct list_head *entry);
extern void list_add(struct list_head *new_nod, struct list_head *head);

#endif

