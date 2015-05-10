#ifndef __MATH_TOOL_H__
#define __MATH_TOOL_H__
#include "base_type.h"
#include <float.h>
//ָ��ȫΪ1��β��ȫΪ0��ʾ����������������λ����
#define IS_FLOAT_INFINITE(f) ( (((FLOAT_FORMAT*)(&f))->exponent == 0xFF) && (((FLOAT_FORMAT*)(&f))->mantissa == 0) )

#define IS_FLOAT_NEGATIVE_INFINITE(f) ( IS_FLOAT_INFINITE(f) && ( ((FLOAT_FORMAT*)(&f))->sign == 1))

#define IS_FLOAT_POSITIVE_INFINITE(f) ( IS_FLOAT_INFINITE(f) && ( ((FLOAT_FORMAT*)(&f))->sign == 0))

//NaN��not a number��ָ��ȫΪ1��β����Ϊ0
#define IS_FLOAT_NAN(f) ( (((FLOAT_FORMAT*)(&f))->exponent == 0xFF) && (((FLOAT_FORMAT*)(&f))->mantissa) )

int32_t is_float_equal(float f1,float f2,int32_t maxUlps)
{
	if (f1==f2)
	{
		return 1;
	}
	int32_t i1,i2;
	if (((FLOAT_UINT32_UNION*)(&f1))->ui < 0)
		i1 = (((FLOAT_UINT32_UNION*)(&f1))->ui)-0x80000000;
	if (((FLOAT_UINT32_UNION*)(&f2))->ui < 0)
		i2 = (((FLOAT_UINT32_UNION*)(&f2))->ui)-0x80000000;

	int intDiff = i1-i2;
	intDiff = (intDiff>0)?(intDiff):(-intDiff);

}
int32_t get_msb0(uint32_t n)
{
	int32_t highest_bit_pos = 31;
	if (0==n)
	{
		return 0;
	}
	while (!((1<<highest_bit_pos) & n))
		highest_bit_pos--;
	return highest_bit_pos;
}
//get most significant bit,��ȡ���λΪ1��bit����
int32_t get_msb1(uint32_t n)
{
	int32_t lvl=4;
	int32_t msb=0;
	while (lvl>=0)
	{
		//��bit���ִ���0
		//ȡ��λ����
		if( n>>(1<<lvl) & ( (1<<(1<<lvl)) - 1)) //��λ����0 (1<<(lvl+1) - 1)��mask
		{
			n=n>>(1<<lvl);
			msb = msb + (1<<lvl);
		}
		else //ȡ��λ����
		{
			n=n & ( (1<<(1<<lvl))-1);
		}
		lvl--;
	}
	//printf("msb is %d\n",msb);
	return msb;
}
int32_t get_msb2(uint32_t n)
{
	int32_t lvl=4;
	int32_t msb=0;
	while (lvl>=0)
	{
		//��bit���ִ���0
		//ȡ��λ����
		int32_t shift = 1<<lvl;
		if( n>>shift ) //��λ����0 (1<<(lvl+1) - 1)��mask
		{
			n=n>>shift;
			msb = msb + shift;
		}
		lvl--;
	}
	//printf("msb is %d\n",msb);
	return msb;
}

int32_t get_msb_noloop(uint32_t n)
{
	int32_t msb=0;
	///////////////////////lvl=4
	if( n>>16 &  0xFFFF ) 
	{
		n=n>>16;
		msb = msb + 16;
	}
	else //ȡ��λ����
	{
		n=n & 0xFFFF ; 
	}
	//////////////////////lvl=3
	if( n>>8 & 0xFF )
	{
		n=n>>8;
		msb = msb + 8;
	}
	else //ȡ��λ����
	{
		n=n & 0xFF; 
	}
	///////////////////////lvl=2
	if( n>>4 & 0xF) 
	{
		n=n>>4;
		msb = msb + 4;
	}
	else //ȡ��λ����
	{
		n=n & 0xF; 
	}
	//////////////////////lvl=1
	if( n>>2 & 0x3)
	{
		n=n>>2;
		msb = msb + 2;
	}
	else //ȡ��λ����
	{
		n=n & 0x3; 
	}
	/////////////////////lvl=0
	if( n>>1 & 0x1)
	{
		//n=n>>1;last step,no need to shift n;
		msb = msb + 1;
	}
	else //ȡ��λ����
	{
		//last step,no need to get n's half low bits 
		//n=n & ( 1 ); 
	}

	//printf("msb is %d\n",msb);
	return msb;
}




int32_t get_msb_noloop0(uint32_t n)
{
	int32_t msb=0;
	int32_t m;
	m = n>>16;
	if(m) 
	{
		n=m;
		msb = msb + 16;
	}
	m=n>>8;
	if(m)
	{
		n=m;
		msb = msb + 8;
	}
	m=n>>4;
	if(m) 
	{
		n=m;
		msb = msb + 4;
	}
	m=n>>2;
	if(m)
	{
		n=m;
		msb = msb + 2;
	}
	m=n>>1;
	if( m )
	{
		//n=m;last step,no need to shift n;
		msb = msb + 1;
	}
	//printf("msb is %d\n",msb);
	return msb;
}




//�Ѹ�����ת���ɶ���������ͨ��*(1<<fracbits)�ķ�ʽ��ֻͨ����λ������fracbits�Ƕ�����ƫ��
int32_t float_to_fixpoint(float fval,int32_t fracbits)
{
	int32_t ival = *(int32_t *)&fval;
	// ��ȡβ��,ע��ʵ�ʵ�β��ǰ�滹��һ����ʡ�Ե���1
	int32_t mantissa = (ival & 0x007fffff) | 0x800000;
	// ��ȡָ��,��23�ֽ�
	int32_t exponent =  (ival >> 23) & 0xff;
	exponent = exponent - 127;//�õ�������ָ��ֵ
	if ( (fracbits+exponent) < 23)
		mantissa = mantissa>>( 23 - (fracbits+exponent));
	else
		mantissa = mantissa<<(  (fracbits+exponent) - 23);
	// ���С��0���򽫽��ȡ��
	if ((*(int32_t *)&fval) & 0x80000000)
		mantissa = -mantissa;
	//int32_t vvv = fval * (1<<fracbits);
	return mantissa;
}
//�Ѷ�����ת���ɸ�����
float fixpoint_to_float(int32_t i,int32_t fractbits)
{
	float f;
	int32_t *p = (int32_t*)&f;
	*p=0;
	int32_t is_negative=0;
	if (i<0)
	{
		is_negative = 1;
		i=-i;
	}
	//���λbit����λ��
	/*int32_t highest_bit_pos = 31;
	while (!((1<<highest_bit_pos) & i))
		highest_bit_pos--;*/
	int32_t highest_bit_pos = get_msb_noloop0(i);
	int32_t exponent = highest_bit_pos-fractbits;
	exponent = exponent + 127;
	//ȥ�����λ��1
	int32_t mantissa = (i & (  ~(1<<highest_bit_pos) ));
	//�ٰ����λ�ұ��Ǹ�bit��λ����23��bit��λ��,����
	if (highest_bit_pos-1<22)
		mantissa = mantissa << (22 - (highest_bit_pos - 1));
	else
		mantissa = mantissa >> ( (highest_bit_pos-1) - 22 );
	*p = exponent;
	*p = (*p << 23);
	*p = (*p)|mantissa;
	if (is_negative)
		f = -f;
	return f;
}


#endif