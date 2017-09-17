/********************************************************************
 *                Copyright co. Ltd.                  
 *---------------------------------------------------------------------
 * FileName      :   xxxxxx.c
 * version       :   0.1
 * Description   :   Founction of this Source of C;
 * Authors       :   j.x.b
 * Notes         :
 *---------------------------------------------------------------------
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.10  2016/3/9, j.x.b originally.
 *--------------------------------------------------------------------
 * File Description
 *
 *--------------------------------------------------------------------
 ********************************************************************/

#ifndef THREAD_RECIVE_808DATA_H
   #define THREAD_RECIVE_808DATA_H



/********************************************************************
 * Include Files
 ********************************************************************/
       #include"eat_interface.h"


/********************************************************************
* Macros
 ********************************************************************/
 
/**********************平台下发ID*****************/
#define Tcp_Rev_Comm_ID     (uint16_t)0x8001//终端应对通用ID
#define TCP_VDR_Seting_ID   (uint16_t)0x8103//参数设置
#define remoto_update_processor (uint16_t)0x8108//升级
#define remote_sleep_setting (uint16_t)0xd105  //休眠指令

#define TCP_VDR_ChaXun_ALL  (uint16_t)0x8104//查询全部参数
#define TCP_VDR_ChaXun_ID   (uint16_t)0x8106//查询指定参数
#define tcp_request_param_replay (uint16_t)0x0104//查询终端参数应答

#define  Tcp_Down_Though_ID    (uint16_t)0x8900
#define Tcp_Location_Ask_ID    (uint16_t)0x8201 //位置查询
#define tcp_location_tracking_id (uint16_t)0x8202
/****************人工报警确认****************/
#define man_aram_qveren (uint16_t)0x8203

/******************终端发送ID*********************/
#define Tcp_VDR_Common_Replay_ID (uint16_t)0x0001 //终端通用应答
#define Tcp_Pass_Though_ID    (uint16_t)0x0900//透传

/****************8.23 文本信息下发****************/
#define txt_information_issued    (uint16_t)0x8300 //平台文本下发

/****************行驶记录数据采集****************/
#define drive_record_iquire_id (uint16_t)0x8700
/****************行驶记录参数设置****************/
#define drive_record_set_id (uint16_t)0x8701


/***立即拍照***/
#define tcp_take_pictures_id (uint16_t)0x8801
/***拍照应答***/
#define tcp_take_pictures_id_replay (uint16_t)0x8800

/***录音开始命令***/
#define tcp_voice_record (uint16_t)0x8804
/*存储多媒体数据检索*/
#define tcp_media_search_id (uint16_t)0x8802
/***储存多媒体长传ID***/
#define save_tcp_media_update (uint16_t)0x8803

/***单条多媒体检索***/
#define tcp_signel_media_search_id (uint16_t)0x8805

/***终端属性***/
#define tcp_vdr_attributes_id   0x8107
/*查询终端属性应答*/
#define tcp_vdr_attributes_replay_id   0x0107

/***车辆控制***/
#define tcp_car_contrl_id   0x8500
/*车辆控制应答*/
#define tcp_car_contrl_id_replay_id   0x0500

/*终端控制*/
#define tcp_vdr_ctrl_id 0x8105


/******************跛行控制ID*********************/
#define lock_car_contrl_id 0xF001
#define lock_car_request_id 0xF003
#define lock_car_demarcate_id 0xF006//标定指令
#define lock_car_heart_beat_id 0xF002//心跳指令


/********************************************************************
* Types
********************************************************************/


/********************************************************************
* Extern Variables (Extern /Global)
********************************************************************/


/********************************************************************
* Local Variables:  STATIC
********************************************************************/
    
/********************************************************************
* External Functions declaration
********************************************************************/
extern void remote_message_rev_data_pack(uint8_t *Source_Buff,uint16_t Re_Lens,struct list_head *data_pack);//user1
extern void analyze_tcp_type_setup(int16_t socket,int8_t *pSource,uint16_t dat_cnt);
extern void Tcp_VDR_Common_Replay(char_t socket,uint16_t Sern_num,uint16_t Data_ID,uint8_t Result);

/********************************************************************
* Local Function declaration
********************************************************************/
     
#endif

    





