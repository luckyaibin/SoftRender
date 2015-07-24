#ifndef __CRC32_H__
#define __CRC32_H__

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

unsigned int crc32_gen_table()
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

	for ( int i=0;i<256;i++)
	{
		unsigned int m = divide_table[i];
		char * bit_str = get_bit_string(m);
		printf("%d,%s \n",i,bit_str);
	}
	return 0;
}


unsigned int crc32_gen_table2()
{
	unsigned int poly=0x04C11DB7;
	int divide_table[256]={};
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

	for ( int i=0;i<256;i++)
	{
		unsigned int m = divide_table[i];
		char * bit_str = get_bit_string(m);
		printf("%d,%s \n",i,bit_str);
	}
	return 0;
}

#endif