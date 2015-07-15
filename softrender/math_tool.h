#ifndef __MATH_TOOL_H__
#define __MATH_TOOL_H__
#include "base_type.h"
#include <float.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
//ָ��ȫΪ1��β��ȫΪ0��ʾ����������������λ����
#define IS_FLOAT_INFINITE(f) ( (((FLOAT_FORMAT*)(&f))->exponent == 0xFF) && (((FLOAT_FORMAT*)(&f))->mantissa == 0) )

#define IS_FLOAT_NEGATIVE_INFINITE(f) ( IS_FLOAT_INFINITE(f) && ( ((FLOAT_FORMAT*)(&f))->sign == 1))

#define IS_FLOAT_POSITIVE_INFINITE(f) ( IS_FLOAT_INFINITE(f) && ( ((FLOAT_FORMAT*)(&f))->sign == 0))

//�ж��ǲ��Ƿǹ�񻯸��������ǹ�񸡵�����exponent������ȫ��0
//��С�Ĺ�񻯸�������FLT_MIN����1.175494351e-38F:0000 0000 1000 0000 0000 0000 0000 0000
//-38��ʵ����ָ�����ֵ�1������127:01111111���õ���10000000:-128������10����Ϊ10e-38
//������������23λ��Ч����Ϊ1��
//���ķǹ�񻯸�������
#define IS_FLOAT_DENORMAL(f) (((FLOAT_FORMAT*)(&f))->exponent == 0x00)

//NaN��not a number��ָ��ȫΪ1��β����Ϊ0
#define IS_FLOAT_NAN(f) ( (((FLOAT_FORMAT*)(&f))->exponent == 0xFF) && (((FLOAT_FORMAT*)(&f))->mantissa) )

//һ�����ֲ�����Ч�����֣������������Nan��
#define IS_FLOAT_INVALID_NUM(f) (  IS_FLOAT_INFINITE(f)  ||  IS_FLOAT_NAN(f)   )

#define ABS(x) (x>0)?(x):(-x)

inline uint32_t dump_float(float f)
{
	FLOAT_UINT32_UNION *fu = (FLOAT_UINT32_UNION*)&f;
	for (int i=0;i<32;i++)
	{
		uint32_t bit = (fu->ui & (1<<(31-i)));
		printf("%d",bit>0?1:0);
		if ( (i+1)%4==0)
		{
			printf(" ");
		}
	}
	printf("\n");
	return fu->ui;
}

inline float float_from_bit_array(uint32_t bit_arr[],uint32_t size)
{
	float res_float = 0.0f;
	FLOAT_UINT32_UNION *fu = (FLOAT_UINT32_UNION*)&res_float;
	int32_t index = 0;
	while (index < size)
	{
		uint32_t bit = !!bit_arr[index];
		fu->ui |= (bit<<(31-index)); 
		index++;
	}
	//printf("�õ���� %5.60f\n",res_float);
	return res_float;
}
/*
Float value	ԭʼ��Hexadecimal	��������0x80000000��ȥԭʼHexadecimal				Decimal
4.20E-45		0x00000003				0x00000003										3
2.80E-45		0x00000002				0x00000002										2
1.40E-45		0x00000001				0x00000001										1
0				0x00000000				0x00000000										0
-1.40E-45		0x80000001				0xFFFFFFFF										-1
-2.80E-45		0x80000002				0xFFFFFFFE										-2
-4.20E-45		0x80000003				0xFFFFFFFD										-3
*/
//ת��Ϊ�������бȽϣ�����и�������Ϊ������תΪ��Ӧ�ĸ�������ʾ�������
inline int32_t is_float_equal(float f1,float f2,int32_t maxUlps)
{
	if (f1==f2)
	{
		return 1;
	}
	int32_t i1,i2;
	if (((FLOAT_UINT32_UNION*)(&f1))->ui < 0)
	{
		i1 = 0x80000000 - (((FLOAT_UINT32_UNION*)(&f1))->ui);
	}
	if (((FLOAT_UINT32_UNION*)(&f2))->ui < 0)
	{
		i2 = 0x80000000 - (((FLOAT_UINT32_UNION*)(&f2))->ui);
	}

	int intDiff = i1-i2;
	intDiff = (intDiff>0)?(intDiff):(-intDiff);
	if (intDiff <= maxUlps)
	{
		return 1;
	}
	return 0;
}

inline uint32_t float_diff_to_ULP(float diff)
{
	FLOAT_UINT32_UNION fuu;
	fuu.f = diff;
	return fuu.ui;
}
union Float_t
{
	Float_t(float num = 0.0f) : f(num) {}
	// Portable extraction of components.
	bool Negative() const { return (i >> 31) != 0; }
	int32_t RawMantissa() const { return i & ((1 << 23) - 1); }
	int32_t RawExponent() const { return (i >> 23) & 0xFF; }

	int32_t i;
	float f;
#ifdef _DEBUG
	struct
	{   // Bitfields for exploration. Do not use in production code.
		uint32_t mantissa : 23;
		uint32_t exponent : 8;
		uint32_t sign : 1;
	} parts;
#endif
};

inline void IterateAllPositiveFloats(float start_float)
{
	// Start at zero and print that float.
	Float_t allFloats;
	allFloats.f = start_float;
	printf("%1.8e\n", allFloats.f);
	dump_float(allFloats.f);

	// Continue through all of the floats, stopping
	// when we get to positive infinity.
	while (allFloats.RawExponent() < 255)
	{
		// Increment the integer representation
		// to move to the next float.
		allFloats.i += 1;
		printf("%1.8e ", allFloats.f);
		dump_float(allFloats.f);
	}
}
inline int32_t get_msb0(uint32_t n)
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
inline int32_t get_msb1(uint32_t n)
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
inline int32_t get_msb2(uint32_t n)
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

inline int32_t get_msb_noloop(uint32_t n)
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




inline int32_t get_msb_noloop0(uint32_t n)
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
inline int32_t float_to_fixpoint(float fval,int32_t fracbits)
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
inline float fixpoint_to_float(int32_t i,int32_t fractbits)
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
 
inline int _matherr__( struct _exception *except )
{
	/* Handle _DOMAIN errors for log or log10. */
	if( except->type == _DOMAIN )
	{
		if( strcmp( except->name, "log" ) == 0 )
		{
			except->retval = log( -(except->arg1) );
			printf( "Special: using absolute value: %s: _DOMAIN "
				"error\n", except->name );
			return 1;
		}
		else if( strcmp( except->name, "log10" ) == 0 )
		{
			except->retval = log10( -(except->arg1) );
			printf( "Special: using absolute value: %s: _DOMAIN "
				"error\n", except->name );
			return 1;
		}
	}
	else
	{
		printf( "Normal: " );
		return 0;    /* Else use the default actions */
	}
}
#endif