#ifndef __TOOL_H__
#define __TOOL_H__
//�Ѹ�����ת���ɶ���������ͨ��*(1<<fracbits)�ķ�ʽ��ֻͨ����λ������fracbits�Ƕ�����ƫ��
int float_to_fixpoint(float fval,int fracbits)
{
	int ival = *(int *)&fval;
	// ��ȡβ��,ע��ʵ�ʵ�β��ǰ�滹��һ����ʡ�Ե���1
	int mantissa = (ival & 0x007fffff) | 0x800000;
	// ��ȡָ��,��23�ֽ�
	int exponent =  (ival >> 23) & 0xff;
	exponent = exponent - 127;//�õ�������ָ��ֵ
	if ( (fracbits+exponent) < 23)
		mantissa = mantissa>>( 23 - (fracbits+exponent));
	else
		mantissa = mantissa<<(  (fracbits+exponent) - 23);
	// ���С��0���򽫽��ȡ��
	if ((*(int *)&fval) & 0x80000000)
		mantissa = -mantissa;
	//int vvv = fval * (1<<fracbits);
	return mantissa;
}
//�Ѷ�����ת���ɸ�����
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
	//���λbit����λ��
	int highest_bit_pos = 31;
	while (!((1<<highest_bit_pos) & i))
		highest_bit_pos--;
	int exponent = highest_bit_pos-fractbits;
	exponent = exponent + 127;
	//ȥ�����λ��1
	int mantissa = (i & (  ~(1<<highest_bit_pos) ));
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

/*
(x0,y0)��������ʱ����תtheta֮��Ϊ(x1,y1)
����:
x1 = x0*cos(��) - y0*sin(��)
y1 = x1*sin(��) + y0*cos(��)



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