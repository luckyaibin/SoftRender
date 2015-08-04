#ifndef __CRC32_H__
#define __CRC32_H__
#include <stdio.h>
char * get_bit_string(unsigned int v);

unsigned int crc32_gen_table1();


unsigned int crc32_gen_table(int divide_table[256]);


unsigned int CRC32(void* msg,int msg_len,unsigned int initial_effect=0);


#define POLY 0x04C11DB7L // CRC32生成多项式 
static unsigned int crc_table[256]; 
unsigned int get_sum_poly_others(unsigned char data); 

void create_crc_table_others();


unsigned int CRC32_others(char *p,int len);

#endif