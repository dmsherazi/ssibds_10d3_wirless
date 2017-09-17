/********************************************************************
 *                Copyright co. Ltd.  ssi                
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
 * ������ˮ�Ŵ��������꣬����ƽ̨�а���ű�����������
 *--------------------------------------------------------------------
 ********************************************************************/
#include "subcontract_servn_num.h"


/*ȫ����ˮ��*/
uint16_t global_tcp_msg_servn=0; 
/*��ý���ϴ�ȫ����ˮ��*/
uint16_t multimedia_start_msg_servn=0;
/*��ʻ��¼�ϴ�ȫ����ˮ��*/
uint16_t driver_start_msg_servn=0;

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Protocol_Init
 *  Description:  ע�����ݳ�ʼ��
 *  Parameters :  void     
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
void subcontract_servn_num_pro(uint16_t msg_id,uint16_t package_cnt,uint16_t *msg_sn_f,uint16_t *msg_sn_out)
{   
    if(msg_id==multimedia_updata_id)
    {
        if(package_cnt==1)multimedia_start_msg_servn=global_tcp_msg_servn;
        *msg_sn_f = multimedia_start_msg_servn + package_cnt-1;
        *msg_sn_out=multimedia_start_msg_servn + package_cnt-1;
    }
    else if(msg_id==driver_record_updata_id)
    {
        if(package_cnt==1)driver_start_msg_servn=global_tcp_msg_servn;
        *msg_sn_f = driver_start_msg_servn + package_cnt-1;
        *msg_sn_out = driver_start_msg_servn + package_cnt-1;
    }
    else
    {
        *msg_sn_out=global_tcp_msg_servn++;
    }
}
    
