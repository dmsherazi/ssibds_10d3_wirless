
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
}_RET_INFOR; //���ݷ������

/*��ʱʱ�䡢����*/
typedef struct
{
    uint32_t waiting_time; //Ƶ��
    uint32_t timeout_times;//����
}Timeout_Struct;
/*��ʱ�ṹ��*/
typedef struct
{
    char_t on_off;
    Timeout_Struct time_out_struct;
}TimeoutEvent;

/*�������б�*/
typedef enum
{
  MAIN_SERVER=0, //��һ��������
  BACKUP_MAIN_SEVER, //��һ����������
  SECOND_SERVER,//�ڶ�������
  BACKUP_SECOND_SERVER //�ڶ�����������
}LinkIndex_Enum;//�������� 

/*��ȡIP״̬*/
typedef enum
{
  GETIPSUCESS=1,  //��ȡIP�ɹ�
  GETIPWATING,    //�ȴ���ȡIP���
  GETIPFAIL       //���IPʧ��
}GETIPRESULT_Enum;//��ȡIP����

/*�����������ṹ*/
typedef struct 
{
    uint8_t server_domin[68];//����
    uint32_t server_socket;//�˿�
    uint32_t udp_server_socket;//udP�˿�
}ServerParameter_Struct; //������Ϣ�ṹ��

/*�����������ṹ*/
typedef struct 
{
    uint8_t main_server_domin[68];  //IC����
    uint8_t backup_server_domin[68];//IC ��������
    uint32_t tcp_server_socket;  //TCP�˿�
    uint32_t udp_server_socket;  //UDP�˿�
}ICServerParameter_Struct; //������Ϣ�ṹ��


typedef struct 
{
    char_t get_ip_statue; //��ȡIP״̬
    LinkIndex_Enum link_index; //��������
    char_t link_staue;  //���ӵ�ǰ״̬
    uint8_t link_ip[4]; //����IP 4BYTE
    int16_t mode_Socket; //ģ���ڲ��˿ں�
}PresentLinkParameter_Struct;


typedef struct
{
   uint8_t beare_apn[32];
   uint8_t beare_user[32];
   uint8_t beare_password[32];
}GprsBearerParameter_Struct;//�������



typedef struct
{
    uint8_t terminl_phone_nums[14];//�ַ���
    uint8_t terminl_phone_numd[6];//BCD��
}TerminlPhone_Struct;//�ֻ��Žṹͼ
/*������Ϣλ�����Ŵ���ṹ��*/
typedef union _Alarm_Inf_Struct_
{
  uint32_t Alarm_Original;
  struct 
  {
    uint32_t Sos: 1;                       //��������1
    uint32_t Speed_Out: 1;                  //����2
    uint32_t Fatigue_Driv: 1;              //ƣ�ͼ�ʻ3
    uint32_t Danger: 1;                    //Σ��Ԥ��4
    uint32_t GNSS_Mode: 1;                 //GNSS,ģ�����5
    uint32_t Antenna_Discon: 1;            //���߶Ͽ�6
    uint32_t Antenna_Short_Circuit: 1;     //��·7
    uint32_t Undervoltage: 1;              //Ƿѹ8

    uint32_t Power_Down: 1;                //����1
    uint32_t LCD_Failure: 1;               //LCD����2
    uint32_t TTS_Failure: 1;               //tts����3
    uint32_t Camer_Failure: 1;             //Camer����4
    uint32_t IC_Failure: 1;                //IC����5
    uint32_t Speed_Early_Warning: 1;       //����Ԥ��6
    uint32_t Fatigue_Driv_Early_Warning: 1;//ƣ�ͼ�ʻԤ��7
    uint32_t SOS_Phone: 1;				  //ƣ�ͼ�ʻԤ��7
    uint32_t Rev1: 2;                      //����812

    uint32_t Day_Sum_Fatigue_Driv: 1;      //�����ۼƳ�ʱ��ʻ3
    uint32_t Park_Over_Time: 1;            //��ʱͣ����4
    uint32_t Region_In_Out: 1;             //��������5
    uint32_t Route_In_Out: 1;              //����·�ߣ�6
    uint32_t Route_Driving_OutorShort:1;   //·����ʻʱ�䣻7
    uint32_t Off_Route: 1;                 //ƫ��·�ߣ�8

    uint32_t VSS_Failure: 1;               //VSS���ϣ� 1
    uint32_t Oil_Mass_Failure: 1;          //�����쳣��2
    uint32_t Viotrue_Stolen: 1;            //������͵��3
    uint32_t Illegality_Ignited: 1;        //�Ƿ����; 4
    uint32_t Illegality_Vector: 1;         //�Ƿ�λ�ƣ�5
    uint32_t Crashed: 1;                   //��ײ      6   
    uint32_t Rolloved: 1;                  //�෭      7
    uint32_t Illegality_Open_Door: 1;      //�Ƿ�����  8
  }Alarm_Bit;
}Alarm_Inf_Struct;


/*״̬��Ϣλ�����Ŵ���ṹ��*/
typedef union _State_Inf_Struct_
{
  uint32_t State_Inf;
  struct 
  {
    uint32_t ACC: 1;                           //ACC���أ� 1
    uint32_t Wether_Location : 1;              //�Ƿ�λ  2
    uint32_t SouthorNorth: 1;                  //�ϱ�γ��  3
    uint32_t EastorWest: 1;                    //����γ��  4
    uint32_t Operation: 1;                     //��Ӫ״̬  5
    uint32_t Wether_Encrypt_LongLa: 1;         //��γ�ȼ��� 6
    uint32_t Rev: 2;                           //������7 8

    uint32_t Kg: 2;                            //����״̬��1��2
    uint32_t Oil_Way: 1;                       //��·״̬��3
    uint32_t Circuit: 1;                       //��·״̬��4
    uint32_t Lock_Door: 1;                     //������    5
    uint32_t Front_Door: 1;                    //ǰ��������6
    uint32_t Mid_Door: 1;                      //������    7
    uint32_t Befor_Door: 1;                    //������    8

    uint32_t Driv_Door: 1;                     //��ʻϯ����1
    uint32_t Rev_Door: 1;                      //�Զ���    2
    uint32_t GPS: 1;                           //GPS��λ   3
    uint32_t BD: 1;                            //BD��λ    4
    uint32_t GLONSS: 1;                        //GLONSS    5
    uint32_t Galile: 1;                        //Galile  
    uint32_t usb_sate :1;                      //usb״̬
	uint32_t ip1_lock_board_state;             //IP1����״̬
	uint32_t ip2_lock_board_state;             //IP2����״̬
    uint32_t State_Rev2: 7;                         //����
  }State_Bit;
}State_Inf_Struct;


/*���ٱ���������Ϣ�ṹ��*/
typedef struct _Over_Speed_Alarm_Add_Struct_
{
	uint8_t Over_Speed_Location_Type; /*����λ������ 0x00:���ض�λ�ã�0x01:Բ��0x02:���Σ� 0x03:����Σ�0x04:·�ߣ�*/															    
	uint32_t Area_ID;				 //����ID
}Over_Speed_Alarm_Add_Struct;//���ٸ�����Ϣ


/*�������򸽼���Ϣ�ṹ��*/
typedef struct _Area_InorOut_Inf_Add_Struct_
{
	uint8_t InorOut_Area_Type;	/*��������:0x01:Բ��0x02:���Σ� 0x03:����Σ�0x04:·�ߣ�*/
	uint32_t InorOut_Area_ID;	//��������ID;
	uint8_t InorOut_Area_Action;	//���������� 0x00:����0x01:����
}Area_InorOut_Inf_Add_Struct;

/*·����ʻʱ�䲻�������Ϣ�ṹ��*/
typedef struct _Route_Driv_OutorShort_Add_Struct_
{
	uint32_t Route_Driv_OutorShort_ID;		//·��ID
	uint16_t Route_Driv_OutorShort_Time;		//��ʻʱ��;
	uint8_t Route_Driv_OutorShort_Reslut;	//���������� 0x00:����0x01:����
}Route_Driv_OutorShort_Add_Struct;

/*��չ�ź���*/
typedef union _Extend_Signle_ 
{
	uint32_t Extend_Signle;
	struct
	{
		uint32_t Near_Light: 1;				//�����;   1
		uint32_t Far_Light: 1;				//Զ��ƣ�	2
		uint32_t Right_Light: 1;				//��ת��ƣ�3
		uint32_t Left_Light: 1;				//��ת���	4
		uint32_t Bark_Signle: 1;				//ɲ���ź�	5
		uint32_t Reverse_Gear_Signle :1;		//�����ź�	6
		uint32_t Fog_Lamp : 1;				//����ź�	7
		uint32_t End_Outline_Market_Lamp : 1;//���ȵ��ź�	8
		
		uint32_t LoudSpeak_Signle: 1;		//�����ź�	9
		uint32_t Air_Condition: 1;			//�յ�		10
		uint32_t Neutral_Gear: 1;			//�յ�		11
		uint32_t	Retarder:1;					//������	12
		uint32_t ABS:1;						//			13
		uint32_t Heater:1 ;					//������	14
		uint32_t Clutch :1;					//�����	16
		uint32_t Little_Lamp:1;				//С��
		
		uint32_t Rev_IO:16;					//����
	}Extend_Signle_Bit;
}Extend_Signle_Union;

/*IO״̬�ṹ��*/
typedef union _IO_State_Union_
{
	uint16_t IO_State;
	struct
	{
		uint16_t Deep_Sleep: 1;				//�������
		uint16_t System_Sleep: 1;			//ϵͳ����
		uint16_t	IO_Rev: 14;					//IO����
	}IO_State_Bit;
}IO_State_Union;


/*λ���Ŵ���ṹ��*/
typedef struct 
{
    /*����λ����Ϣ*/ 
    Alarm_Inf_Struct Alarm_Inf;           //������Ϣ
    State_Inf_Struct VDR_State_Inf;       //״̬��Ϣ
    uint32_t Location_Latitude;                //γ��
    uint32_t Location_Longitude;               //����
    uint16_t Location_Altitude;                //�߳�
    uint16_t Location_Speed;                   //�ٶ���Ϣ
    uint16_t Location_Direction;               //���� 0-360
    uint8_t  Curren_Time[6];                   //��ǰʱ��
    /*����λ����Ϣ*/ 
	uint32_t Vehicle_Odometer;					//ID 0x01;1/10KM
	uint16_t Power_volate;
	uint16_t Oil_Mass;							//ID 0X02;1/10L
	uint32_t oil_mass_second;//�ڶ��ͺ�
	uint32_t oil_first;//��һIP
	uint16_t Travel_Speed;						//ID 0x03;1/10km/h
	uint16_t Manual_Work_ID;						//ID 0x04;�˹�ȷ�ϱ���ID;
	Over_Speed_Alarm_Add_Struct Over_Speed_Alarm_Add; 			//ID 0x11;���ٱ���������Ϣ
	Area_InorOut_Inf_Add_Struct Area_InorOut_Inf_Add;			//ID 0x12;�������򸽼���Ϣ
	Route_Driv_OutorShort_Add_Struct Route_Driv_OutorShort_Add;	//ID 0x13;·����ʻʱ�䲻����߹���
	Extend_Signle_Union Extend_Signle_Add;						//ID 0x25;��չ�ź�
	IO_State_Union IO_State_Add;								//ID 0x2A;IO״̬��
	uint32_t Analog_Signal;											//ID 0x2B;ģ����
	uint8_t Sing_strength;											//ID 0x30;�����ź�ǿ��
	uint8_t Satelite_Amount;											//ID 0x31;����������
}Location_Inf_Struct;


struct list_head 
{
      struct list_head *next, *prev;
};

typedef struct _receive_data_pack_queue
{
   uint16_t data_len;
   uint8_t tcp_data_buf[TcpDataBufSize];//����ͷָ��
   struct list_head re_list;
}receive_data_pack_queue;//tcp���ջ���
/*��ý����Ϣ������Ϣ*/
typedef struct 
{
    uint32_t media_id;
    uint8_t media_type;//0��ͼ��1����Ƶ��2����Ƶ��
    uint8_t media_format;// 0��JPEG��1��TIF��2��MP3��3��WAV��4��WMV���������� 
    uint8_t media_event;// ��������/*0��ƽ̨�·�ָ�1����ʱ������2�����ٱ���������3����ײ�෭����������4���ſ����գ�5���Ź����գ�6�������ɿ���أ�ʱ�ٴӣ�20 ���ﵽ���� 20 ���7���������գ�*/
    uint8_t media_chanl_id;// ͨ��ID
    uint8_t process_type;  //�������� 0��ʵʱ�ϴ���1������
    uint8_t picture_size;//ͼƬ��С
    uint8_t picture_qulity;//ͼƬ����
    uint8_t media_location[28];//��ý��λ����Ϣ
}multimedia_add_struct;
/*¼����Ϣ���Ӳ���*/
typedef struct 
{
    uint32_t media_id;//��ý��ID
    uint8_t chanel_id;//��ý��ͨ��
    uint8_t media_event;//��ý���¼�
    uint32_t media_time;//��ý��ʱ��
}media_add_struct;
/*�������ݼ�¼��Ϣ�ṹ��*/

typedef struct _lock_car_save_struct_
{
    uint8_t lock_ip;
	uint8_t save_state;		//����״̬
	uint8_t re_save_state;  //����״̬
	
	uint16_t lock_id;		//����ID;
	uint16_t sleep_lock_id; //��������ID
	
	uint16_t lock_data_len;       //���ݳ���
	uint16_t re_lock_data_len;    //�������ݳ���
	uint16_t sleep_lock_data_len; //������������
	
	uint8_t sleep_data_save[32];//�������ݴ洢
	uint8_t lock_data[32];	//�������ݣ�
	uint8_t re_lock_data[32];	//�������ݷ��أ�
	uint8_t lock_car_data_save[64];//�洢����
}lock_car_save_struct;

#endif 
