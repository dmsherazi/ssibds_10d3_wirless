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

#ifndef SYSTEM_PARAM_PROC_H
   #define SYSTEM_PARAM_PROC_H



/********************************************************************
 * Include Files
 ********************************************************************/
 
   #include "eat_interface.h"

/********************************************************************
* Macros
 ********************************************************************/
#define File_SystemParaSave	L"C:\\File_SystemParaSave.bin" //终端参数

#define max_can_id 20

#define DYNAMIC_MEM_SIZE 1024*400

/********************************************************************
* Types
********************************************************************/
/*注册信息结构体*/
typedef struct//注册
{
     int16_t provincial_id;
     int16_t county_id;
     uint8_t terminl_id[8];
     uint8_t plate_coror;
     uint8_t vehicle_vin[18];
     uint8_t plate_number[13];//车牌号
     uint8_t vehicle_classification[13];//车辆分类
     
     uint8_t manufacture_id[8];
     uint8_t terminl_model[20];
     uint8_t iccd[10];
     uint8_t hardware_vison[20];
     uint8_t softerware_vison[36];
     uint16_t vehicle_puse_num;
     uint8_t first_authentication_id[64];
     uint8_t second_authentication_id[64];
}terminl_registe_struct;

/*位置信息上报参数结构体*/
typedef struct
{
    uint32_t policy; //位置汇报策略，0：定时汇报；1：定距汇报；2：定时和定距汇报
    uint32_t scheme;/*位置汇报方案，0：根据 ACC 状态； 1：根据登录状态和 ACC 状态，
                          先判断登录状态，若登录再根据 ACC 状态*/
    uint32_t not_signed_frze;     //驾驶员未登录汇报时间间隔，单位为秒（s），>0
    uint32_t sleeping_frze;         //0x0027 DWORD 休眠时汇报时间间隔，单位为秒（s），>0
    uint32_t emergency_alarm_frze;  //0x0028 DWORD 紧急报警时汇报时间间隔，单位为秒（s），>0
    uint32_t default_frze;          //0x0029 DWORD 缺省时间汇报间隔，单位为秒（s），>0
    uint32_t night_sleep_frze;      //晚上给你休眠时间间隔
    uint8_t time_night[4];        //晚上时间
    uint16_t sleep_delay;
      
    uint32_t default_distance;//0x002C DWORD 缺省距离汇报间隔，单位为米（m），>0
    uint32_t not_signed_distance;  //0x002D DWORD 驾驶员未登录汇报距离间隔，单位为米（m），>0
    uint32_t sleeping_distance;    //0x002E DWORD 休眠时汇报距离间隔，单位为米（m），>0
    uint32_t emergency_alarm_distance; //0x002F DWORD 紧急报警时汇报距离间隔，单位为米（m），>0
}location_reporting_param_strcut;

/*手机号码参数结构体*/
typedef struct
{
    uint8_t monitor_platform[14];//0x0040 STRING 监控平台电话号码
    uint8_t terminal_restart[14];//0x0041 STRING 复位电话号码，可采用此电话号码拨打终端电话让终端复位
    uint8_t terminal_format[14];//0x0042 STRING恢复出厂设置电话号码，可采用此电话号码拨打终端电话让终端恢复出厂设置
    
    uint8_t monitor_platform_sms[14];//0x0043 STRING 监控平台 SMS 电话号码
    uint8_t terminal_sms_send[14];   //0x0044 STRING 接收终端 SMS 文本报警号码

    uint32_t answer_policy;//0x0045 DWORD 终端电话接听策略，0：自动接听；1：ACC ON 时自动接听，OFF 时手动接听
    uint32_t every_longest_call_time;//0x0046 DWORD 每次最长通话时间，单位为秒（s），0 为不允许通话，0xFFFFFFFF 为不限制
    uint32_t mounth_longest_call_time;//0x0047 DWORD 当月最长通话时间，单位为秒（s），0 为不允许通话，0xFFFFFFFF 为不限制

    uint8_t monitor_telephone[14];//0x0048 STRING 监听电话号码
    uint8_t supervision_platform[14];//监管平台特权短信号码
    
    uint8_t sos_telephone[14];//0x0048 STRING 监听电话号码
    
}phone_number_param_strcut;


/*拍照参数联合体
bit:0  摄像通道 1 定时/定距拍照开关标志 0：不允许； 1：允许
bit:1  摄像通道 2 定时/定距拍照开关标志 0：不允许； 1：允许
bit:2  摄像通道 3 定时/定距拍照开关标志 0：不允许； 1：允许
bit:3  摄像通道 4 定时/定距拍照开关标志 0：不允许； 1：允许
bit:4  摄像通道 5 定时/定距拍照开关标志 0：不允许； 1：允许
bit:5-7 保留
bit:8  摄像通道 1 定时/定距拍照存储标志 0：存储； 1：上传
bit:9  摄像通道 2 定时/定距拍照存储标志 0：存储； 1：上传
bit:10 摄像通道 3 定时/定距拍照存储标志 0：存储； 1：上传
bit:11 摄像通道 4 定时/定距拍照存储标志 0：存储； 1：上传
bit:12 摄像通道 5 定时/定距拍照存储标志 0：存储； 1：上传
bit:13-15 保留
bit:16 
    定时时间单位:
    0：秒，当数值小于 5 秒时，终端按 5 秒处理；
    1：分。
    定距距离单位:
    0：米，当数值小于 100 米时，终端按 100 米处理；
    1：公里。
bit:17-31 定时时间间隔/定距距离间隔 收到参数设置或重新启动后执行*/

typedef union _photo_param_struct_
{
  uint32_t photo_data;
  struct 
  {
    uint32_t swtich_chanel_1: 1;              
    uint32_t swtich_chanel_2: 1;              
    uint32_t swtich_chanel_3: 1;              
    uint32_t swtich_chanel_4: 1;              
    uint32_t swtich_chanel_5: 1;                
    uint32_t swtich_chanel_rev: 3; 

    uint32_t save_chanel_1: 1;              
    uint32_t save_chanel_2: 1;              
    uint32_t save_chanel_3: 1;              
    uint32_t save_chanel_4: 1;              
    uint32_t save_chanel_5: 1;                
    uint32_t save_chanel_rev: 3; 

    uint32_t time_uinit:1;//单位
    uint32_t time_fre:15; //间隔
  }param_bit;
}photo_param_struct;

/*CAN相关参数结构体*/
/*CAN 总线 ID 单独采集设置：
uint32_t can_id_collect_frze
bit63-bit32 表示此 ID 采集时间间隔(ms)，0 表示不采集；

uint32_t can_id()
bit31 表示 CAN 通道号，0：CAN1，1：CAN2；
bit30 表示帧类型，0：标准帧，1：扩展帧；
bit29 表示数据采集方式，0：原始数据，1：采集区间的计算值；
bit28-bit0 表示 CAN 总线 ID。*/
typedef struct
{
    uint32_t can_id_collect_frze;//0x0100 DWORD CAN 总线通道 1 采集时间间隔(ms)，0 表示不采集
    uint32_t can_id;
    
}can_id_param_struct;

/*CAN相关参数结构体*/
typedef struct
{
    uint8_t update_socket;//0x01 MAIN 0x02SECOND
    uint8_t canid_num;
    uint32_t can1_time_interval;//0x0100 DWORD CAN 总线通道 1 采集时间间隔(ms)，0 表示不采集
    uint16_t can1_upload_interval;//0x0101 WORD CAN 总线通道 1 上传时间间隔(s)，0 表示不上传
    
    uint32_t can2_time_interval;//0x0102 DWORD CAN 总线通道 2 采集时间间隔(ms)，0 表示不采集
    uint16_t can2_upload_interval;//0x0103 WORD CAN 总线通道 2 上传时间间隔(s)，0 表示不上传
    can_id_param_struct can_id_param_array[max_can_id];
}can_collect_param_struct;


/*系统参数状态共同体*/
typedef union _system_param_save_struct_
{
  uint32_t save_statue;
  struct 
  {
    uint32_t wether_first_power_on: 1;   //是否第一次上电           
    uint32_t wether_system_update:1;     //是否系统升级
    uint32_t wether_can_orign_updata:1;     //是否上传CAN原始数据
    uint32_t wether_sleep_wake:1;     //是否记忆休眠状态
    uint32_t secondip_open_switch:1;
    uint32_t viech_kg:2;//载重
	uint32_t ip1_lock:1;//IP1锁定
	uint32_t ip2_lock:1;//IP2锁定标志
    uint32_t wether_rev:23;
  }statue;
}param_save_sruct;


/*设置参数结构体*/
typedef struct
{
    /*状态参数共同体*/
    param_save_sruct param_save; 

   /*锁车数据保存结构体*/
   lock_car_save_struct lock_car_save;//锁车数据保存
   
   /*注册参数信息*/
   terminl_registe_struct terminl_registe; //注册信息
   TerminlPhone_Struct SaveTerminlPhone; //终端手机号

   /*超时参数*/
   uint32_t HeartBeatUpdataFre;//心跳频率 //0x0001
   Timeout_Struct SaveTcpRetransTimes;//TCP消息超时参数0x0002 0x0003
   Timeout_Struct SaveUdpRetransTimes;//UDP消息超时参数0x0004 0x0005
   Timeout_Struct SaveSmsRetransTimes;//sms消息超时参数0x0006 0x0007

   /*连接APN参数*/
   GprsBearerParameter_Struct SaveMainBeareParameter; //主APN参数0x0010 0x0011 0x0012 
   GprsBearerParameter_Struct SaveBackupBeareParameter;//备份

   /*主备服务器连接参数*/
   ServerParameter_Struct SaveServerParameterArray[SERVERSAVENUM];// 主要备份服务器参数

   /*IC卡认证服务器连接参数*/
   ICServerParameter_Struct ic_server_parameter; //IC域名参数。

   /*上报参数*/ //0x0020--->
   location_reporting_param_strcut location_reporting_param;
   
   /*拐点电子围栏参数*/ //0x0030--->0x0031
   uint32_t inflection_angle_passup;            //0x0030 DWORD 拐点补传角度，<180
   uint16_t electronic_fence_radius;            //0x0031 WORD 电子围栏半径（非法位移阈值），单位为米

   /*手机号码相关参数*/ 
   phone_number_param_strcut phone_number_param;

   /*报警屏蔽参数*/ 
   Alarm_Inf_Struct alarm_mask;//0x0050 DWORD报警屏蔽字，与位置信息汇报消息中的报警标志相对应，相应位为 1则相应报警被屏蔽

   /*短信报警开关参数*/ 
   Alarm_Inf_Struct alarm_sms_swtich;//0x0051 DWORD 报警发送文本 SMS 开关，与位置信息汇报消息中的报警标志相对应，相应位为 1 则相应报警时发送文本 SMS

   /*报警拍照开关参数*/ 
   Alarm_Inf_Struct alarm_camer_switch;//0x0052 DWORD报警拍摄开关，与位置信息汇报消息中的报警标志相对应，相应位为1 则相应报警时摄像头拍摄
   /*报警拍照储存参数*/ 
   Alarm_Inf_Struct alarm_camer_upload;//0x0053 DWORD报警拍摄存储标志，与位置信息汇报消息中的报警标志相对应，相应位为 1 则对相应报警时拍的照片进行存储，否则实时上传

   /*关键报警参数*/ 
   Alarm_Inf_Struct linchpin_alarm;//0x0054 DWORD 关键标志，与位置信息汇报消息中的报警标志相对应，相应位为 1 则对相应报警为关键报警

   /*超速参数*/ 
   uint32_t max_speed;        //0x0055 DWORD 最高速度，单位为公里每小时（km/h）
   uint32_t speeding_duration;//0x0056 DWORD 超速持续时间，单位为秒（s）

   /*疲劳驾驶参数*/ 
   uint32_t continuous_driving_threshold;//0x0057 DWORD 连续驾驶时间门限，单位为秒（s）
   uint32_t day_continuous_driving_threshold;//0x0058 DWORD 当天累计驾驶时间门限，单位为秒（s）
   uint32_t min_rest_period;//0x0059 DWORD 最小休息时间，单位为秒（s）
   uint32_t max_parking_time;//最长停车时间，单位为秒（s）

   /*预警参数*/ 
   uint16_t speed_warning_dif; //0x005B WORD 超速报警预警差值，单位为 1/10Km/h
   uint16_t fatigue_dring_warning_dif; //0x005C WORD 疲劳驾驶预警差值，单位为秒（s），>0

   /*碰撞参数*/ 
   uint16_t collision_warning_param;/*0x005D WORD
                                                碰撞报警参数设置：
                                                b7-b0： 碰撞时间，单位 4ms；
                                                b15-b8：碰撞加速度，单位 0.1g，设置范围在：0-79 之间，默认为
                                                10。*/
   uint16_t rollover_param;//0x005D WORD0x005E WORD侧翻报警参数设置：侧翻角度，单位 1 度，默认为 30 度。

   /*拍照参数*/
   photo_param_struct time_photo_param;//定时拍照
   photo_param_struct distance_photo_param;//定距拍照

   /*图像参数*/
   uint32_t image_quality;    //0x0070 DWORD 图像/视频质量，1-10，1 最好
   uint32_t image_brightness; //0x0071 DWORD 亮度，0-255
   uint32_t image_contrast;   //0x0072 DWORD 对比度，0-127
   uint32_t image_saturation; //0x0073 DWORD 饱和度，0-12
   uint32_t image_chroma;     //0x0074 DWORD 色度，0-255
   
   /*里程参数*/
   uint32_t mileage;//0x0080 DWORD 车辆里程表读数，1/10km
                    /*省市域参数见注册参数*/
   /*定位参数*/
   uint8_t location_mode;/*0x0091 BYTE
                                GNSS 定位模式，定义如下：
                                bit0，0：禁用 GPS 定位， 1：启用 GPS 定位；
                                bit1，0：禁用北斗定位， 1：启用北斗定位；
                                bit2，0：禁用 GLONASS 定位， 1：启用 GLONASS 定位；
                                bit3，0：禁用 Galileo 定位， 1：启用 Galileo 定位。*/
   uint8_t baud_rate;/*0x0092 BYTE
                            GNSS 波特率，定义如下：
                            0x00：4800；0x01：9600；
                            0x02：19200；0x03：38400；
                            0x04：57600；0x05：115200。*/
   uint8_t location_out_frze;/*GNSS 模块详细定位数据输出频率，定义如下：
                                      0x00：500ms；0x01：1000ms（默认值）；
                                      0x02：2000ms；0x03：3000ms；
                                      0x04：4000ms。*/ 
                                     
   uint32_t detail_location_get_frze;//GNSS 模块详细定位数据采集频率，单位为秒，默认为 1
   uint8_t detail_location_upload_mode;/*GNSS 模块详细定位数据上传方式：
                                                0x00，本地存储，不上传（默认值）；
                                                0x01，按时间间隔上传；
                                                0x02，按距离间隔上传；
                                                0x0B，按累计时间上传，达到传输时间后自动停止上传；
                                                0x0C，按累计距离上传，达到距离后自动停止上传；
                                                0x0D，按累计条数上传，达到上传条数后自动停止上传。*/
   uint32_t detail_location_upload_setting;/*GNSS 模块详细定位数据上传设置：
                                                    上传方式为 0x01 时，单位为秒；
                                                    上传方式为 0x02 时，单位为米；
                                                    上传方式为 0x0B 时，单位为秒；
                                                    上传方式为 0x0C 时，单位为米；
                                                    上传方式为 0x0D 时，单位为条。*/  
   /*CAN参数*/                                                
   can_collect_param_struct can_collect_param;

}system_param_struct;
/********************************************************************
* Extern Variables (Extern /Global)
********************************************************************/
extern system_param_struct system_param;

/********************************************************************
* Local Variables:  STATIC
********************************************************************/
    
/********************************************************************
* External Functions declaration
********************************************************************/
extern uint8_t app_dynic_mem_test[DYNAMIC_MEM_SIZE];

extern void system_para_save_at_once(void);
extern void sys_param_init_proc(void);
    
/********************************************************************
* Local Function declaration
********************************************************************/
     
#endif

    





