#ifndef __TOOL_H__
#define __TOOL_H__
#include "base_type.h"
#include "math_tool.h"
#include "comm_headers.h"
#include <math.h>

/*
(x0,y0)的向量逆时针旋转theta之后为(x1,y1)
其中:
x1 = x0*cos(θ) - y0*sin(θ)
y1 = x0*sin(θ) + y0*cos(θ)

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

关于最后乘以一个校正值的正确性推导：
Xn = Xn-1 * cos(θn-1) - Yn-1 * sin(θn-1)
Yn = Yn-1 * sin(θn-1) + Yn-1 * cos(θn-1)

Xn+1 = Xn * cos(θn) - Yn * sin(θn)
Yn+1 = Xn * sin(θn) + Yn * cos(θn)
代入后根据积化和差公式可以得到

Xn+1 = cos(θn-1 + θn) * Xn-1 - sin(θn-1 + θn) * Yn-1
Yn+1 = sin(θn-1 + θn) * Xn-1 + cos(θn-1 + θn) * Yn-1

所以最后的X，Y的值其实等于连续旋转的所有角度加起来的cos和sin值乘以最初的x，y的值
只可意会，感觉说不明白了。。
*/
static float atan_rad_table[21] = {
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

static float k_accu_table[21] = {
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
// 1/(2^0) ,1/(2^1) ... 1/(2^n)
static float inverse_two_power[21] ={
	1.0f,	 
	0.5f,	 
	0.25f,	 
	0.125f,
	0.0625f,
	0.03125f,
	0.015625f,
	0.0078125f,
	0.00390625f,
	0.001953125f,
	0.0009765625f,
	0.00048828125f,
	0.000244140625f,
	0.0001220703125f,
	6.103515625e-005f,
	3.0517578125e-005f,
	1.52587890625e-005f,
	7.62939453125e-006f,
	3.814697265625e-006f,
	1.9073486328125e-006f,
	9.5367431640625e-007f,
};
static float cos_table[] = 
{
	0.70710678118655,
	0.89442719099992,
	0.97014250014533,
	0.99227787671367,
	0.99805257848289,
	0.99951207608708,
	0.9998779520347,
	0.99996948381879,
	0.99999237069278,
	0.99999809265682,
	0.99999952316318,
	0.99999988079073,
	0.99999997019768,
	0.99999999254942,
	0.99999999813735,
	0.99999999953434,
	0.99999999988358,
	0.9999999999709,
	0.99999999999272,
	0.99999999999818,
	0.99999999999955,
};
//最后需要矫正来得到正确的浮点数x和y值(rad是0 ~ PI/2)
inline float __todo_fast_tan(float rad)
{
	float z = 0;
	float curr_x = 1;
	float curr_y = 0;
	float final_x = curr_x*cos(rad) - curr_y*sin(rad);
	float final_y = curr_x*sin(rad) + curr_y*cos(rad);
	int32_t d = 1;
	int32_t iter = 0;
	while (iter<=20)
	{
		float x = curr_x  - d*curr_y*inverse_two_power[iter];
		float y = d*curr_x*inverse_two_power[iter] + curr_y;
		z = z + d*atan_rad_table[iter];
		curr_x = x;
		curr_y = y;
		iter++;
		if (z>rad)
		{
			d = -1;
		}
		else
			d = +1;
	}
	float adjusted_x = k_accu_table[20] * curr_x;
	float adjusted_y = k_accu_table[20] * curr_y;
	return 0.0f;
}

//最后不需要矫正的浮点数xy的值，因为初始化的时候x已经被乘以了最后的矫正值
inline float fast_sin_cordic(float rad,float *p_sin=NULL,float *p_cos=NULL,float *p_tan=NULL)
{
	//正弦余弦的符号
	int32_t sin_sign = 1;
	int32_t cos_sign = 1;
	float old_rad = rad;
	rad = fmod(rad,TWO_PI);  
	if (rad < 0)
	{ 
		sin_sign = -sin_sign;
		rad = - rad;
	}
	if (rad >= PI)
	{
		rad = rad - PI;
		sin_sign = -sin_sign;
		cos_sign = -cos_sign;
	}
	if (rad> HALF_PI)
	{
		rad = rad-HALF_PI;
		sin_sign = sin_sign*1;
		cos_sign=  cos_sign*(-1);
	}
	//dump_triangle_value(rad);
	float z = 0;
	float curr_x = k_accu_table[20];
	float curr_y = 0;
	int32_t d = 1;
	int32_t iter = 0;
	while (iter<=20)
	{
		float x = curr_x  - d*curr_y*inverse_two_power[iter];
		float y = d*curr_x*inverse_two_power[iter] + curr_y;
		//rad_by_tan(y/x);
		z = z + d*atan_rad_table[iter];
		curr_x = x;
		curr_y = y;
		iter++;
		if (z>rad)
		{
			d = -1;
		}
		else
			d = +1;
	}
	float cos_v = curr_x*cos_sign;
	float sin_v = curr_y*sin_sign;
	//printf("cos is %5.5f,sin is %5.5f\n",cos_v*cos_sign,sin_v*sin_sign);
	if (p_sin)
		*p_sin = sin_v;
	if (p_cos)
		*p_cos = cos_v;
	if (p_tan)
		*p_tan = sin_v/cos_v;
	//rad_by_tan(adjusted_y/adjusted_x);
	//printf("mine	%10.40f :\n sinv_v is %3.40f ,\n cos_v is %3.40f\n\n",old_rad,sin_v,cos_v);
	//printf("correct %10.40f :\n sinv_v is %3.40f ,\n cos_v is %3.40f\n\n",old_rad,sin(old_rad),cos(old_rad));
	return sin_v;
}

//定点数版本的CORDIC算法float_rad属于 :0~PI/2
inline float fast_sin_cos_fix_point_18(float float_rad)
{
	//这些都是浮点数左移了18bit之后的定点数
	int32_t k_accu_table[21] = {
		185363,
		165794,
		160844,
		159602,
		159291,
		159213,
		159194,
		159189,
		159188,
		159187,
		159187,
		159187,
		159187,
		159187,
		159187,
		159187,
		159187,
		159187,
		159187,
		159187,
		159187,
	};
	int32_t atan_rad_table[21]={
		205887,
		121542,
		64219,
		32598,
		16362,
		8189,
		4095,
		2047,
		1023,
		511,
		255,
		127,
		64,
		32,
		16,
		8,
		4,
		2,
		1,
		0,
		0,
	};
	//dump_triangle_value(rad);
	int32_t rad = float_to_fixpoint(float_rad,18);

	int32_t z = 0;
	int32_t curr_x = k_accu_table[20];
	int32_t curr_y = 0;
	
	int32_t d = 1;
	int32_t iter = 0;
	while (iter<=20)
	{
		int32_t x,y;
		if (d>0)
		{
			x = curr_x  - (curr_y>>iter);
			y = (curr_x>>iter) + curr_y;
			z = z + atan_rad_table[iter];
		}
		else
		{
			x = curr_x  + (curr_y>>iter);
			y = curr_y - (curr_x>>iter) ;
			z = z - atan_rad_table[iter];
		}
		//printf("%5.5f,%f5.5\n",fixpoint_to_float(x,18),fixpoint_to_float(y,18));
		//rad_by_tan((float)y/x);
		curr_x = x;
		curr_y = y;
		iter++;
		if (z>rad)
		{
			d = -1;
		}
		else
			d = +1;
	}
	int32_t cos_v = curr_x;
	int32_t sin_v = curr_y;

	float f_cos_v = fixpoint_to_float(cos_v,18);
	float f_sin_v = fixpoint_to_float(sin_v,18);

	//rad_by_tan(adjusted_y/adjusted_x);
	return f_sin_v;
}

//角度x: 0 ~ PI
inline float fast_sin_parabola__(float x)
{
	float y = x*(const_fast_sin_4_d_pi - const_fast_sin_4_d_pi2*x);
	y = y*(const_fast_sin_Q + const_fast_sin_P*y);
	return y;
}

inline float fast_sin()
{
	return 0;
}



//from http://forum.devmaster.net/t/fast-and-accurate-sine-cosine/9648/63
//Andy201
inline float fastAcos(float x)
{
	return sqrt(1-x)*(1.5707963267948966192313216916398f + x*(-0.213300989f + x*(0.077980478f + x*-0.02164095f)));
}

inline double isqrt_approx_in_neighborhood(double s, double neighborhood, double scale) {
	return scale / sqrt(neighborhood) + scale * (s - neighborhood) * (-0.5 * 1.0 / (neighborhood * sqrt(neighborhood)));
}

inline double fast_normalize(double s, double neighborhood, double scale) {
	double factor = 1;
	const double LIMIT = 0.9995;
	const double limit2 = LIMIT * LIMIT;

	int ntries = 0;

	while (factor * factor * s < limit2) {
		double new_factor = fast_normalize(factor * factor * s,
			neighborhood, scale);
		factor *= new_factor;

		// 'ntries' is just a mechanism to keep us from infinite-looping
		// while we are experimenting.

		ntries++;
		if (ntries > 8) break;
	}

	return factor;
}
//求 1/sqrt(v)http://www.matrix67.com/data/InvSqrt.pdf
inline float InvSqrt(float x)
{ 
	float xhalf = 0.5f*x;
	int i = *(int*)&x;       // get bits for floating value
	i = 0x5f375a86- (i>>1);  // gives initial guess y0
	x = *(float*)&i;         // convert bits back to float
	x = x*(1.5f-xhalf*x*x);  // Newton step, repeating increases accuracy
	//one more repeat
	//x = x*(1.5f-xhalf*x*x);
	return x;
}
inline float mod_pi(float x)
{
	const float pi = PI;
	float x1 = x * (1.0f / pi);
	return pi * (x1 - (int)x1);
}

#endif