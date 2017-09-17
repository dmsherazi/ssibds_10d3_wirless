/********************************************************************************


*********************************************************************************/

#ifndef __ENCRYPTION_MD5_H
#define __ENCRYPTION_MD5_H                  

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include"eat_interface.h"

#define md5_encpy_debug_printf 1

#define F(x,y,z) (((x)&(y))|((~x)&(z)))
#define G(x,y,z) (((x)&(z))|((y)&(~z)))
#define H(x,y,z) ((x)^(y)^(z))
#define I(x,y,z) ((y)^((x)|(~z)))
#define ROTATE_LEFT(x,n) (((x) << n) | ((x) >> (32-(n))))
#define FF(a,b,c,d,x,s,ac) \
          { \
          a += F(b,c,d) + x + (uint32_t)(ac); \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define GG(a,b,c,d,x,s,ac) \
          { \
          a += G(b,c,d) + x + (uint32_t)(ac); \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define HH(a,b,c,d,x,s,ac) \
          { \
          a += H(b,c,d) + x + (uint32_t)(ac); \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define II(a,b,c,d,x,s,ac) \
          { \
          a += I(b,c,d) + x + (uint32_t)(ac); \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }   
typedef struct
{
    uint32_t count[2];
    uint32_t state[4];
    uint8_t buffer[64];
	
}MD5_CTX;
          
void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
void MD5Final(MD5_CTX *context,unsigned char digest[16]);
void MD5Transform(unsigned int state[4],unsigned char block[64]);
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);
extern int check_md5_encryption(uint8_t *souce_code,uint8_t *num_scr);
extern int check_md5_encryption_sms1(uint8_t *souce_code,uint8_t *time_scr);
extern int check_md5_encryption_sms2(uint8_t *souce_code,uint8_t *time_scr);

#endif

