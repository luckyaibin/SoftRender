#include "crc32.h"




char * get_bit_string(unsigned int v)
{
	static char bit_array[33] = {0};
	static char bit_array_seperated[100] = {0};

	for (int i=31;i>=0;i--)
	{
		int bit = (v>>i) & 1;
		bit_array[31-i]='0'+bit;
	}
	int comma_num=0;
	for (int i=0;i<=31;i++)
	{
		if ((i+1)%8==0)
		{
			bit_array_seperated[i+comma_num]=bit_array[i];
			comma_num++;
			bit_array_seperated[i+comma_num]=',';
		}
		else
			bit_array_seperated[i+comma_num]=bit_array[i];
	}
	return bit_array_seperated;
}

unsigned int crc32_gen_table1()
{
	unsigned int poly=0x04C11DB7;
	//真正的进行除法
	//unsigned long poly_real64 = 0x100000000 | poly;//33bit的，用int放不下
	unsigned int poly_real32 = 0x80000000 | (poly>>1);

	int divide_table[256]={};
	for (int i=0;i<=255;i++)
	{
		unsigned int dividen = (i<< 24);
		unsigned int low_24bit_mask=0;//把除法过程中对低24bit的影响记录下来（xor下来）
		for (int right_shift=0;right_shift<8;right_shift++)
		{
			unsigned int divider = poly_real32>>right_shift;//除数
			if ( (0x80000000>>right_shift) & dividen)//除数最高位和被除数最高位都是1并且对其了
			{
				dividen = (dividen ^ divider) & 0xff000000;//只需要被除数最高8bit
				low_24bit_mask = low_24bit_mask ^ (poly << (7 - right_shift));//取出32bit的影响
			}
			if (dividen==0)//被除数已经被除为0了，结束
			{
				divide_table[i]=low_24bit_mask;
				break;
			}
		}
	}

	/*for ( int i=0;i<256;i++)
	{
	unsigned int m = divide_table[i];
	char * bit_str = get_bit_string(m);
	printf("%d,%s \n",i,bit_str);
	}*/
	return 0;
}

unsigned int crc32_gen_table(int divide_table[256])
{
	unsigned int poly=0x04C11DB7;
	//int divide_table[256]={};
	for (int i=0;i<=255;i++)
	{
		unsigned int dividen = i;
		unsigned int divider = 0x80 | (poly>>25);
		unsigned int low_24bit_mask=0;//把除法过程中对低24bit的影响记录下来（xor下来）
		for (int j=0;j<8;j++)
		{
			if ( (0x80>>j) & dividen )//最高位1对齐了
			{
				dividen = dividen ^ divider;
				low_24bit_mask = low_24bit_mask ^ (poly << (7-j));
			}
			divider = divider >> 1;
			if (dividen==0)
			{
				divide_table[i] = low_24bit_mask;
				break;
			}
		}
	}
	return 0;
}

unsigned int CRC32(void* msg,int msg_len,unsigned int initial_effect)
{
	unsigned int poly=0x04C11DB7;
	static int CRC32_DIVIDE_TABLE[256]={0};
	static int __INITIALIZER__ = crc32_gen_table(CRC32_DIVIDE_TABLE);

	char *m = (char*)msg;
	char *cur_m = m;
	 
	unsigned int poly_effect = initial_effect;
	while (cur_m - m < msg_len)
	{
		unsigned int b = (*cur_m) ^ (poly_effect>>24);
		unsigned int new_effect = CRC32_DIVIDE_TABLE[b];
		poly_effect = (poly_effect << 8) ^ (new_effect);
		cur_m++;
	}
	return poly_effect^initial_effect;
}

unsigned int get_sum_poly_others(unsigned char data)
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

void create_crc_table_others()
{
	for(int i = 0; i < 256; i++) 
	{ 
		crc_table[i] = get_sum_poly_others(i&0xFF); 
	} 

	for (int i=0;i<256;i++)
	{
		char * bit_str = get_bit_string(crc_table[i]);
		printf("%d,%s \n",i,bit_str);
	}
}

unsigned int CRC32_others(char *p,int len)
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
