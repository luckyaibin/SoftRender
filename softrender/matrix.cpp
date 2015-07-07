#include "matrix.h"
#include "comm_headers.h"
#include <stdio.h>

Matrix3::Matrix3(const Matrix3 &m1)
{
	m00=m1.m00,m01=m1.m01,m02=m1.m02;
	m10=m1.m10,m11=m1.m11,m12=m1.m12;
	m20=m1.m20,m21=m1.m21,m22=m1.m22;
}
Matrix3& Matrix3::operator=(const Matrix3 &m1)
{
	if(this == &m1)
		return *this;

	m00=m1.m00,m01=m1.m01,m02=m1.m02;
	m10=m1.m10,m11=m1.m11,m12=m1.m12;
	m20=m1.m20,m21=m1.m21,m22=m1.m22;
	return *this;
}
Matrix3 operator*(const Matrix3& m1,const Matrix3& m2)
{
	Matrix3 r;
	r.m00 = m1.m00 * m2.m00 + m1.m01 * m2.m10 + m1.m02 * m2.m20;
	r.m01 = m1.m00 * m2.m01 + m1.m01 * m2.m11 + m1.m02 * m2.m21;
	r.m02 = m1.m00 * m2.m02 + m1.m01 * m2.m12 + m1.m02 * m2.m22;

	r.m10 = m1.m10 * m2.m00 + m1.m11 * m2.m10 + m1.m12 * m2.m20;
	r.m11 = m1.m10 * m2.m01 + m1.m11 * m2.m11 + m1.m12 * m2.m21;
	r.m12 = m1.m10 * m2.m02 + m1.m11 * m2.m12 + m1.m12 * m2.m22;

	r.m20 = m1.m20 * m2.m00 + m1.m21 * m2.m10 + m1.m22 * m2.m20;
	r.m21 = m1.m20 * m2.m01 + m1.m21 * m2.m11 + m1.m22 * m2.m21;
	r.m22 = m1.m20 * m2.m02 + m1.m21 * m2.m12 + m1.m22 * m2.m22;

	return r;
}
Matrix3 operator*(const Matrix3& m1,float f)
{	
	Matrix3 m;
	m.m00 = m1.m00 * f;
	m.m01 = m1.m01 * f;
	m.m02 = m1.m02 * f;

	m.m10 = m1.m10 * f;
	m.m11 = m1.m11 * f;
	m.m12 = m1.m12 * f;

	m.m20 = m1.m20 * f;
	m.m21 = m1.m21 * f;
	m.m22 = m1.m22 * f;
	return m;
}
Matrix3 operator*(float f,const Matrix3 &m1)
{
	Matrix3 m;
	m.m00 = m1.m00 * f;
	m.m01 = m1.m01 * f;
	m.m02 = m1.m02 * f;

	m.m10 = m1.m10 * f;
	m.m11 = m1.m11 * f;
	m.m12 = m1.m12 * f;

	m.m20 = m1.m20 * f;
	m.m21 = m1.m21 * f;
	m.m22 = m1.m22 * f;
	return m;
}

//vector3 operator*(Matrix3 m,vector3 v)
//{
//	float x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
//	float y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
//	float z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;
//	return vector3(x,y,z);
//}

float matrix3_det(Matrix3 m)
{
	//克莱姆法则
	//float d = m.m00 * m.m11 *m.m22 + m.m01*m.m12*m.m20 + m.m02*m.m10*m.m21 - m.m02*m.m11*m.m20 - m.m01*m.m10*m.m22 - m.m00*m.m12*m.m21
	//代数余子式
	float d = m.m00*(m.m11*m.m22 - m.m21*m.m12) - m.m01*(m.m10*m.m22 - m.m20*m.m12) - m.m02*(m.m10*m.m21 - m.m20*m.m11);
	return d;
}

int matrix3_inverse(Matrix3 m,Matrix3_Ptr inverse_res)
{
	float det = matrix3_det(m);
	if (det<epsilon)
	{
		return 0;
	}
	float det_inv = 1.0/det;
	 
	inverse_res->m00 =  det_inv*(m.m11*m.m22 - m.m21*m.m12);
	inverse_res->m10 = -det_inv*(m.m10*m.m22 - m.m20*m.m12);
	inverse_res->m20 =  det_inv*(m.m10*m.m21 - m.m20*m.m11);

	inverse_res->m01 = -det_inv*(m.m01*m.m22 - m.m21*m.m02);
	inverse_res->m11 =  det_inv*(m.m00*m.m22 - m.m20*m.m02);
	inverse_res->m21 = -det_inv*(m.m00*m.m21 - m.m20*m.m01);

	inverse_res->m02 =  det_inv*(m.m01*m.m12 - m.m11*m.m02);
	inverse_res->m12 = -det_inv*(m.m00*m.m12 - m.m10*m.m02);
	inverse_res->m22 =  det_inv*(m.m00*m.m11 - m.m10*m.m01);
	return 1;
}

const float& get_element_const_ref(const Matrix3& m,int row,int col)
{
	if (row==0 && col==0)
		return m.m00;
	if (row==0 && col==1)
		return m.m01;
	if (row==0 && col==2)
		return m.m02;
	if (row==1 && col==0)
		return m.m10;
	if (row==1 && col==1)
		return m.m11;
	if (row==1 && col==2)
		return m.m12;
	if (row==2 && col==0)
		return m.m20;
	if (row==2 && col==1)
		return m.m21;
	if (row==2 && col==2)
		return m.m22;
}

float& get_element_ref(Matrix3& m,int row,int col)
{
	if (row==0 && col==0)
		return m.m00;
	if (row==0 && col==1)
		return m.m01;
	if (row==0 && col==2)
		return m.m02;
	if (row==1 && col==0)
		return m.m10;
	if (row==1 && col==1)
		return m.m11;
	if (row==1 && col==2)
		return m.m12;
	if (row==2 && col==0)
		return m.m20;
	if (row==2 && col==1)
		return m.m21;
	if (row==2 && col==2)
		return m.m22;
}

//////////////////////////////////////////////////////////////////////////

Matrix4::Matrix4(const Matrix4 &m1)
{
	m00=m1.m00,m01=m1.m01,m02=m1.m02,m03=m1.m03;
	m10=m1.m10,m11=m1.m11,m12=m1.m12,m13=m1.m13;
	m20=m1.m20,m21=m1.m21,m22=m1.m22,m23=m1.m23;
	m30=m1.m30,m31=m1.m31,m32=m1.m32,m33=m1.m33;
}
Matrix4& Matrix4::operator=(const Matrix4 &m1)
{
	if(this == &m1)
		return *this;

	m00=m1.m00,m01=m1.m01,m02=m1.m02,m03=m1.m03;
	m10=m1.m10,m11=m1.m11,m12=m1.m12,m13=m1.m13;
	m20=m1.m20,m21=m1.m21,m22=m1.m22,m23=m1.m23;
	m30=m1.m30,m31=m1.m31,m32=m1.m32,m33=m1.m33;
	return *this;
}
Matrix4 operator*(const Matrix4& m1,const Matrix4& m2)
{
	Matrix4 r;
	r.m00 = m1.m00 * m2.m00 + m1.m01 * m2.m10 + m1.m02 * m2.m20 + m1.m03 * m2.m30;
	r.m01 = m1.m00 * m2.m01 + m1.m01 * m2.m11 + m1.m02 * m2.m21 + m1.m03 * m2.m31;
	r.m02 = m1.m00 * m2.m02 + m1.m01 * m2.m12 + m1.m02 * m2.m22 + m1.m03 * m2.m32;
	r.m03 = m1.m00 * m2.m03 + m1.m01 * m2.m13 + m1.m02 * m2.m23 + m1.m03 * m2.m33;

	r.m10 = m1.m10 * m2.m00 + m1.m11 * m2.m10 + m1.m12 * m2.m20 + m1.m13 * m2.m30;
	r.m11 = m1.m10 * m2.m01 + m1.m11 * m2.m11 + m1.m12 * m2.m21 + m1.m13 * m2.m31;
	r.m12 = m1.m10 * m2.m02 + m1.m11 * m2.m12 + m1.m12 * m2.m22 + m1.m13 * m2.m32;
	r.m13 = m1.m10 * m2.m03 + m1.m11 * m2.m13 + m1.m12 * m2.m23 + m1.m13 * m2.m33;

	r.m20 = m1.m20 * m2.m00 + m1.m21 * m2.m10 + m1.m22 * m2.m20 + m1.m23 * m2.m30;
	r.m21 = m1.m20 * m2.m01 + m1.m21 * m2.m11 + m1.m22 * m2.m21 + m1.m23 * m2.m31;
	r.m22 = m1.m20 * m2.m02 + m1.m21 * m2.m12 + m1.m22 * m2.m22 + m1.m23 * m2.m32;
	r.m23 = m1.m20 * m2.m03 + m1.m21 * m2.m13 + m1.m22 * m2.m23 + m1.m23 * m2.m33;

	r.m30 = m1.m30 * m2.m00 + m1.m31 * m2.m10 + m1.m32 * m2.m20 + m1.m33 * m2.m30;
	r.m31 = m1.m30 * m2.m01 + m1.m31 * m2.m11 + m1.m32 * m2.m21 + m1.m33 * m2.m31;
	r.m32 = m1.m30 * m2.m02 + m1.m31 * m2.m12 + m1.m32 * m2.m22 + m1.m33 * m2.m32;
	r.m33 = m1.m30 * m2.m03 + m1.m31 * m2.m13 + m1.m32 * m2.m23 + m1.m33 * m2.m33;

	return r;
}
Matrix4 operator*(const Matrix4& m1,float f)
{	
	Matrix4 m;
	m.m00 = m1.m00 * f;
	m.m01 = m1.m01 * f;
	m.m02 = m1.m02 * f;
	m.m03 = m1.m03 * f;

	m.m10 = m1.m10 * f;
	m.m11 = m1.m11 * f;
	m.m12 = m1.m12 * f;
	m.m13 = m1.m13 * f;

	m.m20 = m1.m20 * f;
	m.m21 = m1.m21 * f;
	m.m22 = m1.m22 * f;
	m.m23 = m1.m23 * f;

	m.m30 = m1.m30 * f;
	m.m31 = m1.m31 * f;
	m.m32 = m1.m32 * f;
	m.m33 = m1.m33 * f;

	return m;
}
Matrix4 operator*(float f,const Matrix4 &m1)
{
	return m1*f;
}
//vector3 operator * (Matrix4 m,vector3 v)
//{
//	float x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03;
//	float y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13;
//	float z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23;
//	return vector3(x,y,z);
//}
float matrix4_det(Matrix4)
{
	return 0;
}

const float& get_element_const_ref(const Matrix4& m,int row,int col)
{
	if (row==0 && col==0)
		return m.m00;
	if (row==0 && col==1)
		return m.m01;
	if (row==0 && col==2)
		return m.m02;
	if (row==0 && col==3)
		return m.m03;
	if (row==1 && col==0)
		return m.m10;
	if (row==1 && col==1)
		return m.m11;
	if (row==1 && col==2)
		return m.m12;
	if (row==1 && col==3)
		return m.m13;
	if (row==2 && col==0)
		return m.m20;
	if (row==2 && col==1)
		return m.m21;
	if (row==2 && col==2)
		return m.m22;
	if (row==2 && col==3)
		return m.m23;
	if (row==3 && col==0)
		return m.m30;
	if (row==3 && col==1)
		return m.m31;
	if (row==3 && col==2)
		return m.m32;
	if (row==3 && col==3)
		return m.m33;
}
float& get_element_ref(Matrix4& m,int row,int col)
{
	if (row==0 && col==0)
		return m.m00;
	if (row==0 && col==1)
		return m.m01;
	if (row==0 && col==2)
		return m.m02;
	if (row==0 && col==3)
		return m.m03;
	if (row==1 && col==0)
		return m.m10;
	if (row==1 && col==1)
		return m.m11;
	if (row==1 && col==2)
		return m.m12;
	if (row==1 && col==3)
		return m.m13;
	if (row==2 && col==0)
		return m.m20;
	if (row==2 && col==1)
		return m.m21;
	if (row==2 && col==2)
		return m.m22;
	if (row==2 && col==3)
		return m.m23;
	if (row==3 && col==0)
		return m.m30;
	if (row==3 && col==1)
		return m.m31;
	if (row==3 && col==2)
		return m.m32;
	if (row==3 && col==3)
		return m.m33;
}


void matrix_dump(const Matrix3& m)
{
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<3;j++)
		{
			printf("%f	",get_element_const_ref(m,i,j));
		}
		printf("\n");
	}
}


void matrix_dump(const Matrix4& m)
{
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			printf("%f	",get_element_const_ref(m,i,j));
		}
		printf("\n");
	}
}

int matrix4_inverse(Matrix4 m,Matrix4_Ptr inverse_res)
{
	float det = m.m00*(m.m11*m.m22 - m.m21*m.m12) - m.m01*(m.m10*m.m22 - m.m20*m.m12) - m.m02*(m.m10*m.m21 - m.m20*m.m11);
	if (det<epsilon)
	{
		return 0;
	}
	float det_inv = 1.0/det;

	inverse_res->m00 =  det_inv*(m.m11*m.m22 - m.m21*m.m12);
	inverse_res->m01 = -det_inv*(m.m01*m.m22 - m.m21*m.m02);
	inverse_res->m02 =  det_inv*(m.m01*m.m12 - m.m11*m.m02);
	inverse_res->m03 = 0.0;

	inverse_res->m10 = -det_inv*(m.m10*m.m22 - m.m20*m.m12);
	inverse_res->m11 =  det_inv*(m.m00*m.m22 - m.m20*m.m02);
	inverse_res->m12 = -det_inv*(m.m00*m.m12 - m.m10*m.m02);
	inverse_res->m13 = 0.0;

	inverse_res->m20 =  det_inv*(m.m10*m.m21 - m.m20*m.m11);
	inverse_res->m21 = -det_inv*(m.m00*m.m21 - m.m20*m.m01);
	inverse_res->m22 =  det_inv*(m.m00*m.m11 - m.m10*m.m01);
	inverse_res->m23 = 0.0;

	inverse_res->m30 = -( m.m30 * inverse_res->m00 + m.m31 * inverse_res->m10 + m.m32 * inverse_res->m20 );
	inverse_res->m31 = -( m.m30 * inverse_res->m01 + m.m31 * inverse_res->m11 + m.m32 * inverse_res->m21 );
	inverse_res->m32 = -( m.m30 * inverse_res->m02 + m.m31 * inverse_res->m12 + m.m32 * inverse_res->m22 );
	inverse_res->m33 = 1.0;
	return 1;
}
