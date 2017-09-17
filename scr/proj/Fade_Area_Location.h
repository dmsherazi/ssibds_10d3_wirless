
#ifndef _FADE_AREA_LOCATION_
	#define _FADE_AREA_LOCATION_


#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

#define Fade_Area_Data_MAX 2001
#define Location_data_save_offest 4

extern uint16_t fade_location_head;//文件头
extern uint16_t fade_location_tail;//文件尾

extern uint16_t second_fade_location_head;
extern uint16_t second_fade_location_tail;//文件尾

extern void File_Fade_Area_Location_Send(void);
extern void Second_File_Fade_Area_Location_Send(void);

extern void Clear_Location_data_save(void);
extern void second_Clear_Location_data_save(void);

extern void Save_Fade_Location_data(void);

extern void Temp_Fade_Area_Location_Send(void);
extern void Second_Temp_Fade_Area_Location_Send(void);

extern void Second_Fade_Area_Location_Save(void);
extern void fade_data_init(void);
extern void fade_data_read(void);

 #endif

