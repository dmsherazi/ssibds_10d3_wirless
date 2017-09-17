
#ifndef _LOCK_CAR_STRATEGY_H_
    #define _LOCK_CAR_STRATEGY_H_

    #include <stdio.h>
    #include <string.h>
    #include"eat_interface.h"

    
    #define lock_car_debug_pirntf 1
    #define remote_lock_car_send 0x8A
    #define lock_car_data_save_board_rev 0x8B
extern void lock_car_strategy_rev_proc(uint8_t *pscr,uint16_t data_len);
extern void lock_car_strategy_send_proc(uint16_t lock_car_id,uint8_t *lock_data,uint16_t data_len);
extern void lock_car_strategy_sleep_send_proc(void);
extern void lock_car_strategy_send(void);
extern void lock_car_strategy_save_board_data(uint8_t *rve_data);

#endif
    
