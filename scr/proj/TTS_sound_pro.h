#ifndef _TTS_SOUND_PRO_H
#define _TTS_SOUND_PRO_H

#include "eat_interface.h"

/*语音播报调试开关*/
#define tts_tone_debug_printf 1
/*语音播报调试开关*/
#define txt_save_num_max 50

/*文本信息储存文件夹*/
#define txt_record_file L"D:\\txt_record"

/*文本信息储存*/
#define txt_data_record L"D:\\txt_record\\txt_data_record.bin"//导航信息数量

/***txt信息个数查询***/
#define send_txt_cnt_inquire 0x80

/***txt信息内容***/
#define send_txt_infor_inquire 0x81


/*文本信息标志位含义*/
typedef  union _remote_txt_data//平台下发数据结构
{
	unsigned char txt_rev;
	struct
		{
		    unsigned char urgent : 1;  // 1：紧急
			unsigned char rsv_0 : 1;  //保留
			unsigned char displaly : 1; // 1：终端显示器显示
			unsigned char tone_play : 1; // 1：终端 TTS 播读
			unsigned char advert_display :1;// 1：广告屏显示
			unsigned char navigation_can_error :1;// 0：中心导航信息，1：CAN 故障码信息
			unsigned char rsv : 2;        //保留位 			
		}txt_bit;
}remote_txt_data;

/*文本记录结构体*/
typedef struct
{
    uint8_t navigation_num;// 导航信息个数
    uint8_t can_error_num;// CAN 故障 txt个数
    uint8_t navig_time[50][6];
    uint8_t can_time[50][6];
}txt_record_struct;

/*文本信息记录*/
extern txt_record_struct txt_record;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: txt_data_record_init
 *  Description: txt
 *        input: 初始化txt记录
 *       author: Jumping create at 2015-12-16
 * =====================================================================================
 */
extern void txt_data_record_init(void);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: read_txt_data_record
 *  Description: 读状态
 *        input: 初始化txt记录
 *       author: Jumping create at 2015-12-16
 * =====================================================================================
 */
extern void read_txt_data_record(void);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: tts_tone_envlope_play
 *  Description: 封装语音播报。
 *        input: scr_tone:需要播报文本报文指针
 *       author: Jumping create at 2015-12-16
 * =====================================================================================
 */
extern void tts_tone_envlope_play(uint8_t *scr_tone);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_Tcp_Register
 *  Description:  处理接收到的通用应答
 *  Parameters : Temp_data_buff ;接收到的有效数据指针
 *             :  tempLens 长度
 *			   :  Sn 流水号
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
extern void remote_txt_data_proc(uint8_t *remote_point,uint16_t data_len);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: txt_data_cnt_inquire
 *  Description:  查询TXT数据个数
 *  Parameters : cmd_data ;接收到的有效数据命令
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
extern void txt_data_cnt_inquire(uint8_t cmd_data);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: txt_data_cnt_inquire
 *  Description:  查询TXT数据内容
 *  Parameters :  cmd data :0x01，导航，0x02:CAN故障。TXT_nuM数据编号
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void txt_data_infor_inquire(uint8_t cmd_data,uint8_t txt_num);

#endif
