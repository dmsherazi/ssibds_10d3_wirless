#if 0
/*********************************************Copyright (c)***********************************************
**                              Wuhan South Sagittarius Integration Co.,Ltd.
**
**                                        http://www.dfssi.com.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           dbc_decode.c
** Last modified Date:  2016-02-15
** Last Version:        v1.00
** Descriptions:        The dbc_decode() function
**
**--------------------------------------------------------------------------------------------------------
** Created by:          c.c. 
** Created date:        2016-02-15
** Version:             v1.00
** Descriptions:        dbc_decode function for can protocol stack project
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         c.c.
** Modified date:       2016-02-15
** Version:             v1.10
** Descriptions:
** Checked by:          c.c.(2016-02-15)
** Rechecked by:
*********************************************************************************************************/
/***********************************************(Include)************************************************/

#include "dbc_decode.h"

/***********************************************(Define)*************************************************/
#define MESSAGE_ADDRESS 0x30
#define MESSAGE_SIZE 0x10
#define MESSAGE_TYPE_ADDRESS 0x35
#define MESSAGE_TYPE_NEXT 0x10
#define SIGNAL_BIN_ADDRESS 0x37
#define SIGNAL_ADDRESS 0x800
#define SIGANL_SIZE 0x30
#define SIGNAL_NUM_ADDRESS 0x36

/***********************************************(Typedef)************************************************/


/***********************************************(Static parameters)**************************************/
DBC_FORMAT dbf;
MESSAGE_FORMAT maf;
SIGNAL_FORMAT sgf;
uint8_t message_type[256];//0:StandardCAN  1:ExtendedCAN  2:reserved  3:J1939PG

/***********************************************(Static Function)****************************************/
uint8_t line_decode(uint8_t *buffer, uint8_t *struct_id);
uint8_t message_signal_line_resolve(uint8_t *block_buffer, uint32_t block_buffer_length, uint16_t block_id);
uint8_t algorithm_analysis(uint8_t *block_buffer, uint32_t block_buffer_length);

/*********************************************************************************************************
* Function Name  : dbc_decode
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*********************************************************************************************************/
uint8_t dbc_decode(char const* _DBCFileName, char const* _BINFileName)
{
	uint32_t message_seek = MESSAGE_ADDRESS,
		signal_seek = SIGNAL_ADDRESS,
		message_type_seek = MESSAGE_TYPE_ADDRESS,
		signal_num_seek = SIGNAL_NUM_ADDRESS,
		signal_bin_seek = SIGNAL_BIN_ADDRESS;
	uint32_t bw;
	uint8_t signal_num[256] = {0}, message_num = 0;
	uint8_t Readbuf[256];
	uint8_t *th,i;
	int *fout, *fin;
	uint8_t struct_id,ret,times = 0;

	fout = eat_fs_Open(_DBCFileName, FS_READ_ONLY);
	//fin = fopen(_BINFileName, "w b+");
	if (fout != NULL&&fin != NULL)
	{
//		printf("%s open succeed!\r\n", _DBCFileName);
//		printf("%s open succeed!\r\n", _BINFileName);

		do
		{
			th = fgets((char *)Readbuf, sizeof(Readbuf), (FILE *)fout);
			if ((!ferror(fout)) && (th != NULL))
			{
				ret = line_decode(Readbuf, &struct_id);
				if (!ret&&struct_id == 0x02)
				{
					message_seek += MESSAGE_SIZE;
					if (bw == sizeof(MESSAGE_FORMAT) / sizeof(uint8_t))
					{
//						printf("message frame write succeed!\r\n");
					}
					else
					{
//						printf("message frame write failed!\r\n");
						return 3;//write message frame error
					}
					uint32_t address = signal_seek;
					fseek(fin, signal_bin_seek, SEEK_SET);
					bw = fwrite(&address, sizeof(uint8_t), sizeof(uint32_t) / sizeof(uint8_t), fin);
					signal_bin_seek += MESSAGE_SIZE;
					if (bw == sizeof(uint32_t) / sizeof(uint8_t))
					{
//						printf("message frame write succeed!\r\n");
					}
					else
					{
//						printf("message frame write failed!\r\n");
						return 3;//write message frame error
					}
					message_num++;
				}
				else if (!ret&&struct_id == 0x03)
				{
					fseek(fin, signal_seek, SEEK_SET);
					bw = fwrite(&sgf, sizeof(uint8_t), sizeof(SIGNAL_FORMAT) / sizeof(uint8_t), fin);
					signal_seek += SIGANL_SIZE;
					if (bw == sizeof(SIGNAL_FORMAT) / sizeof(uint8_t))
					{
//						printf("signal frame write succeed!\r\n");
					}
					else
					{
//						printf("signal frame write failed!\r\n");
						return 4;//write signal frame error
					}
					signal_num[message_num-1]++;
				}
				else if (ret)
				{
//					return 3;//line decode error!
				}
				times = 0;
			}
			else
			{
				times++;
			}
		} while ((th != NULL) && (times <= 3));

		if (times>3)
		{
			return 2;//read line times out!
		}
		fseek(fin, 0, SEEK_SET);
		bw = fwrite(&dbf, sizeof(uint8_t), sizeof(DBC_FORMAT) / sizeof(uint8_t), fin);
		if (bw == sizeof(DBC_FORMAT) / sizeof(uint8_t))
		{
//			printf("message_type frame write succeed!\r\n");
		}
		else
		{
//			printf("message_type frame write failed!\r\n");
			return 5;//write message_type frame error
		}
		for (i = 0; i < dbf.message_num; i++)
		{
			fseek(fin, message_type_seek, SEEK_SET);
			bw = fwrite(&(message_type[i]), sizeof(uint8_t), 1, fin);
			message_type_seek += MESSAGE_TYPE_NEXT;
			if (bw == 1)
			{
//				printf("message_type frame write succeed!\r\n");
			}
			else
			{
//				printf("message_type frame write failed!\r\n");
				return 5;//write message_type frame error
			}
			fseek(fin, signal_num_seek, SEEK_SET);
			bw = fwrite(&(signal_num[i]), sizeof(uint8_t), 1, fin);
			signal_num_seek += MESSAGE_TYPE_NEXT;
			if (bw == 1)
			{
//				printf("signal_num frame write succeed!\r\n");
			}
			else
			{
//				printf("signal_num frame write failed!\r\n");
				return 6;//write signal_num frame error
			}
		}
		/* close file */
		fclose(fout);
		fclose(fin);
		return 0;//success
	}
	else
	{
//		printf("%s or %s open failed!\r\n", _DBCFileName, _BINFileName);
		return 1;//open file failed!
	}
}

/*********************************************************************************************************
* Function Name  : dbc_decode
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*********************************************************************************************************/
uint8_t line_decode(uint8_t *buffer, uint8_t *struct_id)
{
	uint8_t title_buffer[5];
	uint8_t block_buffer[256] = { 0 };
	uint8_t ret, block_id = 0;
	uint16_t i, seek_point;
	uint32_t strlenth, block_buffer_length;

//	printf("%s\r\n", buffer);

	if (buffer[0] == 0x20 || buffer[0] == 0x09)
		seek_point = 1;
	else
		seek_point = 0;

	strlenth = strlen(buffer);
	if (strlenth <= seek_point)
		return 1;//length error!

	for (i = 0; i < 5; i++)
		title_buffer[i] = buffer[i];

	if (strstr(title_buffer, "BU_:") != NULL)
	{
		memset(&dbf, 0, sizeof(DBC_FORMAT));
		*struct_id = 0x01;
	}
	else if (strstr(title_buffer, "BO_ ") != NULL)
	{
		memset(&maf, 0, sizeof(MESSAGE_FORMAT));
		*struct_id = 0x02;
	}
	else if (strstr(title_buffer, "SG_ ") != NULL)
	{
		memset(&sgf, 0, sizeof(SIGNAL_FORMAT));
		*struct_id = 0x03;
	}
	else if (strstr(title_buffer, "BA_ ") != NULL)
	{
		sscanf(buffer, "%*[^\"]\"%[^\"]", block_buffer);
		if (strstr(block_buffer, "HS") != NULL)
		{
			dbf.can_baudrate[0] = 1;
			dbf.can_baudrate[1] = buffer[strlenth - 3] - 0x30;
		}
		else if (strstr(block_buffer, "MS") != NULL)
		{
			dbf.can_baudrate[2] = 2;
			dbf.can_baudrate[3] = buffer[strlenth - 3] - 0x30;
		}
		else if (strstr(block_buffer, "LS") != NULL)
		{
			dbf.can_baudrate[4] = 3;
			dbf.can_baudrate[5] = buffer[strlenth - 3] - 0x30;
		}
		else if (strstr(block_buffer, "ES") != NULL)
		{
			dbf.can_baudrate[6] = 4;
			dbf.can_baudrate[7] = buffer[strlenth - 3] - 0x30;
		}
		else if (strstr(block_buffer, "VFrameFormat") != NULL)
		{
			message_type[dbf.message_num] = buffer[strlenth - 3] - 0x30;
			dbf.message_num++;
		}
		*struct_id = 0x04;
	}
	else
	{
		return 2;//not need line!
	}
	for (i = seek_point; i <= strlenth; i++)
	{
		if (buffer[i] == 0x20 || buffer[i] == 0x0A)
		{
			sscanf(buffer + seek_point, "%s", block_buffer);
			block_buffer_length = i - seek_point;
			block_id++;
//			printf("%s\r\nid=%X\r\n", block_buffer, block_id);
			if (*struct_id == 0x02 || struct_id[0] == 0x03)
			{
				ret = message_signal_line_resolve(block_buffer, block_buffer_length, (*struct_id << 8) | block_id);
				if (!ret)
					return 3;//resolve line error!
			}
// 			if (*struct_id == 0x01 && block_id != 0x01)
// 			{
//  				for (j = 0; j < block_buffer_length; j++)
//  					dbf.node_name[dbf.node_num][j] = block_buffer[j];
//  				dbf.node_num++;
// 			}
			seek_point = i + 1;
		}
	}
	return 0;
}

uint8_t message_signal_line_resolve(uint8_t *block_buffer, uint32_t block_buffer_length, uint16_t block_id)
{
	uint16_t i, j, ret, seek_point = 0;
	uint8_t buffer[20] = { 0 };

	switch (block_id)
	{
	case 0x0202:
		maf.message_id.simple_id = (uint32_t)atoll(block_buffer) - 0x80000000;
		return 1;
// 	case 0x0203:
// 		for (i = 0; i < block_buffer_length - 1; i++)
// 			maf.message_name[i] = block_buffer[i];
// 		return 1;
	case 0x0204:
		maf.message_size = atoi(block_buffer);
		return 1;
// 	case 0x0205:
// 		for (i = 0; i < block_buffer_length; i++)
// 			maf.send_ecu_name[i] = block_buffer[i];
// 		return 1;
	case 0x0302:
		for (i = 0; i < block_buffer_length; i++)
		{
			if (block_buffer[i] == 0xA1 && block_buffer[i + 1] == 0xF2)
			{
				for (j = 0; j < i; j++)
					buffer[j] = block_buffer[j];
				sgf.signal_id.simple_id = atoi(buffer);
				seek_point = i + 2;
			}
		}
		if (block_buffer[block_buffer_length - 2] == 0x5F)//_T or _W
		{
			if (block_buffer[block_buffer_length - 1] == 0x57)
				sgf.multiplexer_indicator = 1;
			else
				sgf.multiplexer_indicator = 2;
//  			for (i = 0; i < block_buffer_length - 2; i++)
//  				sgf.signal_name[i] = block_buffer[seek_point + i];
		}
		else if (block_buffer[block_buffer_length - 3] == 0x5F)//_TW
		{
			sgf.multiplexer_indicator = 3;
//  			for (i = 0; i < block_buffer_length - 3; i++)
//  				sgf.signal_name[i] = block_buffer[seek_point + i];
		}
		else//NULL
		{
			sgf.multiplexer_indicator = 0;
//  			for (i = 0; i < block_buffer_length; i++)
//  				sgf.signal_name[i] = block_buffer[seek_point + i];
		}
		return 1;
	case 0x0304:
		ret = algorithm_analysis(block_buffer, block_buffer_length);
		if (!ret)
			return 0;
		return 1;
// 	case 0x0305:
// 		for (i = 0; i < block_buffer_length - 2; i++)
// 			sgf.uint[i] = block_buffer[i + 1];
// 		return 1;
// 	case 0x0306:
// 		for (i = 0; i < block_buffer_length; i++)
// 			sgf.receive_ecu_name[i] = block_buffer[i];
// 		return 1;
	default:
		return 2;//not need line
	}
}

uint8_t algorithm_analysis(uint8_t *block_buffer, uint32_t block_buffer_length)
{
	uint8_t i;
	uint8_t errsv = errno;
	uint8_t buffer1[30], buffer2[30], buffer3[30];

	memset(&buffer1, 0, 30);
	memset(&buffer2, 0, 30);
	memset(&buffer3, 0, 30);
	sscanf(block_buffer, "%[^@]", buffer1);
	if (errsv == -1)
	{
//		printf("extract start_bit and signal_size error!\r\n");
		return 0;
	}
	sscanf(buffer1, "%[^|]", buffer2);
	sgf.start_bit = atoi(buffer2);
	sscanf(buffer1, "%*[^|]|%s", buffer3);
	sgf.signal_size = atoi(buffer3);

	memset(&buffer1, 0, 30);
	memset(&buffer2, 0, 30);
	memset(&buffer3, 0, 30);
	sscanf(block_buffer, "%*[^(](%[^)]", buffer1);
	if (errsv == -1)
	{
//		printf("extract factor and offset error!\r\n");
		return 0;
	}
	sscanf(buffer1, "%[^,]", buffer2);
	sgf.factor.simple_factor = atof(buffer2);
// 	for (i = 0; i < 8; i++)
// 	{
// 		printf("%X ", sgf.factor.bin_factor[i]);
// 	}
	sscanf(buffer1, "%*[^,],%s", buffer3);
	sgf.offset.simple_offset = atof(buffer3);
//	for (i = 0; i < 8; i++)
//	{
//		printf("%X ", sgf.offset.bin_offset[i]);
//	}

	memset(&buffer1, 0, 30);
	memset(&buffer2, 0, 30);
	memset(&buffer3, 0, 30);
	sscanf(block_buffer, "%*[^[][%[^]]", buffer1);
	if (errsv == -1)
	{
//		printf("extract minimum and maximum error!\r\n");
		return 0;
	}
	sscanf(buffer1, "%[^|]", buffer2);
	sgf.minimum.simple_minimum = atof(buffer2);
	sscanf(buffer1, "%*[^|]|%s", buffer3);
	sgf.maximum.simple_maximum = atof(buffer3);

	for (i = 0; i < block_buffer_length; i++)
	{
		if ((block_buffer[i] == 0x40) && (i <= block_buffer_length - 1))
		{
			sgf.bit_order = block_buffer[i + 1] - 0x30;
			sgf.value_type = block_buffer[i + 2];
		}
// 		else
// 		{
// 			printf("extract bit_order and value_type error!\r\n");
// 			return 0;
// 		}
	}
	return 1;
}

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	 /* Infinite loop */
	while (1)
	{
		/*do nothing*/
	}
}
#endif

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
#endif
