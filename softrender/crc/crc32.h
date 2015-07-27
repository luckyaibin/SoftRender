#ifndef __CRC32_H__
#define __CRC32_H__

char * get_bit_string(unsigned int v);

unsigned int crc32_gen_table1();


unsigned int crc32_gen_table(int divide_table[256]);


unsigned int CRC32(void* msg,int msg_len);


#define POLY 0x04C11DB7L // CRC32生成多项式 
static unsigned int crc_table[256]; 
unsigned int get_sum_poly(unsigned char data) 
{ 
	unsigned int sum_poly = data; 
	sum_poly <<= 24; 
	for(int j = 0; j < 8; j++) 
	{ 
		int hi = sum_poly&0x80000000; // 取得reg的最高位 
		sum_poly <<= 1; 
		if(hi) sum_poly = sum_poly^POLY; 
	} 
	return sum_poly; 
} 

void create_crc_table() 
{ 
	for(int i = 0; i < 256; i++) 
	{ 
		crc_table[i] = get_sum_poly(i&0xFF); 
	} 

	for (int i=0;i<256;i++)
	{
		char * bit_str = get_bit_string(crc_table[i]);
		printf("%d,%s \n",i,bit_str);
	}
}


unsigned int CRC32_2(char *p,int len)
{
	unsigned int reg = 0; 
	for(int i = 0; i < len; i++) 
	{ 
		reg = (reg<<8) ^ crc_table[(reg>>24)&0xFF ^ p[i]]; 
		char * bit_str = get_bit_string(reg);
		printf("%d,%s \n",i,bit_str);
	} 
	return reg;
}

#endif