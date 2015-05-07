#ifndef __TOOL_H__
#define __TOOL_H__
#include <math.h>
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
	printf("弧度:%5.5f,角度:%5.5f°,sin:%5.5f,cos:%5.5f,tan:%5.5f,atan:%5.5f\n",rad,rad/pi*180,sin(rad),cos(rad),tan(rad),atan(rad));
}
//根据tan值得到弧度值
float rad_by_tan(float tan_value)
{
	float pi = 3.14159265335f;
	float rad = atan(tan_value);
	printf("当前弧度为:%5.5f,角度为:%5.5f\n",rad,rad/pi*180);
	return rad;
}
//最后需要矫正来得到正确的浮点数x和y值
float fast_tan(float rad)
{
	dump_triangle_value(rad);
	float z = 0;
	float curr_x = 1;
	float curr_y = 0;
	float final_x = curr_x*cos(rad) - curr_y*sin(rad);
	float final_y = curr_x*sin(rad) + curr_y*cos(rad);
	int d = 1;
	int iter = 0;
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

//最后不需要矫正的浮点数xy的值，因为初始化的时候x已经被乘以了最后的矫正值
float fast_tan2(float rad)
{
	//dump_triangle_value(rad);
	float z = 0;
	float curr_x = k_accu_table[20];
	float curr_y = 0;
	int d = 1;
	int iter = 0;
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
	float cos_v = curr_x;
	float sin_v = curr_y;
	//rad_by_tan(adjusted_y/adjusted_x);
	return 0.0f;
}

//定点数版本
float fast_tan_fix_point_18(float float_rad)
{
	//这些都是浮点数左移了18bit之后的定点数
	int k_accu_table[21] = {
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
	int atan_rad_table[21]={
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
	int rad = float_to_fixpoint(float_rad,18);

	int z = 0;
	int curr_x = k_accu_table[20];
	int curr_y = 0;
	
	int d = 1;
	int iter = 0;
	while (iter<=10)
	{
		int x,y;
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
	int cos_v = curr_x;
	int sin_v = curr_y;

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

	int i = 0;  
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
		printf("Debug: i = %d angleSum = %f, angle = %f, ρ = %f\n", i, angleSum, angle[i], hypot(x, y));  
	}  
	return angleSum;  
}  
#endif