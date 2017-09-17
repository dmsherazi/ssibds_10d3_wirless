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
#define File_SystemParaSave	L"C:\\File_SystemParaSave.bin" //�ն˲���

#define max_can_id 20

#define DYNAMIC_MEM_SIZE 1024*400

/********************************************************************
* Types
********************************************************************/
/*ע����Ϣ�ṹ��*/
typedef struct//ע��
{
     int16_t provincial_id;
     int16_t county_id;
     uint8_t terminl_id[8];
     uint8_t plate_coror;
     uint8_t vehicle_vin[18];
     uint8_t plate_number[13];//���ƺ�
     uint8_t vehicle_classification[13];//��������
     
     uint8_t manufacture_id[8];
     uint8_t terminl_model[20];
     uint8_t iccd[10];
     uint8_t hardware_vison[20];
     uint8_t softerware_vison[36];
     uint16_t vehicle_puse_num;
     uint8_t first_authentication_id[64];
     uint8_t second_authentication_id[64];
}terminl_registe_struct;

/*λ����Ϣ�ϱ������ṹ��*/
typedef struct
{
    uint32_t policy; //λ�û㱨���ԣ�0����ʱ�㱨��1������㱨��2����ʱ�Ͷ���㱨
    uint32_t scheme;/*λ�û㱨������0������ ACC ״̬�� 1�����ݵ�¼״̬�� ACC ״̬��
                          ���жϵ�¼״̬������¼�ٸ��� ACC ״̬*/
    uint32_t not_signed_frze;     //��ʻԱδ��¼�㱨ʱ��������λΪ�루s����>0
    uint32_t sleeping_frze;         //0x0027 DWORD ����ʱ�㱨ʱ��������λΪ�루s����>0
    uint32_t emergency_alarm_frze;  //0x0028 DWORD ��������ʱ�㱨ʱ��������λΪ�루s����>0
    uint32_t default_frze;          //0x0029 DWORD ȱʡʱ��㱨�������λΪ�루s����>0
    uint32_t night_sleep_frze;      //���ϸ�������ʱ����
    uint8_t time_night[4];        //����ʱ��
    uint16_t sleep_delay;
      
    uint32_t default_distance;//0x002C DWORD ȱʡ����㱨�������λΪ�ף�m����>0
    uint32_t not_signed_distance;  //0x002D DWORD ��ʻԱδ��¼�㱨����������λΪ�ף�m����>0
    uint32_t sleeping_distance;    //0x002E DWORD ����ʱ�㱨����������λΪ�ף�m����>0
    uint32_t emergency_alarm_distance; //0x002F DWORD ��������ʱ�㱨����������λΪ�ף�m����>0
}location_reporting_param_strcut;

/*�ֻ���������ṹ��*/
typedef struct
{
    uint8_t monitor_platform[14];//0x0040 STRING ���ƽ̨�绰����
    uint8_t terminal_restart[14];//0x0041 STRING ��λ�绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˸�λ
    uint8_t terminal_format[14];//0x0042 STRING�ָ��������õ绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˻ָ���������
    
    uint8_t monitor_platform_sms[14];//0x0043 STRING ���ƽ̨ SMS �绰����
    uint8_t terminal_sms_send[14];   //0x0044 STRING �����ն� SMS �ı���������

    uint32_t answer_policy;//0x0045 DWORD �ն˵绰�������ԣ�0���Զ�������1��ACC ON ʱ�Զ�������OFF ʱ�ֶ�����
    uint32_t every_longest_call_time;//0x0046 DWORD ÿ���ͨ��ʱ�䣬��λΪ�루s����0 Ϊ������ͨ����0xFFFFFFFF Ϊ������
    uint32_t mounth_longest_call_time;//0x0047 DWORD �����ͨ��ʱ�䣬��λΪ�루s����0 Ϊ������ͨ����0xFFFFFFFF Ϊ������

    uint8_t monitor_telephone[14];//0x0048 STRING �����绰����
    uint8_t supervision_platform[14];//���ƽ̨��Ȩ���ź���
    
    uint8_t sos_telephone[14];//0x0048 STRING �����绰����
    
}phone_number_param_strcut;


/*���ղ���������
bit:0  ����ͨ�� 1 ��ʱ/�������տ��ر�־ 0�������� 1������
bit:1  ����ͨ�� 2 ��ʱ/�������տ��ر�־ 0�������� 1������
bit:2  ����ͨ�� 3 ��ʱ/�������տ��ر�־ 0�������� 1������
bit:3  ����ͨ�� 4 ��ʱ/�������տ��ر�־ 0�������� 1������
bit:4  ����ͨ�� 5 ��ʱ/�������տ��ر�־ 0�������� 1������
bit:5-7 ����
bit:8  ����ͨ�� 1 ��ʱ/�������մ洢��־ 0���洢�� 1���ϴ�
bit:9  ����ͨ�� 2 ��ʱ/�������մ洢��־ 0���洢�� 1���ϴ�
bit:10 ����ͨ�� 3 ��ʱ/�������մ洢��־ 0���洢�� 1���ϴ�
bit:11 ����ͨ�� 4 ��ʱ/�������մ洢��־ 0���洢�� 1���ϴ�
bit:12 ����ͨ�� 5 ��ʱ/�������մ洢��־ 0���洢�� 1���ϴ�
bit:13-15 ����
bit:16 
    ��ʱʱ�䵥λ:
    0���룬����ֵС�� 5 ��ʱ���ն˰� 5 �봦��
    1���֡�
    ������뵥λ:
    0���ף�����ֵС�� 100 ��ʱ���ն˰� 100 �״���
    1�����
bit:17-31 ��ʱʱ����/��������� �յ��������û�����������ִ��*/

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

    uint32_t time_uinit:1;//��λ
    uint32_t time_fre:15; //���
  }param_bit;
}photo_param_struct;

/*CAN��ز����ṹ��*/
/*CAN ���� ID �����ɼ����ã�
uint32_t can_id_collect_frze
bit63-bit32 ��ʾ�� ID �ɼ�ʱ����(ms)��0 ��ʾ���ɼ���

uint32_t can_id()
bit31 ��ʾ CAN ͨ���ţ�0��CAN1��1��CAN2��
bit30 ��ʾ֡���ͣ�0����׼֡��1����չ֡��
bit29 ��ʾ���ݲɼ���ʽ��0��ԭʼ���ݣ�1���ɼ�����ļ���ֵ��
bit28-bit0 ��ʾ CAN ���� ID��*/
typedef struct
{
    uint32_t can_id_collect_frze;//0x0100 DWORD CAN ����ͨ�� 1 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�
    uint32_t can_id;
    
}can_id_param_struct;

/*CAN��ز����ṹ��*/
typedef struct
{
    uint8_t update_socket;//0x01 MAIN 0x02SECOND
    uint8_t canid_num;
    uint32_t can1_time_interval;//0x0100 DWORD CAN ����ͨ�� 1 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�
    uint16_t can1_upload_interval;//0x0101 WORD CAN ����ͨ�� 1 �ϴ�ʱ����(s)��0 ��ʾ���ϴ�
    
    uint32_t can2_time_interval;//0x0102 DWORD CAN ����ͨ�� 2 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�
    uint16_t can2_upload_interval;//0x0103 WORD CAN ����ͨ�� 2 �ϴ�ʱ����(s)��0 ��ʾ���ϴ�
    can_id_param_struct can_id_param_array[max_can_id];
}can_collect_param_struct;


/*ϵͳ����״̬��ͬ��*/
typedef union _system_param_save_struct_
{
  uint32_t save_statue;
  struct 
  {
    uint32_t wether_first_power_on: 1;   //�Ƿ��һ���ϵ�           
    uint32_t wether_system_update:1;     //�Ƿ�ϵͳ����
    uint32_t wether_can_orign_updata:1;     //�Ƿ��ϴ�CANԭʼ����
    uint32_t wether_sleep_wake:1;     //�Ƿ��������״̬
    uint32_t secondip_open_switch:1;
    uint32_t viech_kg:2;//����
	uint32_t ip1_lock:1;//IP1����
	uint32_t ip2_lock:1;//IP2������־
    uint32_t wether_rev:23;
  }statue;
}param_save_sruct;


/*���ò����ṹ��*/
typedef struct
{
    /*״̬������ͬ��*/
    param_save_sruct param_save; 

   /*�������ݱ���ṹ��*/
   lock_car_save_struct lock_car_save;//�������ݱ���
   
   /*ע�������Ϣ*/
   terminl_registe_struct terminl_registe; //ע����Ϣ
   TerminlPhone_Struct SaveTerminlPhone; //�ն��ֻ���

   /*��ʱ����*/
   uint32_t HeartBeatUpdataFre;//����Ƶ�� //0x0001
   Timeout_Struct SaveTcpRetransTimes;//TCP��Ϣ��ʱ����0x0002 0x0003
   Timeout_Struct SaveUdpRetransTimes;//UDP��Ϣ��ʱ����0x0004 0x0005
   Timeout_Struct SaveSmsRetransTimes;//sms��Ϣ��ʱ����0x0006 0x0007

   /*����APN����*/
   GprsBearerParameter_Struct SaveMainBeareParameter; //��APN����0x0010 0x0011 0x0012 
   GprsBearerParameter_Struct SaveBackupBeareParameter;//����

   /*�������������Ӳ���*/
   ServerParameter_Struct SaveServerParameterArray[SERVERSAVENUM];// ��Ҫ���ݷ���������

   /*IC����֤���������Ӳ���*/
   ICServerParameter_Struct ic_server_parameter; //IC����������

   /*�ϱ�����*/ //0x0020--->
   location_reporting_param_strcut location_reporting_param;
   
   /*�յ����Χ������*/ //0x0030--->0x0031
   uint32_t inflection_angle_passup;            //0x0030 DWORD �յ㲹���Ƕȣ�<180
   uint16_t electronic_fence_radius;            //0x0031 WORD ����Χ���뾶���Ƿ�λ����ֵ������λΪ��

   /*�ֻ�������ز���*/ 
   phone_number_param_strcut phone_number_param;

   /*�������β���*/ 
   Alarm_Inf_Struct alarm_mask;//0x0050 DWORD���������֣���λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ 1����Ӧ����������

   /*���ű������ز���*/ 
   Alarm_Inf_Struct alarm_sms_swtich;//0x0051 DWORD ���������ı� SMS ���أ���λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ 1 ����Ӧ����ʱ�����ı� SMS

   /*�������տ��ز���*/ 
   Alarm_Inf_Struct alarm_camer_switch;//0x0052 DWORD�������㿪�أ���λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1 ����Ӧ����ʱ����ͷ����
   /*�������մ������*/ 
   Alarm_Inf_Struct alarm_camer_upload;//0x0053 DWORD��������洢��־����λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ 1 �����Ӧ����ʱ�ĵ���Ƭ���д洢������ʵʱ�ϴ�

   /*�ؼ���������*/ 
   Alarm_Inf_Struct linchpin_alarm;//0x0054 DWORD �ؼ���־����λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ 1 �����Ӧ����Ϊ�ؼ�����

   /*���ٲ���*/ 
   uint32_t max_speed;        //0x0055 DWORD ����ٶȣ���λΪ����ÿСʱ��km/h��
   uint32_t speeding_duration;//0x0056 DWORD ���ٳ���ʱ�䣬��λΪ�루s��

   /*ƣ�ͼ�ʻ����*/ 
   uint32_t continuous_driving_threshold;//0x0057 DWORD ������ʻʱ�����ޣ���λΪ�루s��
   uint32_t day_continuous_driving_threshold;//0x0058 DWORD �����ۼƼ�ʻʱ�����ޣ���λΪ�루s��
   uint32_t min_rest_period;//0x0059 DWORD ��С��Ϣʱ�䣬��λΪ�루s��
   uint32_t max_parking_time;//�ͣ��ʱ�䣬��λΪ�루s��

   /*Ԥ������*/ 
   uint16_t speed_warning_dif; //0x005B WORD ���ٱ���Ԥ����ֵ����λΪ 1/10Km/h
   uint16_t fatigue_dring_warning_dif; //0x005C WORD ƣ�ͼ�ʻԤ����ֵ����λΪ�루s����>0

   /*��ײ����*/ 
   uint16_t collision_warning_param;/*0x005D WORD
                                                ��ײ�����������ã�
                                                b7-b0�� ��ײʱ�䣬��λ 4ms��
                                                b15-b8����ײ���ٶȣ���λ 0.1g�����÷�Χ�ڣ�0-79 ֮�䣬Ĭ��Ϊ
                                                10��*/
   uint16_t rollover_param;//0x005D WORD0x005E WORD�෭�����������ã��෭�Ƕȣ���λ 1 �ȣ�Ĭ��Ϊ 30 �ȡ�

   /*���ղ���*/
   photo_param_struct time_photo_param;//��ʱ����
   photo_param_struct distance_photo_param;//��������

   /*ͼ�����*/
   uint32_t image_quality;    //0x0070 DWORD ͼ��/��Ƶ������1-10��1 ���
   uint32_t image_brightness; //0x0071 DWORD ���ȣ�0-255
   uint32_t image_contrast;   //0x0072 DWORD �Աȶȣ�0-127
   uint32_t image_saturation; //0x0073 DWORD ���Ͷȣ�0-12
   uint32_t image_chroma;     //0x0074 DWORD ɫ�ȣ�0-255
   
   /*��̲���*/
   uint32_t mileage;//0x0080 DWORD ������̱������1/10km
                    /*ʡ���������ע�����*/
   /*��λ����*/
   uint8_t location_mode;/*0x0091 BYTE
                                GNSS ��λģʽ���������£�
                                bit0��0������ GPS ��λ�� 1������ GPS ��λ��
                                bit1��0�����ñ�����λ�� 1�����ñ�����λ��
                                bit2��0������ GLONASS ��λ�� 1������ GLONASS ��λ��
                                bit3��0������ Galileo ��λ�� 1������ Galileo ��λ��*/
   uint8_t baud_rate;/*0x0092 BYTE
                            GNSS �����ʣ��������£�
                            0x00��4800��0x01��9600��
                            0x02��19200��0x03��38400��
                            0x04��57600��0x05��115200��*/
   uint8_t location_out_frze;/*GNSS ģ����ϸ��λ�������Ƶ�ʣ��������£�
                                      0x00��500ms��0x01��1000ms��Ĭ��ֵ����
                                      0x02��2000ms��0x03��3000ms��
                                      0x04��4000ms��*/ 
                                     
   uint32_t detail_location_get_frze;//GNSS ģ����ϸ��λ���ݲɼ�Ƶ�ʣ���λΪ�룬Ĭ��Ϊ 1
   uint8_t detail_location_upload_mode;/*GNSS ģ����ϸ��λ�����ϴ���ʽ��
                                                0x00�����ش洢�����ϴ���Ĭ��ֵ����
                                                0x01����ʱ�����ϴ���
                                                0x02�����������ϴ���
                                                0x0B�����ۼ�ʱ���ϴ����ﵽ����ʱ����Զ�ֹͣ�ϴ���
                                                0x0C�����ۼƾ����ϴ����ﵽ������Զ�ֹͣ�ϴ���
                                                0x0D�����ۼ������ϴ����ﵽ�ϴ��������Զ�ֹͣ�ϴ���*/
   uint32_t detail_location_upload_setting;/*GNSS ģ����ϸ��λ�����ϴ����ã�
                                                    �ϴ���ʽΪ 0x01 ʱ����λΪ�룻
                                                    �ϴ���ʽΪ 0x02 ʱ����λΪ�ף�
                                                    �ϴ���ʽΪ 0x0B ʱ����λΪ�룻
                                                    �ϴ���ʽΪ 0x0C ʱ����λΪ�ף�
                                                    �ϴ���ʽΪ 0x0D ʱ����λΪ����*/  
   /*CAN����*/                                                
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

    





