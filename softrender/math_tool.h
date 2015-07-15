#ifndef __MATH_TOOL_H__
#define __MATH_TOOL_H__
#include "base_type.h"
#include <float.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
//指数全为1，尾数全为0表示无穷，正、负无穷看符号位决定
#define IS_FLOAT_INFINITE(f) ( (((FLOAT_FORMAT*)(&f))->exponent == 0xFF) && (((FLOAT_FORMAT*)(&f))->mantissa == 0) )

#define IS_FLOAT_NEGATIVE_INFINITE(f) ( IS_FLOAT_INFINITE(f) && ( ((FLOAT_FORMAT*)(&f))->sign == 1))

#define IS_FLOAT_POSITIVE_INFINITE(f) ( IS_FLOAT_INFINITE(f) && ( ((FLOAT_FORMAT*)(&f))->sign == 0))

//判断是不是非规格化浮点数，非规格浮点数的exponent部分是全是0
//最小的规格化浮点数是FLT_MIN，即1.175494351e-38F:0000 0000 1000 0000 0000 0000 0000 0000
//-38其实是由指数部分的1，加上127:01111111，得到的10000000:-128，换成10进制为10e-38
//隐含的右数第23位有效部分为1，
//最大的非规格化浮点数是
#define IS_FLOAT_DENORMAL(f) (((FLOAT_FORMAT*)(&f))->exponent == 0x00)

//NaN，not a number，指数全为1，尾数不为0
#define IS_FLOAT_NAN(f) ( (((FLOAT_FORMAT*)(&f))->exponent == 0xFF) && (((FLOAT_FORMAT*)(&f))->mantissa) )

//一个数字不是有效的数字（是正负无穷和Nan）
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
	//printf("得到结果 %5.60f\n",res_float);
	return res_float;
}
/*
Float value	原始的Hexadecimal	负数则用0x80000000减去原始Hexadecimal				Decimal
4.20E-45		0x00000003				0x00000003										3
2.80E-45		0x00000002				0x00000002										2
1.40E-45		0x00000001				0x00000001										1
0				0x00000000				0x00000000										0
-1.40E-45		0x80000001				0xFFFFFFFF										-1
-2.80E-45		0x80000002				0xFFFFFFFE										-2
-4.20E-45		0x80000003				0xFFFFFFFD										-3
*/
//转换为整数进行比较，如果有浮点数数为负数，转为对应的负正数表示，在相减
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
//get most significant bit,获取最高位为1的bit索引
inline int32_t get_msb1(uint32_t n)
{
	int32_t lvl=4;
	int32_t msb=0;
	while (lvl>=0)
	{
		//高bit部分大于0
		//取高位部分
		if( n>>(1<<lvl) & ( (1<<(1<<lvl)) - 1)) //高位大于0 (1<<(lvl+1) - 1)是mask
		{
			n=n>>(1<<lvl);
			msb = msb + (1<<lvl);
		}
		else //取低位部分
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
		//高bit部分大于0
		//取高位部分
		int32_t shift = 1<<lvl;
		if( n>>shift ) //高位大于0 (1<<(lvl+1) - 1)是mask
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
	else //取低位部分
	{
		n=n & 0xFFFF ; 
	}
	//////////////////////lvl=3
	if( n>>8 & 0xFF )
	{
		n=n>>8;
		msb = msb + 8;
	}
	else //取低位部分
	{
		n=n & 0xFF; 
	}
	///////////////////////lvl=2
	if( n>>4 & 0xF) 
	{
		n=n>>4;
		msb = msb + 4;
	}
	else //取低位部分
	{
		n=n & 0xF; 
	}
	//////////////////////lvl=1
	if( n>>2 & 0x3)
	{
		n=n>>2;
		msb = msb + 2;
	}
	else //取低位部分
	{
		n=n & 0x3; 
	}
	/////////////////////lvl=0
	if( n>>1 & 0x1)
	{
		//n=n>>1;last step,no need to shift n;
		msb = msb + 1;
	}
	else //取低位部分
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




//把浮点数转换成定点数，不通过*(1<<fracbits)的方式，只通过移位操作。fracbits是定点数偏移
inline int32_t float_to_fixpoint(float fval,int32_t fracbits)
{
	int32_t ival = *(int32_t *)&fval;
	// 提取尾数,注意实际的尾数前面还有一个被省略掉的1
	int32_t mantissa = (ival & 0x007fffff) | 0x800000;
	// 提取指数,以23分界
	int32_t exponent =  (ival >> 23) & 0xff;
	exponent = exponent - 127;//得到真正的指数值
	if ( (fracbits+exponent) < 23)
		mantissa = mantissa>>( 23 - (fracbits+exponent));
	else
		mantissa = mantissa<<(  (fracbits+exponent) - 23);
	// 如果小于0，则将结果取反
	if ((*(int32_t *)&fval) & 0x80000000)
		mantissa = -mantissa;
	//int32_t vvv = fval * (1<<fracbits);
	return mantissa;
}
//把定点数转换成浮点数
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
	//最高位bit所在位置
	/*int32_t highest_bit_pos = 31;
	while (!((1<<highest_bit_pos) & i))
		highest_bit_pos--;*/
	int32_t highest_bit_pos = get_msb_noloop0(i);
	int32_t exponent = highest_bit_pos-fractbits;
	exponent = exponent + 127;
	//去掉最高位的1
	int32_t mantissa = (i & (  ~(1<<highest_bit_pos) ));
	//再把最高位右边那个bit移位到第23个bit的位置,对齐
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