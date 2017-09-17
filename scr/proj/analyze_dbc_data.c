
/****************************************************
 * Include Files
****************************************************/ 
 
 #include "analyze_dbc_data.h"

 /****************************************************
* External variables declaration
*****************************************************/
DbcDataSave_Struct DbcDataSave={"",0,{3,3,6,6},{0}};

/*****************************************************
* Local variables declaration(Static)
******************************************************/
static int32_t dbc_file_size=0;
static uint32_t read_dbc_offest=0;
static DBCFileRead_Struct dbc_file_read={{0},0};//解析完成的dbc数据
 
/*****************************************************************************
* Function :dbc_save_init
* Description:  inintnal dbc save param
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/

static void dbc_save_init(void)
{
    dbc_file_size=0;
    read_dbc_offest=0;
    memset(&DbcDataSave,0,sizeof(DbcDataSave));
    memset(&dbc_file_read,0,sizeof(dbc_file_read));
}

/*****************************************************************************
* Function :board_can_inf_setting
* Description:  snalyze all dbc data
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void dbc_file_read_init(void)
{
    file_write(dbc_struct_save,0,&DbcDataSave,sizeof(DbcDataSave));//
    if(DbcDataSave.save_message_num==0)
    {
        file_read(dbc_struct_save,0,&DbcDataSave,sizeof(DbcDataSave));//
        if(DbcDataSave.save_message_num==0)
        {
            file_read(dbc_struct_save,0,&DbcDataSave,sizeof(DbcDataSave));//
        }
    }
}

/*****************************************************************************
* Function :board_can_inf_setting
* Description:  snalyze all dbc data
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void board_can_inf_setting(void)
{
    uint8_t can_id_num=0;
    uint8_t *send_point=NULL;
    uint8_t send_data[704]={0};
    send_data[0] = DbcDataSave.chanel_baudrate.HSBaudrate;
    send_data[1] = DbcDataSave.chanel_baudrate.MSBaudrate;
    send_data[2] = DbcDataSave.chanel_baudrate.LSBaudrate;
    send_data[3] = DbcDataSave.chanel_baudrate.ESBaudrate;
    send_data[4]=0;
    send_data[5] = DbcDataSave.save_message_num;
    send_point = send_data + 6;
    for(can_id_num=0;can_id_num<DbcDataSave.save_message_num;can_id_num++)
    {
        *send_point = DbcDataSave.DbcParmSaveArray[can_id_num].message_attribute;
        send_point+=1;
        Big_End_WordtoByte(DbcDataSave.DbcParmSaveArray[can_id_num].message_get_fre,send_point);
        send_point+=2;
        *send_point=DbcDataSave.DbcParmSaveArray[can_id_num].chanal;
        send_point+=1;
        Big_End_DwordtoByte(DbcDataSave.DbcParmSaveArray[can_id_num].message_id,send_point);
        send_point+=4;
    }
    uart1_send_data_frame(dbc_can_id_seting,send_data,8*DbcDataSave.save_message_num+6);
    #if chek_can_data 
    eat_trace("board_can_inf_setting  -->data.");
    HexToAsicc(send_data,8*DbcDataSave.save_message_num+6);
    #endif
}

/*****************************************************************************
* Function :file_read_one_line
* Description:  file_read_one_line
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
char_t file_read_one_line(const unsigned short *FileName,uint32_t size,int Ofest)
{
  uint8_t line_length=0;
  char_t read_statue_ret=0;
  int eat_read_error=0;
  memset(dbc_file_read.block_buffer,0,sizeof(dbc_file_read.block_buffer));
  if(Ofest+sizeof(dbc_file_read.block_buffer)>=size)
  {
      eat_read_error=file_read(FileName,Ofest,dbc_file_read.block_buffer,size-Ofest);
      if(eat_read_error==EAT_FALSE)
      {
          read_statue_ret=3;
      }
      else
      {
          eat_read_error=file_read(FileName,Ofest,dbc_file_read.block_buffer,size-Ofest);
          if(eat_read_error==EAT_FALSE)
          {
              read_statue_ret=3;
          }
          else
          {
              read_statue_ret=4;//读取失败
          }
      }
  }
  else
  {
      read_statue_ret=0;
      eat_read_error=file_read(FileName,Ofest,dbc_file_read.block_buffer,sizeof(dbc_file_read.block_buffer));
      if(eat_read_error==EAT_FALSE)
      {
         read_statue_ret=0;
      }
      else
      {
          eat_read_error=file_read(FileName,Ofest,dbc_file_read.block_buffer,sizeof(dbc_file_read.block_buffer));
          if(eat_read_error==EAT_FALSE)
          {
             eat_read_error=0;
          }
          else
          {
             read_statue_ret=4;
          }
      }
  }
  for(line_length=0;line_length<sizeof(dbc_file_read.block_buffer);line_length++)
  {
   
    if(*(dbc_file_read.block_buffer+line_length)==0x0A)
    {
        dbc_file_read.block_length=line_length+1;
        memset(dbc_file_read.block_buffer+line_length+1,0,sizeof(dbc_file_read.block_buffer)-(line_length+1));
        break;
    }
  }
  if(read_statue_ret!=3)
  {
      if(dbc_file_read.block_length>=3)
      {
         read_statue_ret=1;
      }
      else
      {
          read_statue_ret=2;
      }
  }
  else
  {
      if(line_length==sizeof(dbc_file_read.block_buffer))
      {
          if(*(dbc_file_read.block_buffer+size-Ofest)==';') 
          {
              dbc_file_read.block_length=size-Ofest;
              memset(dbc_file_read.block_buffer+size-Ofest,0,sizeof(dbc_file_read.block_buffer)-(size-Ofest));
          }
      }
  }
  return read_statue_ret;
}
/*****************************************************************************
* Function :dbc_BO_analyze
* Description:  analyze dbc BO_
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void dbc_BO_analyze(uint8_t *block_buff,uint8_t lens,uint32_t *id,uint8_t *size)
{
    uint8_t analyze_cnt=0;
    uint8_t save_cnt=0;
    uint8_t message_id[12]={0};
    uint8_t message_size[4]={0};
    uint8_t bo_analyze_step=0;
    for(analyze_cnt=0;analyze_cnt<lens;analyze_cnt++)
    {
        if(bo_analyze_step==0)
        {
            if(*(block_buff+analyze_cnt)==0x20)
            {
                bo_analyze_step=1;
            }
        }
        else if(bo_analyze_step==1)
        {
            if(*(block_buff+analyze_cnt)==0x20)
            {
                bo_analyze_step=2;
                save_cnt=0;
                *id = (uint32_t)atoll(message_id);
            }
            else
            {
                *(message_id+save_cnt)=*(block_buff+analyze_cnt);
                save_cnt++;
            }
        }
        else if(bo_analyze_step==2)
        {
             if(*(block_buff+analyze_cnt)==0x20)
            {
                bo_analyze_step=3;
            }
        }
        else if(bo_analyze_step==3)
        {
            if(*(block_buff+analyze_cnt)==0x20)
            {
                save_cnt=0;
                *size = (uint8_t)atoi(message_size);
                break;
            }
            else
            {
                *(message_size+save_cnt)=*(block_buff+analyze_cnt);
                 save_cnt++;
            }
        }
    }
}
/*****************************************************************************
* Function :dbc_sg_analyze
* Description:  analyze dbc SG_
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void dbc_sg_analyze(uint8_t *block_buff,uint8_t lens,CanSig_Struct *p_struct)
{
    uint8_t analyze_cnt=0;
    uint8_t save_cnt=0;
    uint8_t uint16buf[6]={0};
    uint8_t uint8buf[4]={0};
    uint8_t endatabuf[64]={0};
    uint8_t end_enter=0;
    uint32_t start_bit=0;//
    uint32_t signal_size=0;
    uint32_t byte_order=0;
    float64_t min,max;
    uint8_t sg_analyze_step=0;
    for(analyze_cnt=0;analyze_cnt<lens;analyze_cnt++)
    {
        if(sg_analyze_step==0)
        {
            if(*(block_buff+analyze_cnt)==0x20)
            {
                sg_analyze_step=1;
            }
        }
        else if(sg_analyze_step==1)
        {
            if(*(block_buff+analyze_cnt)==0xA1)
            {
               if(*(block_buff+analyze_cnt+1)==0xF2)
               {
                  sg_analyze_step=2;
                  save_cnt=0;
                  p_struct->signal_id = (uint16_t)atol(uint16buf);
                  memset(uint16buf,0,sizeof(uint16buf));
               }
               else
               {
                   *(uint16buf+save_cnt)=*(block_buff+analyze_cnt);
                   save_cnt++;
                   analyze_cnt++;
                   *(uint16buf+save_cnt)=*(block_buff+analyze_cnt);
                   save_cnt++;
               }
            }
            else
            {
                *(uint16buf+save_cnt)=*(block_buff+analyze_cnt);
                 save_cnt++;
            }
        }
        else if(sg_analyze_step==2)
        {
             if(*(block_buff+analyze_cnt)==0x3A)
            {
                sg_analyze_step=3;
            }
        }
        else if(sg_analyze_step==3)
        {
             if(*(block_buff+analyze_cnt)==0x20)
            {
                sg_analyze_step=4;
            }
        }
        else if(sg_analyze_step==4)
        {
            if(*(block_buff+analyze_cnt)=='|')
            {
                save_cnt=0;
                sg_analyze_step=5;
                p_struct->start_bit =(uint8_t)atoi(uint8buf);
                memset(uint8buf,0,sizeof(uint8buf));
            }
            else
            {
                *(uint8buf+save_cnt)=*(block_buff+analyze_cnt);
                 save_cnt++;
            }
        }
        else if(sg_analyze_step==5)
        {
           if(*(block_buff+analyze_cnt)=='@')
            {
                save_cnt=0;
                sg_analyze_step=6;
                p_struct->signal_size=(uint8_t)atoi(uint8buf);
                memset(uint8buf,0,sizeof(uint8buf));
            }
            else
            {
                *(uint8buf+save_cnt)=*(block_buff+analyze_cnt);
                 save_cnt++;
            } 
        }
        else if(sg_analyze_step==6)
        {
            if((*(block_buff+analyze_cnt)=='+')||(*(block_buff+analyze_cnt)=='-'))
            {
                save_cnt=0;
                sg_analyze_step=7;
                
                p_struct->byte_order=(uint8_t)atoi(uint8buf);
                memset(uint8buf,0,sizeof(uint8buf));
            }
            else
            {
                *(uint8buf+save_cnt)=*(block_buff+analyze_cnt);
                 save_cnt++;
            } 
        }
        else if(sg_analyze_step==7)
        {
            if(*(block_buff+analyze_cnt)=='(')
            {
                sg_analyze_step=8;
            }
        }
        else if(sg_analyze_step==8)
        {
            if(*(block_buff+analyze_cnt)==',')
            {
                save_cnt=0;
                sg_analyze_step=9;
                p_struct->factor=(float32_t)atof(endatabuf);
                memset(endatabuf,0,sizeof(endatabuf));
            }
            else
            {
                *(endatabuf+save_cnt)=*(block_buff+analyze_cnt);
                 save_cnt++;
            } 
        }
        else if(sg_analyze_step==9)
        {
            if(*(block_buff+analyze_cnt)==')')
            {
                save_cnt=0;
                
                p_struct->offset=(float32_t)atof(endatabuf);
                memset(endatabuf,0,sizeof(endatabuf));
                break;
            }
            else
            {
                *(endatabuf+save_cnt)=*(block_buff+analyze_cnt);
                 save_cnt++;
            } 
        }
    }
}
/*****************************************************************************
* Function :dbc_attribute_analyze
* Description:  analyze message attribute
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void dbc_attribute_analyze(uint8_t *block_buff,uint8_t lens,uint8_t * attribute)
{
    uint8_t lenght=0;
    for(lenght=0;lenght<lens;lenght++)
    {
        if(*(block_buff+lenght)==';')
        {
            //if((*(block_buff+lenght - 1)=='0')||(*(block_buff+lenght-1)=='1'))
            //{
           *attribute = *(block_buff+lenght -1) - 0x30;
            //}
           break;
        }
    }
}
/*****************************************************************************
* Function :dbc_find_start
* Description:  find dbc analyze start line 
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void dbc_find_start(void)
{
    uint8_t break_while=EAT_FALSE;
    char_t read_statue_ret=EAT_FALSE;
    dbc_save_init();
    get_file_size(DBC_FILE,&dbc_file_size);
    #if  0//APP_TRACE_DBG
    eat_trace("dbc_file_read -->file size :%u.",dbc_file_size);
    #endif
    if(dbc_file_size<=0)
    {
        #if dbc_analyze 
        eat_trace("dbc_file_read dbc_file_size<=0 --> read dbc start error!!!!");
        #endif
        dbc_receive_reslut_update(EAT_FALSE);//结果上报
        return;
    }
    do
    {
        read_statue_ret=file_read_one_line(DBC_FILE,dbc_file_size,read_dbc_offest);
        if(read_statue_ret==4)
        {
            #if dbc_analyze 
            eat_trace("dbc_file_read read_statue_ret==4--> read dbc start error!!!!");
            #endif
            dbc_receive_reslut_update(EAT_FALSE);//结果上报
            return;
        }
        else
        {
            if(read_dbc_offest==0)
            {
                if(strstr(dbc_file_read.block_buffer,"VERSION \"")!=NULL)//找到开始地址
                {
                    memset(DbcDataSave.dbc_vison,0,sizeof(DbcDataSave.dbc_vison));
                    memcpy(DbcDataSave.dbc_vison,dbc_file_read.block_buffer,dbc_file_read.block_length);
                    #if dbc_analyze 
                    eat_trace("dbc_file_read -->start:%s",dbc_file_read.block_buffer);
                    #endif
                }
            }
            else
            {
                if(strstr(dbc_file_read.block_buffer,"BU_:")!=NULL)//找到开始地址
                {
                   break_while=EAT_TRUE; 
                   #if dbc_analyze 
                   eat_trace("dbc_file_read -->start:%s",dbc_file_read.block_buffer);
                   #endif
                }
            }
            read_dbc_offest+=dbc_file_read.block_length;
            if(read_dbc_offest==dbc_file_size)
            {
                #if dbc_analyze 
                eat_trace("dbc_find_start read_dbc_offest==dbc_file_size--> read dbc start error!!!!");
                #endif
                break_while=EAT_TRUE; 
            }
        }
    }while(break_while==EAT_FALSE);
    if(read_dbc_offest==dbc_file_size)
    {
        read_dbc_offest=0;
        dbc_receive_reslut_update(EAT_FALSE);//结果上报
    }
    else
    {
        task_msg_send(EAT_USER_0,EAT_USER_3,dbc_file_read_msg);//发送消息到USER_2
    }
}
/*****************************************************************************
* Function :dbc_file_read_line
* Description:  find dbc analyze start line 
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void dbc_file_read_line(void)
{
    char_t read_one_line=127;
    read_one_line=file_read_one_line(DBC_FILE,dbc_file_size,read_dbc_offest);
    #if orign_dbc_printf
    eat_trace("%s",dbc_file_read.block_buffer);
    #endif
    if(read_one_line==1)//发送到解析
    {
        read_dbc_offest+=dbc_file_read.block_length;
        task_msg_send(EAT_USER_3,EAT_USER_3,dbc_file_analyze_msg);//发送消息到USER_2
    }
    else if(read_one_line==3)
    {
        if((read_dbc_offest + dbc_file_read.block_length)>=dbc_file_size)
        {
            read_dbc_offest=0;
            task_msg_send(EAT_USER_3,EAT_USER_3,dbc_file_analyze_end_msg);//发送消息到USER_2
        }
        else
        {
            read_dbc_offest+=dbc_file_read.block_length;
            task_msg_send(EAT_USER_3,EAT_USER_3,dbc_file_analyze_msg);//发送消息到USER_2
        }
    }
    else if(read_one_line==2)
    {
        read_dbc_offest+=dbc_file_read.block_length;
        task_msg_send(EAT_USER_3,EAT_USER_3,dbc_file_read_msg);//发送消息到USER_2
    }
    else if(read_one_line==4)
    {
        dbc_find_start();
    }
}
/*****************************************************************************
* Function :bubbleSort_for_dbc_messgae_id
* Description:  bubbleSort message id 排序
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
static void bubbleSort_for_dbc_messgae_id(void)
{
    uint8_t i,ii;
    DbcData_Struct temp_dbc;
    for(i=0;i<DbcDataSave.save_message_num-1;i++)
    {
        for(ii=0;ii<DbcDataSave.save_message_num-i-1;ii++)
        {
            if(DbcDataSave.DbcParmSaveArray[ii].message_id > DbcDataSave.DbcParmSaveArray[ii+1].message_id)
            {
                memcpy(&temp_dbc,&(DbcDataSave.DbcParmSaveArray[ii]),sizeof(temp_dbc));
                memcpy(&DbcDataSave.DbcParmSaveArray[ii],&DbcDataSave.DbcParmSaveArray[ii+1],sizeof(DbcDataSave.DbcParmSaveArray[ii+1]));
                memcpy(&DbcDataSave.DbcParmSaveArray[ii+1],&temp_dbc,sizeof(temp_dbc));
            }
        }
    }
    #if 0//dbc_analyze 
    for(i=0;i<DbcDataSave.save_message_num;i++)
    {
         eat_trace("bubbleSort_for_dbc_messgae_id -> cnt %u:message_id:%u",i,DbcDataSave.DbcParmSaveArray[i].message_id);
         eat_trace("bubbleSort_for_dbc_messgae_id -> signal_num:%u",DbcDataSave.DbcParmSaveArray[i].signal_num);
         for(ii=0;ii<DbcDataSave.DbcParmSaveArray[i].signal_num;ii++)
         {
              eat_trace("bubbleSort_for_dbc_messgae_id -> ii:%u",ii);
              eat_trace("bubbleSort_for_dbc_messgae_id -> signal_id:%u",DbcDataSave.DbcParmSaveArray[i].signal_array[ii].signal_id);
              eat_trace("bubbleSort_for_dbc_messgae_id -> start_bit:%u",DbcDataSave.DbcParmSaveArray[i].signal_array[ii].start_bit);
              eat_trace("bubbleSort_for_dbc_messgae_id -> signal_size:%d",DbcDataSave.DbcParmSaveArray[i].signal_array[ii].signal_size);
              eat_trace("bubbleSort_for_dbc_messgae_id -> byte_order:%u",DbcDataSave.DbcParmSaveArray[i].signal_array[ii].byte_order);
              eat_trace("bubbleSort_for_dbc_messgae_id -> factor:%lf",DbcDataSave.DbcParmSaveArray[i].signal_array[ii].factor);
              eat_trace("bubbleSort_for_dbc_messgae_id -> offset:%lf",DbcDataSave.DbcParmSaveArray[i].signal_array[ii].offset);
          }
    }
    #endif
}
/*****************************************************************************
* Function :dbc_file_analyze
* Description:  snalyze all dbc data
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void dbc_file_analyze(void)
{
    static uint8_t mesg_num=0;
    #if dbc_analyze 
    eat_trace("dbc_file_analyze --> one line:%s",dbc_file_read.block_buffer);
    #endif
    if(strstr(dbc_file_read.block_buffer,"BA_DEF_ ")!=NULL)
    {
        return;
    }
    else if(strstr(dbc_file_read.block_buffer,"BA_ \"VFrameFormat\" BO_ ")!=NULL)//找到开始地址
    {
        dbc_attribute_analyze(dbc_file_read.block_buffer + strlen("BA_ \"VFrameFormat\" BO_ "),dbc_file_read.block_length - strlen("BA_ \"VFrameFormat\" BO_ "),&DbcDataSave.DbcParmSaveArray[mesg_num].message_attribute);
        #if dbc_analyze 
        eat_trace("dbc_file_analyze -> message_attribute :%d",DbcDataSave.DbcParmSaveArray[mesg_num].message_attribute);
        #endif
        mesg_num++;
        if(mesg_num ==DbcDataSave.save_message_num)
        {
            mesg_num=0;
        }
    }
    else if(strstr(dbc_file_read.block_buffer,"BA_ \"VFrameChannel\" BO_ ")!=NULL)//找到开始地址
    {
        dbc_attribute_analyze(dbc_file_read.block_buffer + strlen("BA_ \"VFrameChannel\" BO_ "),dbc_file_read.block_length - strlen("BA_ \"VFrameChannel\" BO_ "),&DbcDataSave.DbcParmSaveArray[mesg_num].chanal);
        #if dbc_analyze 
        eat_trace("dbc_file_analyze -> message_chanal :%d",DbcDataSave.DbcParmSaveArray[mesg_num].chanal);
        #endif
        mesg_num++;
        if(mesg_num ==DbcDataSave.save_message_num)
        {
            mesg_num=0;
            #if PUBLIC_TRACE
            eat_fs_Delete(DBC_FILE);
            #endif
            file_create(DBC_MDF_FILE);
            bubbleSort_for_dbc_messgae_id();//排序
            file_create(dbc_struct_save);
            file_write(dbc_struct_save,0,&DbcDataSave,sizeof(DbcDataSave));//
            dbc_receive_reslut_update(EAT_TRUE);//结果上报
            #if APP_TRACE_DBG 
            eat_trace("dbc_file_analyze -> analyze all dbc ok!!!!!!!!");
            #endif
        }
    }
    else if(strstr(dbc_file_read.block_buffer,"BA_ \"HSBaudrate\" ")!=NULL)
    {
        sscanf(dbc_file_read.block_buffer,"BA_ \"HSBaudrate\" %c;",&DbcDataSave.chanel_baudrate.HSBaudrate);
        DbcDataSave.chanel_baudrate.HSBaudrate-=0x30;
        if(DbcDataSave.chanel_baudrate.HSBaudrate>6)
        {
            DbcDataSave.chanel_baudrate.HSBaudrate=6;
        }
    }
    else if(strstr(dbc_file_read.block_buffer,"BA_ \"MSBaudrate\" ")!=NULL)
    {
        sscanf(dbc_file_read.block_buffer,"BA_ \"MSBaudrate\" %c;",&DbcDataSave.chanel_baudrate.MSBaudrate);
        DbcDataSave.chanel_baudrate.MSBaudrate -=0x30;
        if(DbcDataSave.chanel_baudrate.MSBaudrate>6)
        {
            DbcDataSave.chanel_baudrate.MSBaudrate=6;
        }
    }
    else if(strstr(dbc_file_read.block_buffer,"BA_ \"LSBaudrate\" ")!=NULL)
    {
        sscanf(dbc_file_read.block_buffer,"BA_ \"LSBaudrate\" %c;",&DbcDataSave.chanel_baudrate.LSBaudrate);
        DbcDataSave.chanel_baudrate.LSBaudrate -=0x30;
        if(DbcDataSave.chanel_baudrate.LSBaudrate>6)
        {
            DbcDataSave.chanel_baudrate.LSBaudrate=6;
        }
    }
    else if(strstr(dbc_file_read.block_buffer,"BA_ \"ESBaudrate\" ")!=NULL)
    {
        sscanf(dbc_file_read.block_buffer,"BA_ \"ESBaudrate\" %c;",&DbcDataSave.chanel_baudrate.ESBaudrate);
        DbcDataSave.chanel_baudrate.ESBaudrate -=0x30;
        if(DbcDataSave.chanel_baudrate.ESBaudrate>6)
        {
            DbcDataSave.chanel_baudrate.ESBaudrate=6;
        }
    }
    else if(strstr(dbc_file_read.block_buffer,"BO_ ")!=NULL)
    {
          #if dbc_analyze
          eat_trace("dbc_file_analyze -->block_data:%s",dbc_file_read.block_buffer);
          #endif
          if(DbcDataSave.save_message_num < Message_Max)
          {
              dbc_BO_analyze(dbc_file_read.block_buffer,dbc_file_read.block_length,\
              &DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num].message_id,\
              &DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num].message_size);
              DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num].message_get_fre=can_update_fre_zero*1000;
              DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num].chanal=0;
              #if 0//dbc_analyze 
              {
                  uint8_t temp[4]={0};
                  eat_trace("dbc_file_analyze -> message_id:%u",DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num].message_id);
                  Big_End_DwordtoByte(DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num].message_id,temp);
                  HexToAsicc(temp,4);
                  eat_trace("dbc_file_analyze -> message_size:%u",DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num].message_size);
                  eat_trace("dbc_file_analyze -> save_message_num:%u",DbcDataSave.save_message_num);
              }
              #endif
              DbcDataSave.save_message_num++;
          }
          else
          {
              #if 0
              APP_TRACE_DBG 
              eat_trace("dbc_BO_analyze -> save_message_num: FULL");
              #endif
          }
    }
    else if(strstr(dbc_file_read.block_buffer," SG_ ")!=NULL)
    {
         if(DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_num < Signal_Max)
         {
              dbc_sg_analyze(dbc_file_read.block_buffer+2,dbc_file_read.block_length-2,\
                &DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_array[DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_num]);       
              #if 0//dbc_analyze
              eat_trace("dbc_file_analyze -> signal_id:%u",DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_array[DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_num].signal_id);
              eat_trace("dbc_file_analyze -> signal_num:%u",DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_num);
              eat_trace("dbc_file_analyze -> start_bit:%u",DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_array[DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_num].start_bit);
              eat_trace("dbc_file_analyze -> signal_size:%d",DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_array[DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_num].signal_size);
              eat_trace("dbc_file_analyze -> byte_order:%u",DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_array[DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_num].byte_order);
              eat_trace("dbc_file_analyze -> factor:%lf",DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_array[DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_num].factor);
              eat_trace("dbc_file_analyze -> offset:%lf",DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_array[DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_num].offset);
              #endif
              DbcDataSave.DbcParmSaveArray[DbcDataSave.save_message_num-1].signal_num++;
          }
          else
          {
              #if APP_TRACE_DBG 
              eat_trace("dbc_sg_analyze -> signal_num: FULL");
              #endif
          }
    }
}
/*****************************************************************************
* Function :dbc_recive_and_save
* Description:  recive and save dbc data;
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void dbc_recive_and_save(const unsigned short *FileName,uint16_t package_num,uint16_t package_cn,uint8_t *buffer,uint16_t len)
{
    static uint32_t file_offest=0;
    static uint32_t file_size=0;
    if(package_cn==1)
    {
        file_offest=0;
        Big_End_BytetoDword(buffer,&file_size);
        file_create(DBC_FILE);
        file_write(DBC_FILE,file_offest,buffer+4,len-4);//
        file_offest += (len-4);
    }
    else if(package_cn==package_num)
    {
        file_write(DBC_FILE,file_offest,buffer,len);//
        file_offest += len;
        if(file_offest==file_size)
        {
            file_offest=0; 
            dbc_find_start();
        }
        else
        {
           file_offest=0;  
           dbc_receive_reslut_update(EAT_FALSE);//结果上报
        }
    }
    else
    {
        file_write(DBC_FILE,file_offest,buffer,len);//
        file_offest += len;
    }
}
/*****************************************************************************
* Function :dbc_receive_reslut_update
* Description:  recive and save dbc data relult;
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void dbc_receive_reslut_update(uint8_t reslut)
{
    uint16_t msgs_sern=0;
    uint8_t updata_buffer[64]={0};
    updata_buffer[0]=reslut;
    memcpy(updata_buffer+1,DbcDataSave.dbc_vison,strlen(DbcDataSave.dbc_vison));
    Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msgs_sern,dbc_data_reslut_up,updata_buffer,strlen(DbcDataSave.dbc_vison)+1,0,0,0);
}
/*****************************************************************************
* Function :dbc_receive_reslut_update
* Description:  recive and save dbc data relult;
* Parameters :
* Returns:
* NOTE
*     Nothing
*****************************************************************************/
void dbc_version_update(void)
{
    uint16_t msgs_sern=0;
    uint8_t updata_buffer[64]={0};
    memcpy(updata_buffer,DbcDataSave.dbc_vison,strlen(DbcDataSave.dbc_vison));
    Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msgs_sern,dbc_data_vision_up,updata_buffer,strlen(DbcDataSave.dbc_vison),0,0,0);
}

