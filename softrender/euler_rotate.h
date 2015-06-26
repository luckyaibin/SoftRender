#ifndef __EULER_ROTATE_H__
#define __EULER_ROTATE_H__
#include "matrix.h"

//����ŷ���ǻ����ת����Χ��x��y��z��������ת���ԵĽǶ�֮��ľ���
Matrix3 get_matrix_from_x_y_z_axis_angle(float x_axis_angle,float y_axis_angle,float z_axis_angle)
{http://fanyi.baidu.com/###
	Matrix3 x_axis
	(
		1,			0,					0,
		0,	cos(x_axis_angle),	-sin(x_axis_angle),
		0,	sin(x_axis_angle),	cos(x_axis_angle)
	);

	Matrix3 y_axis 
	(
		cos(y_axis_angle),		0,		sin(y_axis_angle),
		0,						1,				0,
		-sin(y_axis_angle),		0,		cos(y_axis_angle)
	);
	
	Matrix3 z_axis 
	(
		cos(z_axis_angle),		0,			sin(z_axis_angle),
		0,						1,				0,
		-sin(z_axis_angle),		0,			cos(z_axis_angle)
	);

	return z_axis*y_axis*z_axis;

}

//Χ�����ⵥλ���ȵ�����v��תһ���Ƕ�,a,b,c������v��x��y��z�ϵķ���
Matrix3 get_matrix_from_any_axis_angle(float a,float b,float c,float angle)
{
	float K = 1 - cos(angle);
	float sin_angle = sin(angle);
	float cos_angle = cos(angle);
	Matrix3 m 
	(
		a*a*K + cos_angle,	a*b*K - c*sin_angle,	a*c*K + b*sin_angle,
		a*b*K + c*sin_angle,b*b*K + cos_angle,		b*c*K - a*sin_angle,
		a*c*K - b*sin_angle,b*c*K + a*sin_angle,	c*c*K + cos_angle
	);
	return m;
}

#endif