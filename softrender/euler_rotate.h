#ifndef __EULER_ROTATE_H__
#define __EULER_ROTATE_H__
#include "matrix.h"

//����ŷ���ǻ����ת����Χ��x��y��z��������ת���ԵĽǶ�֮��ľ���
//�������� Quaternion For Computer Graphics page74
Matrix3 get_matrix_from_x_y_z_axis_angle(float x_axis_angle,float y_axis_angle,float z_axis_angle)
{ 
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
		cos(z_axis_angle),		-sin(z_axis_angle),			0,
		sin(z_axis_angle),		cos(z_axis_angle),			0,
		0,						0,							1	
		);
	return x_axis*y_axis*z_axis;
}

//Χ����������v��תһ���Ƕ�,a,b,c������v��x��y��z�ϵķ���
Matrix3 get_matrix_from_any_axis_angle(vector3 v,float angle)
{
	float a = v.x;
	float b = v.y;
	float c = v.z;
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

//�ֱ�����x��y��z����
Matrix3 get_matrix_from_scale_x_y_z(float scale_x,float scale_y,float scale_z)
{
	Matrix3 m(scale_x,	0,			0,
			  0,		scale_y,	0,
			  0,		0,			scale_z);
	return m;
}

//������������v�ķ������� k ��

Matrix3 get_matrix_from_vectror(vector3 v,float k)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	Matrix3 m(
			1 + (k-1)*x*x,	(k-1)*x*y,	(k-1)*x*z,
			(k-1)*x*y,		1+(k-1)*y*y,(k-1)*y*z,
			(k-1)*x*z,	(k-1)*y*z,		1+(k-1)*z*z
		);
	return m;
}



#endif