#ifndef __MATRIX_H
#define __MATRIX_H
#include <memory.h>
#include "base_type.h"
struct Matrix3;
struct Matrix4;

Matrix3 operator*(const Matrix3& m1,const Matrix3& m2);
Matrix3 operator*(const Matrix3& m1,float f);
Matrix3 operator*(float f,const Matrix3 &m1);
const float& get_element_const_ref(const Matrix3& m,int32_t row,int32_t col);
float& get_element_ref(Matrix3& m,int32_t row,int32_t col);
void matrix_dump(const Matrix3&);

Matrix4 operator*(const Matrix4& m1,const Matrix4& m2);
Matrix4 operator*(const Matrix4& m1,float f);
Matrix4 operator*(float f,const Matrix4 &m1);

const float& get_element_const_ref(const Matrix4& m,int32_t row,int32_t col);
float& get_element_ref(Matrix4& m,int32_t row,int32_t col);
void matrix_dump(const Matrix4&);

struct Matrix3
{
	union{
		float m[3][3];
		struct
		{
			float m00,m01,m02;
			float m10,m11,m12;
			float m20,m21,m22;
		};
	};
	Matrix3()
	{
		m00=m01=m02=0.0f;
		m10=m11=m12=0.0f;
		m20=m21=m22=0.0f;
	}
	Matrix3(float _1,float _2,float _3,float _4,float _5,float _6,float _7,float _8,float _9)
	{
		m00=_1;
		m01=_2;
		m02=_3;
		m10=_4;
		m11=_5;
		m12=_6;
		m20=_7;
		m21=_8;
		m22=_9;
	}
	Matrix3(const Matrix3 &m1);
	Matrix3& operator=(const Matrix3 &m1);
	friend Matrix3 operator*(const Matrix3& m1,const Matrix3& m2);
	friend Matrix3 operator*(const Matrix3& m1,float f);
	friend Matrix3 operator*(float f,const Matrix3 &m);

	friend void matrix_dump(const Matrix3&);
};

struct Matrix4{
	union{
		float m[4][4];
		struct{
			float m00,m01,m02,m03;
			float m10,m11,m12,m13;
			float m20,m21,m22,m23;
			float m30,m31,m32,m33;
		};
	};
	Matrix4(float _1,float _2,float _3,float _4,float _5,float _6,float _7,float _8,float _9,
		float _10,float _11,float _12,float _13,float _14,float _15,float _16)
	{

		m00 = _1;	m01 = _2;	m02 =_3;	m03 =_4;
		m10 = _5;	m11 = _6;	m12 = _7;	m13 = _8;	
		m20 = _9;	m21 = _10;	m22 = _11;	m23 = _12;	
		m30 = _13;	m31 = _14;	m32 = _15;	m33 = _16;	
		 
	}
	Matrix4()
	{
		m00=m01=m02=m03=0.0f;
		m10=m11=m12=m13=0.0f;
		m20=m21=m22=m23=0.0f;
		m30=m31=m32=m33=0.0f;
	}
	Matrix4(const Matrix4 &m1);
	Matrix4& operator=(const Matrix4 &m1);
	friend Matrix4 operator*(const Matrix4& m1,const Matrix4& m2);
	friend Matrix4 operator*(const Matrix4& m1,float f);
	friend Matrix4 operator*(float f,const Matrix4 &m1);

	friend void matrix_dump(const Matrix4&);
};

#endif