#ifndef __TOOL_H__
#define __TOOL_H__
//把浮点数转换成定点数，不通过*(1<<fracbits)的方式，只通过移位操作。fracbits是定点数偏移
int float_to_fixpoint(float fval,int fracbits)
{
	int ival = *(int *)&fval;
	// 提取尾数,注意实际的尾数前面还有一个被省略掉的1
	int mantissa = (ival & 0x007fffff) | 0x800000;
	// 提取指数,以23分界
	int exponent =  (ival >> 23) & 0xff;
	exponent = exponent - 127;//得到真正的指数值
	if ( (fracbits+exponent) < 23)
		mantissa = mantissa>>( 23 - (fracbits+exponent));
	else
		mantissa = mantissa<<(  (fracbits+exponent) - 23);
	// 如果小于0，则将结果取反
	if ((*(int *)&fval) & 0x80000000)
		mantissa = -mantissa;
	//int vvv = fval * (1<<fracbits);
	return mantissa;
}
//把定点数转换成浮点数
float fixpoint_to_float(int i,int fractbits)
{
	float f;
	int *p = (int*)&f;
	*p=0;
	int is_negative=0;
	if (i<0)
	{
		is_negative = 1;
		i=-i;
	}
	//最高位bit所在位置
	int highest_bit_pos = 31;
	while (!((1<<highest_bit_pos) & i))
		highest_bit_pos--;
	int exponent = highest_bit_pos-fractbits;
	exponent = exponent + 127;
	//去掉最高位的1
	int mantissa = (i & (  ~(1<<highest_bit_pos) ));
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