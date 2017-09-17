
#ifndef _LIST_TEST_H
    #define _LIST_TEST_H
#define list_max_lenght 20

   #include "eat_interface.h"


   typedef struct
   {
       uint8_t data_test[128];
       uint32_t data_len;
       struct list_head list_data;
   }test_list_data;

extern struct list_head  test_list;
extern void eat_mem_test(void);
extern void delet_node(void);
extern void list_test_creat(const uint8_t cnt);

    #endif
