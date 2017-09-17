#if 0
/*********************************************Copyright (c)***********************************************
**                              Wuhan South Sagittarius Integration Co.,Ltd.
**
**                                        http://www.dfssi.com.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           dbc_decode.h
** Last modified Date:  2016-02-15
** Last Version:        v1.00
** Descriptions:        The dbc_decode() define
**
**--------------------------------------------------------------------------------------------------------
** Created by:          c.c.
** Created date:        2016-02-15
** Version:             v1.00
** Descriptions:        dbc_decode define for can protocol stack project
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         c.c.
** Modified date:       2016-02-15
** Version:             v1.10
** Descriptions:
** Checked by:          c.c.(2016-02-15)
** Rechecked by:
*********************************************************************************************************/
#ifndef __DBC_DECODE_H
#define __DBC_DECODE_H


/***********************************************(Include)************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***********************************************(Define)*************************************************/


/***********************************************(Typedef)************************************************/
typedef struct signal_foramt
{
//	uint8_t signal_name[64];
	union
	{
		uint16_t simple_id;
		uint8_t bin_id[2];
	}signal_id;
	uint8_t multiplexer_indicator;
	uint8_t start_bit;
	uint8_t signal_size;
	uint8_t bit_order;//0:little endian  1:big endian
	uint8_t value_type;//+:unsigned  -:signed
	uint8_t reserve;
	union
	{
		double simple_factor;
		uint8_t bin_factor[8];
	}factor;
	union
	{
		double simple_offset;
		uint8_t bin_offset[8];
	}offset;
	union
	{
		double simple_minimum;
		uint8_t bin_minimum[8];
	}minimum;
	union
	{
		double simple_maximum;
		uint8_t bin_maximum[8];
	}maximum;
//	uint8_t uint[64];
//	uint8_t receive_ecu_name[64];
}SIGNAL_FORMAT;

typedef struct message_format {
	union
	{
		uint32_t simple_id;
		uint8_t bin_id[4];
	}message_id;
	uint8_t message_size;
	uint8_t message_type;//0:StandardCAN  1:ExtendedCAN  2:reserved  3:J1939PG
	uint8_t signal_num;
}MESSAGE_FORMAT;

typedef struct dbc_format {
	uint8_t can_baudrate[8];//0:50K  1:100K  2:125K  3:250K  4:500K  5:1M
//	uint8_t node_name[64][64];
//	uint8_t node_num;
	uint8_t message_num;
}DBC_FORMAT;

/***********************************************(Extern parameters)**************************************/


/***********************************************(Extern Function)****************************************/
uint8_t dbc_decode(char const* _DBCFileName, char const* _BINFileName);


#endif
/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
#endif
