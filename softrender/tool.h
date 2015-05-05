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

/*
(x0,y0)的向量逆时针旋转theta之后为(x1,y1)
其中:
x1 = x0*cos(θ) - y0*sin(θ)
y1 = x1*sin(θ) + y0*cos(θ)



x1 = cos(θ)*(	 x0				-		y0*tan(θ)	)
y1 = cos(θ)*(	 x0*tan(θ)		+		y0			）


因为:cos(θ) = 1/ sqrt[(1+tan(θ)^2)]
所以令k = cos(θ) = 1/ sqrt[(1+tan(θ)^2)]:

令θ为某些特定的值:arctan(π* 1/(2^n))，不同的n可以通过查表得到n=1,...n 相乘的结果
θ为:arctan(π* 1/2),arctan(π* 1/4),arctan( π* 1/8) ...
				|			|			|
				v			v			v
即θ为:	    0.46364      0.24497    0.124354     ...

tan(θ)为    π* 1/2      π* 1/4      π* 1/8

而tan(θ)则为 1/(2^n) ,这个值也可以通过查表得到



*/

float fast_tan(float rad)
{
	float angle_table[21] = {
		0.78539816339745f,
		0.46364760900081f,
		0.24497866312686f,
		0.12435499454676f,
		0.062418809995957f,

		0.031239833430268f,
		0.015623728620477f,
		0.0078123410601011f,
		0.003906230131967f,
		0.0019531225164788f,

		0.00097656218955932f,
		0.0004882812111949f,
		0.00024414062014936f,
		0.00012207031189367f,
		6.1035156174209e-005f,

		3.0517578115526e-005f,
		1.5258789061316e-005f,
		7.629394531102e-006f,
		3.8146972656065e-006f,
		1.9073486328102e-006f,

		9.5367431640596e-007f,
	};

	float k_table[21] = {
		0.70710678118655f,
		0.63245553203368f,
		0.6135719910779f,
		0.60883391251775f,
		0.60764825625617f,
		0.6073517701413f,
		0.60727764409353f,
		0.60725911229889f,
		0.60725447933256f,
		0.60725332108988f,
		0.60725303152913f,
		0.60725295913894f,
		0.6072529410414f,
		0.60725293651701f,
		0.60725293538591f,
		0.60725293510314f,
		0.60725293503245f,
		0.60725293501477f,
		0.60725293501035f,
		0.60725293500925f,
		0.60725293500897f
	};
	return 0.0f;
}

#endif