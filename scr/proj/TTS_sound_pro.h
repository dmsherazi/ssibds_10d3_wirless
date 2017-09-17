#ifndef _TTS_SOUND_PRO_H
#define _TTS_SOUND_PRO_H

#include "eat_interface.h"

/*�����������Կ���*/
#define tts_tone_debug_printf 1
/*�����������Կ���*/
#define txt_save_num_max 50

/*�ı���Ϣ�����ļ���*/
#define txt_record_file L"D:\\txt_record"

/*�ı���Ϣ����*/
#define txt_data_record L"D:\\txt_record\\txt_data_record.bin"//������Ϣ����

/***txt��Ϣ������ѯ***/
#define send_txt_cnt_inquire 0x80

/***txt��Ϣ����***/
#define send_txt_infor_inquire 0x81


/*�ı���Ϣ��־λ����*/
typedef  union _remote_txt_data//ƽ̨�·����ݽṹ
{
	unsigned char txt_rev;
	struct
		{
		    unsigned char urgent : 1;  // 1������
			unsigned char rsv_0 : 1;  //����
			unsigned char displaly : 1; // 1���ն���ʾ����ʾ
			unsigned char tone_play : 1; // 1���ն� TTS ����
			unsigned char advert_display :1;// 1���������ʾ
			unsigned char navigation_can_error :1;// 0�����ĵ�����Ϣ��1��CAN ��������Ϣ
			unsigned char rsv : 2;        //����λ 			
		}txt_bit;
}remote_txt_data;

/*�ı���¼�ṹ��*/
typedef struct
{
    uint8_t navigation_num;// ������Ϣ����
    uint8_t can_error_num;// CAN ���� txt����
    uint8_t navig_time[50][6];
    uint8_t can_time[50][6];
}txt_record_struct;

/*�ı���Ϣ��¼*/
extern txt_record_struct txt_record;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: txt_data_record_init
 *  Description: txt
 *        input: ��ʼ��txt��¼
 *       author: Jumping create at 2015-12-16
 * =====================================================================================
 */
extern void txt_data_record_init(void);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: read_txt_data_record
 *  Description: ��״̬
 *        input: ��ʼ��txt��¼
 *       author: Jumping create at 2015-12-16
 * =====================================================================================
 */
extern void read_txt_data_record(void);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: tts_tone_envlope_play
 *  Description: ��װ����������
 *        input: scr_tone:��Ҫ�����ı�����ָ��
 *       author: Jumping create at 2015-12-16
 * =====================================================================================
 */
extern void tts_tone_envlope_play(uint8_t *scr_tone);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_Tcp_Register
 *  Description:  ������յ���ͨ��Ӧ��
 *  Parameters : Temp_data_buff ;���յ�����Ч����ָ��
 *             :  tempLens ����
 *			   :  Sn ��ˮ��
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
extern void remote_txt_data_proc(uint8_t *remote_point,uint16_t data_len);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: txt_data_cnt_inquire
 *  Description:  ��ѯTXT���ݸ���
 *  Parameters : cmd_data ;���յ�����Ч��������
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
extern void txt_data_cnt_inquire(uint8_t cmd_data);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: txt_data_cnt_inquire
 *  Description:  ��ѯTXT��������
 *  Parameters :  cmd data :0x01��������0x02:CAN���ϡ�TXT_nuM���ݱ��
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void txt_data_infor_inquire(uint8_t cmd_data,uint8_t txt_num);

#endif
