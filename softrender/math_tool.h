#ifndef __MATH_TOOL_H__
#define __MATH_TOOL_H__
#include "base_type.h"
#include <float.h>
//指数全为1，尾数全为0表示无穷，正、负无穷看符号位决定
#define IS_FLOAT_INFINITE(f) ( (((FLOAT_FORMAT*)(&f))->exponent == 0xFF) && (((FLOAT_FORMAT*)(&f))->mantissa == 0) )

#define IS_FLOAT_NEGATIVE_INFINITE(f) ( IS_FLOAT_INFINITE(f) && ( ((FLOAT_FORMAT*)(&f))->sign == 1))

#define IS_FLOAT_POSITIVE_INFINITE(f) ( IS_FLOAT_INFINITE(f) && ( ((FLOAT_FORMAT*)(&f))->sign == 0))

//NaN，not a number，指数全为1，尾数不为0
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
//get most significant bit,获取最高位为1的bit索引
int32_t get_msb1(uint32_t n)
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
int32_t get_msb2(uint32_t n)
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

int32_t get_msb_noloop(uint32_t n)
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




//把浮点数转换成定点数，不通过*(1<<fracbits)的方式，只通过移位操作。fracbits是定点数偏移
int32_t float_to_fixpoint(float fval,int32_t fracbits)
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


#endif