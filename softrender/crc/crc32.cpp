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
	//�����Ľ��г���
	//unsigned long poly_real64 = 0x100000000 | poly;//33bit�ģ���int�Ų���
	unsigned int poly_real32 = 0x80000000 | (poly>>1);

	int divide_table[256]={};
	for (int i=0;i<=255;i++)
	{
		unsigned int dividen = (i<< 24);
		unsigned int low_24bit_mask=0;//�ѳ��������жԵ�24bit��Ӱ���¼������xor������
		for (int right_shift=0;right_shift<8;right_shift++)
		{
			unsigned int divider = poly_real32>>right_shift;//����
			if ( (0x80000000>>right_shift) & dividen)//�������λ�ͱ��������λ����1���Ҷ�����
			{
				dividen = (dividen ^ divider) & 0xff000000;//ֻ��Ҫ���������8bit
				low_24bit_mask = low_24bit_mask ^ (poly << (7 - right_shift));//ȡ��32bit��Ӱ��
			}
			if (dividen==0)//�������Ѿ�����Ϊ0�ˣ�����
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
		unsigned int low_24bit_mask=0;//�ѳ��������жԵ�24bit��Ӱ���¼������xor������
		for (int j=0;j<8;j++)
		{
			if ( (0x80>>j) & dividen )//���λ1������
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

unsigned int CRC32(void* msg,int msg_len)
{
	unsigned int poly=0x04C11DB7;
	static int CRC32_DIVIDE_TABLE[256]={0};
	static int __INITIALIZER__ = crc32_gen_table(CRC32_DIVIDE_TABLE);

	char *m = (char*)msg;
	char *cur_m = m;
	unsigned int poly_effect = 0;
	while (cur_m - m < msg_len)
	{
		unsigned int b = (*cur_m) ^ (poly_effect>>24);
		unsigned int new_effect = CRC32_DIVIDE_TABLE[b];
		poly_effect = (poly_effect << 8) ^ (new_effect);
		cur_m++;
	}
	return poly_effect;
}