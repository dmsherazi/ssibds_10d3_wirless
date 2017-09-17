
#ifndef _PUBLIC_INTERFACE_H
  #define _PUBLIC_INTERFACE_H

  #define SYSTEM_DEBUG_TRACE 1


/** 
* list_entry - get the struct for this entry 
* @ptr: the &struct list_head pointer. 
* @type: the type of the struct this is embedded in. 
* @member: the name of the list_struct within the struct. 
*/ 
#define list_entry(ptr, type, member) ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/********************************************************************
* Types
********************************************************************/
typedef char char_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef float float32_t;
typedef double float64_t;
typedef long double float128_t;  
typedef char_t bool_t;

#if SYSTEM_DEBUG_TRACE
#define APP_TRACE_DBG 1  
#define JL_TRACE 0
#define PUBLIC_TRACE 1
#define dbc_analyze 1
#define orign_dbc_printf 1
#define system_msg_printf 0
#define chek_can_data 0
#define inflect_debug_printf 0
#define updata_orgin_printf 0
#define file_sytem_debug 1
#define system_param_debug 1
#define list_debug 0
#else
#define APP_TRACE_DBG 0  
#define JL_TRACE 0
#define PUBLIC_TRACE 1
#define dbc_analyze 0
#define orign_dbc_printf 0
#define system_msg_printf 0
#define chek_can_data 0
#define inflect_debug_printf 0
#define updata_orgin_printf 0
#define file_sytem_debug 0
#define system_param_debug 0
#define list_debug 0

#endif


#define TcpDataQueueLength 10
#define TcpDataBufSize 2048

#define SERVERSAVENUM   4

typedef enum RET_INFOR
{
    RET_NORMAL=0,
    RET_CRC_ERR, 
    RET_FRAME_HIATUS    
}_RET_INFOR; //数据分析结果

/*超时时间、次数*/
typedef struct
{
    uint32_t waiting_time; //频率
    uint32_t timeout_times;//次数
}Timeout_Struct;
/*超时结构体*/
typedef struct
{
    char_t on_off;
    Timeout_Struct time_out_struct;
}TimeoutEvent;

/*服务器列表*/
typedef enum
{
  MAIN_SERVER=0, //第一服务器主
  BACKUP_MAIN_SEVER, //第一服务器备份
  SECOND_SERVER,//第二服务器
  BACKUP_SECOND_SERVER //第二服务器备份
}LinkIndex_Enum;//连接索引 

/*获取IP状态*/
typedef enum
{
  GETIPSUCESS=1,  //获取IP成功
  GETIPWATING,    //等待获取IP结果
  GETIPFAIL       //获得IP失败
}GETIPRESULT_Enum;//获取IP返回

/*主备服务器结构*/
typedef struct 
{
    uint8_t server_domin[68];//域名
    uint32_t server_socket;//端口
    uint32_t udp_server_socket;//udP端口
}ServerParameter_Struct; //链接信息结构体

/*主备服务器结构*/
typedef struct 
{
    uint8_t main_server_domin[68];  //IC域名
    uint8_t backup_server_domin[68];//IC 备份域名
    uint32_t tcp_server_socket;  //TCP端口
    uint32_t udp_server_socket;  //UDP端口
}ICServerParameter_Struct; //链接信息结构体


typedef struct 
{
    char_t get_ip_statue; //获取IP状态
    LinkIndex_Enum link_index; //链接索引
    char_t link_staue;  //链接当前状态
    uint8_t link_ip[4]; //连接IP 4BYTE
    int16_t mode_Socket; //模块内部端口号
}PresentLinkParameter_Struct;


typedef struct
{
   uint8_t beare_apn[32];
   uint8_t beare_user[32];
   uint8_t beare_password[32];
}GprsBearerParameter_Struct;//激活参数



typedef struct
{
    uint8_t terminl_phone_nums[14];//字符串
    uint8_t terminl_phone_numd[6];//BCD码
}TerminlPhone_Struct;//手机号结构图
/*报警信息位置域信储存结构体*/
typedef union _Alarm_Inf_Struct_
{
  uint32_t Alarm_Original;
  struct 
  {
    uint32_t Sos: 1;                       //紧急报警1
    uint32_t Speed_Out: 1;                  //超速2
    uint32_t Fatigue_Driv: 1;              //疲劳驾驶3
    uint32_t Danger: 1;                    //危险预警4
    uint32_t GNSS_Mode: 1;                 //GNSS,模块故障5
    uint32_t Antenna_Discon: 1;            //天线断开6
    uint32_t Antenna_Short_Circuit: 1;     //短路7
    uint32_t Undervoltage: 1;              //欠压8

    uint32_t Power_Down: 1;                //掉电1
    uint32_t LCD_Failure: 1;               //LCD故障2
    uint32_t TTS_Failure: 1;               //tts故障3
    uint32_t Camer_Failure: 1;             //Camer故障4
    uint32_t IC_Failure: 1;                //IC故障5
    uint32_t Speed_Early_Warning: 1;       //超速预警6
    uint32_t Fatigue_Driv_Early_Warning: 1;//疲劳驾驶预警7
    uint32_t SOS_Phone: 1;				  //疲劳驾驶预警7
    uint32_t Rev1: 2;                      //保留812

    uint32_t Day_Sum_Fatigue_Driv: 1;      //当天累计超时驾驶3
    uint32_t Park_Over_Time: 1;            //超时停车；4
    uint32_t Region_In_Out: 1;             //进出区域；5
    uint32_t Route_In_Out: 1;              //进出路线；6
    uint32_t Route_Driving_OutorShort:1;   //路线行驶时间；7
    uint32_t Off_Route: 1;                 //偏离路线；8

    uint32_t VSS_Failure: 1;               //VSS故障； 1
    uint32_t Oil_Mass_Failure: 1;          //油量异常；2
    uint32_t Viotrue_Stolen: 1;            //车辆被偷；3
    uint32_t Illegality_Ignited: 1;        //非法点火; 4
    uint32_t Illegality_Vector: 1;         //非法位移；5
    uint32_t Crashed: 1;                   //碰撞      6   
    uint32_t Rolloved: 1;                  //侧翻      7
    uint32_t Illegality_Open_Door: 1;      //非法开门  8
  }Alarm_Bit;
}Alarm_Inf_Struct;


/*状态信息位置域信储存结构体*/
typedef union _State_Inf_Struct_
{
  uint32_t State_Inf;
  struct 
  {
    uint32_t ACC: 1;                           //ACC开关； 1
    uint32_t Wether_Location : 1;              //是否定位  2
    uint32_t SouthorNorth: 1;                  //南北纬度  3
    uint32_t EastorWest: 1;                    //东西纬度  4
    uint32_t Operation: 1;                     //运营状态  5
    uint32_t Wether_Encrypt_LongLa: 1;         //经纬度加密 6
    uint32_t Rev: 2;                           //保留：7 8

    uint32_t Kg: 2;                            //载重状态；1，2
    uint32_t Oil_Way: 1;                       //油路状态；3
    uint32_t Circuit: 1;                       //电路状态；4
    uint32_t Lock_Door: 1;                     //车门锁    5
    uint32_t Front_Door: 1;                    //前门锁开关6
    uint32_t Mid_Door: 1;                      //中门锁    7
    uint32_t Befor_Door: 1;                    //后门锁    8

    uint32_t Driv_Door: 1;                     //驾驶席门锁1
    uint32_t Rev_Door: 1;                      //自定义    2
    uint32_t GPS: 1;                           //GPS定位   3
    uint32_t BD: 1;                            //BD定位    4
    uint32_t GLONSS: 1;                        //GLONSS    5
    uint32_t Galile: 1;                        //Galile  
    uint32_t usb_sate :1;                      //usb状态
	uint32_t ip1_lock_board_state;             //IP1锁定状态
	uint32_t ip2_lock_board_state;             //IP2锁定状态
    uint32_t State_Rev2: 7;                         //保留
  }State_Bit;
}State_Inf_Struct;


/*超速报警附件信息结构体*/
typedef struct _Over_Speed_Alarm_Add_Struct_
{
	uint8_t Over_Speed_Location_Type; /*超速位置类型 0x00:无特定位置；0x01:圆；0x02:矩形； 0x03:多边形；0x04:路线；*/															    
	uint32_t Area_ID;				 //区域ID
}Over_Speed_Alarm_Add_Struct;//超速附加信息


/*进出区域附件信息结构体*/
typedef struct _Area_InorOut_Inf_Add_Struct_
{
	uint8_t InorOut_Area_Type;	/*进出类型:0x01:圆；0x02:矩形； 0x03:多边形；0x04:路线；*/
	uint32_t InorOut_Area_ID;	//进出区域ID;
	uint8_t InorOut_Area_Action;	//进出区域动作 0x00:进；0x01:出；
}Area_InorOut_Inf_Add_Struct;

/*路段行驶时间不足过长信息结构体*/
typedef struct _Route_Driv_OutorShort_Add_Struct_
{
	uint32_t Route_Driv_OutorShort_ID;		//路段ID
	uint16_t Route_Driv_OutorShort_Time;		//行驶时间;
	uint8_t Route_Driv_OutorShort_Reslut;	//进出区域动作 0x00:进；0x01:出；
}Route_Driv_OutorShort_Add_Struct;

/*扩展信号量*/
typedef union _Extend_Signle_ 
{
	uint32_t Extend_Signle;
	struct
	{
		uint32_t Near_Light: 1;				//近光灯;   1
		uint32_t Far_Light: 1;				//远光灯；	2
		uint32_t Right_Light: 1;				//右转向灯；3
		uint32_t Left_Light: 1;				//左转向灯	4
		uint32_t Bark_Signle: 1;				//刹车信号	5
		uint32_t Reverse_Gear_Signle :1;		//倒档信号	6
		uint32_t Fog_Lamp : 1;				//雾灯信号	7
		uint32_t End_Outline_Market_Lamp : 1;//走廊灯信号	8
		
		uint32_t LoudSpeak_Signle: 1;		//喇叭信号	9
		uint32_t Air_Condition: 1;			//空调		10
		uint32_t Neutral_Gear: 1;			//空挡		11
		uint32_t	Retarder:1;					//缓速器	12
		uint32_t ABS:1;						//			13
		uint32_t Heater:1 ;					//加热器	14
		uint32_t Clutch :1;					//离合器	16
		uint32_t Little_Lamp:1;				//小灯
		
		uint32_t Rev_IO:16;					//保留
	}Extend_Signle_Bit;
}Extend_Signle_Union;

/*IO状态结构体*/
typedef union _IO_State_Union_
{
	uint16_t IO_State;
	struct
	{
		uint16_t Deep_Sleep: 1;				//深度休眠
		uint16_t System_Sleep: 1;			//系统休眠
		uint16_t	IO_Rev: 14;					//IO保留
	}IO_State_Bit;
}IO_State_Union;


/*位置信储存结构体*/
typedef struct 
{
    /*必须位置信息*/ 
    Alarm_Inf_Struct Alarm_Inf;           //报警信息
    State_Inf_Struct VDR_State_Inf;       //状态信息
    uint32_t Location_Latitude;                //纬度
    uint32_t Location_Longitude;               //经度
    uint16_t Location_Altitude;                //高程
    uint16_t Location_Speed;                   //速度信息
    uint16_t Location_Direction;               //方向 0-360
    uint8_t  Curren_Time[6];                   //当前时间
    /*附加位置信息*/ 
	uint32_t Vehicle_Odometer;					//ID 0x01;1/10KM
	uint16_t Power_volate;
	uint16_t Oil_Mass;							//ID 0X02;1/10L
	uint32_t oil_mass_second;//第二油耗
	uint32_t oil_first;//第一IP
	uint16_t Travel_Speed;						//ID 0x03;1/10km/h
	uint16_t Manual_Work_ID;						//ID 0x04;人工确认报警ID;
	Over_Speed_Alarm_Add_Struct Over_Speed_Alarm_Add; 			//ID 0x11;超速报警附件信息
	Area_InorOut_Inf_Add_Struct Area_InorOut_Inf_Add;			//ID 0x12;进出区域附加信息
	Route_Driv_OutorShort_Add_Struct Route_Driv_OutorShort_Add;	//ID 0x13;路段行驶时间不足或者过长
	Extend_Signle_Union Extend_Signle_Add;						//ID 0x25;扩展信号
	IO_State_Union IO_State_Add;								//ID 0x2A;IO状态；
	uint32_t Analog_Signal;											//ID 0x2B;模拟量
	uint8_t Sing_strength;											//ID 0x30;无线信号强度
	uint8_t Satelite_Amount;											//ID 0x31;卫星数量；
}Location_Inf_Struct;


struct list_head 
{
      struct list_head *next, *prev;
};

typedef struct _receive_data_pack_queue
{
   uint16_t data_len;
   uint8_t tcp_data_buf[TcpDataBufSize];//数组头指针
   struct list_head re_list;
}receive_data_pack_queue;//tcp接收缓存
/*多媒体信息附加信息*/
typedef struct 
{
    uint32_t media_id;
    uint8_t media_type;//0：图像；1：音频；2：视频；
    uint8_t media_format;// 0：JPEG；1：TIF；2：MP3；3：WAV；4：WMV；其他保留 
    uint8_t media_event;// 触发类型/*0：平台下发指令；1：定时动作；2：抢劫报警触发；3：碰撞侧翻报警触发；4：门开拍照；5：门关拍照；6：车门由开变关，时速从＜20 公里到超过 20 公里；7：定距拍照；*/
    uint8_t media_chanl_id;// 通道ID
    uint8_t process_type;  //处理类型 0：实时上传；1：保存
    uint8_t picture_size;//图片大小
    uint8_t picture_qulity;//图片质量
    uint8_t media_location[28];//多媒体位置信息
}multimedia_add_struct;
/*录音信息附加参数*/
typedef struct 
{
    uint32_t media_id;//多媒体ID
    uint8_t chanel_id;//多媒体通道
    uint8_t media_event;//多媒体事件
    uint32_t media_time;//多媒体时间
}media_add_struct;
/*锁车数据记录信息结构体*/

typedef struct _lock_car_save_struct_
{
    uint8_t lock_ip;
	uint8_t save_state;		//锁车状态
	uint8_t re_save_state;  //返回状态
	
	uint16_t lock_id;		//锁车ID;
	uint16_t sleep_lock_id; //休眠锁车ID
	
	uint16_t lock_data_len;       //数据长度
	uint16_t re_lock_data_len;    //返回数据长度
	uint16_t sleep_lock_data_len; //休眠锁车数据
	
	uint8_t sleep_data_save[32];//休眠数据存储
	uint8_t lock_data[32];	//锁车数据；
	uint8_t re_lock_data[32];	//锁车数据返回；
	uint8_t lock_car_data_save[64];//存储参数
}lock_car_save_struct;

#endif 
