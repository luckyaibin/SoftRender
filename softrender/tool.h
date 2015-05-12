#ifndef __TOOL_H__
#define __TOOL_H__
#include "base_type.h"
#include "math_tool.h"
#include <math.h>


/*
(x0,y0)��������ʱ����תtheta֮��Ϊ(x1,y1)
����:
x1 = x0*cos(��) - y0*sin(��)
y1 = x0*sin(��) + y0*cos(��)



x1 = cos(��)*(	 x0				-		y0*tan(��)	)
y1 = cos(��)*(	 x0*tan(��)		+		y0			��


��Ϊ:cos(��) = 1/ sqrt[(1+tan(��)^2)]
������k = cos(��) = 1/ sqrt[(1+tan(��)^2)]:

���ΪĳЩ�ض���ֵ:arctan(��* 1/(2^n))����ͬ��n����ͨ�����õ�n=1,...n ��˵Ľ��
��Ϊ:arctan(��* 1/2),arctan(��* 1/4),arctan( ��* 1/8) ...
				|			|			|
				v			v			v
����Ϊ:	    0.46364      0.24497    0.124354     ...

tan(��)Ϊ    ��* 1/2      ��* 1/4      ��* 1/8

��tan(��)��Ϊ 1/(2^n) ,���ֵҲ����ͨ�����õ�

����������һ��У��ֵ����ȷ���Ƶ���
Xn = Xn-1 * cos(��n-1) - Yn-1 * sin(��n-1)
Yn = Yn-1 * sin(��n-1) + Yn-1 * cos(��n-1)

Xn+1 = Xn * cos(��n) - Yn * sin(��n)
Yn+1 = Xn * sin(��n) + Yn * cos(��n)
�������ݻ����Ͳʽ���Եõ�

Xn+1 = cos(��n-1 + ��n) * Xn-1 - sin(��n-1 + ��n) * Yn-1
Yn+1 = sin(��n-1 + ��n) * Xn-1 + cos(��n-1 + ��n) * Yn-1

��������X��Y��ֵ��ʵ����������ת�����нǶȼ�������cos��sinֵ���������x��y��ֵ
ֻ����ᣬ�о�˵�������ˡ���
*/
float atan_rad_table[21] = {
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

float k_accu_table[21] = {
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
float inverse_two_power[21] ={
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
float cos_table[] = 
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

void dump_triangle_value(float rad)
{
	float pi = 3.14159265335f;
	printf("����:%5.5f,�Ƕ�:%5.5f��,sin:%5.5f,cos:%5.5f,tan:%5.5f,atan:%5.5f\n",rad,rad/pi*180,sin(rad),cos(rad),tan(rad),atan(rad));
}
//����tanֵ�õ�����ֵ
float rad_by_tan(float tan_value)
{
	float pi = 3.14159265335f;
	float rad = atan(tan_value);
	printf("��ǰ����Ϊ:%5.5f,�Ƕ�Ϊ:%5.5f\n",rad,rad/pi*180);
	return rad;
}
//�����Ҫ�������õ���ȷ�ĸ�����x��yֵ
float fast_tan(float rad)
{
	dump_triangle_value(rad);
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
		rad_by_tan(y/x);
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
	rad_by_tan(adjusted_y/adjusted_x);
	return 0.0f;
}

//�����Ҫ�����ĸ�����xy��ֵ����Ϊ��ʼ����ʱ��x�Ѿ������������Ľ���ֵ
float fast_sin(float rad,float *p_sin=NULL,float *p_cos=NULL,float *p_tan=NULL)
{
	//�������ҵķ���
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
	printf("ras %10.40f :\n sinv_v is %3.40f ,\n cos_v is %3.40f\n\n",old_rad,sin_v,cos_v);

	return sin_v;
}

//�������汾
float fast_tan_fix_point_18(float float_rad)
{
	//��Щ���Ǹ�����������18bit֮��Ķ�����
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


//http://www.fx114.net/qa-171-97060.aspx
double my_atan4(double x, double y)  
{  
	const double tangent[] = {1.0, 1 / 2.0, 1 / 4.0, 1 / 8.0, 1 / 16.0,  
		1 / 32.0, 1 / 64.0, 1 / 128.0, 1 / 256.0, 1 / 512.0,  
		1 / 1024.0, 1 / 2048.0, 1 / 4096.0, 1 / 8192.0, 1 / 16384.0  
	};  
	const double angle[] = {45.0, 26.565051177078, 14.0362434679265, 7.1250163489018, 3.57633437499735,  
		1.78991060824607, 0.8951737102111, 0.4476141708606, 0.2238105003685, 0.1119056770662,  
		0.0559528918938, 0.027976452617, 0.01398822714227, 0.006994113675353, 0.003497056850704  
	};  

	int32_t i = 0;  
	double x_new, y_new;  
	double angleSum = 0.0;  

	for(i = 0; i < 15; i++)  
	{  
		if(y > 0)  
		{  
			x_new = x + y * tangent[i];  
			y_new = y - x * tangent[i];  
			x = x_new;  
			y = y_new;  
			angleSum += angle[i];  
		}  
		else  
		{  
			x_new = x - y * tangent[i];  
			y_new = y + x * tangent[i];  
			x = x_new;  
			y = y_new;  
			angleSum -= angle[i];  
		}  
		printf("Debug: i = %d angleSum = %f, angle = %f, �� = %f\n", i, angleSum, angle[i], hypot(x, y));  
	}  
	return angleSum;  
}  
#endif