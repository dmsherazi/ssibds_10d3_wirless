#if !defined (__EAT_TYPE_H__)
#define __EAT_TYPE_H__


/********************************************************************
* Macros
 ********************************************************************/
 
#include "public_interface.h"






#define EAT_NULL 0
typedef enum  {
    EAT_FALSE,
    EAT_TRUE
} eat_bool;

#define NULL 0

#define FALSE 0
#define TRUE 1
//typedef unsigned char bool;

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short int u16;
typedef signed short int s16;

typedef unsigned int u32;
typedef signed int s32;

typedef unsigned long long u64;
typedef signed long long s64;


/*enum value for user task*/
typedef enum {
    EAT_USER_0,
    EAT_USER_1,
    EAT_USER_2,
    EAT_USER_3,
    EAT_USER_4,
    EAT_USER_5,
    EAT_USER_6,
    EAT_USER_7,
    EAT_USER_8,
    EAT_USER_NUM
} EatUser_enum;
#endif
