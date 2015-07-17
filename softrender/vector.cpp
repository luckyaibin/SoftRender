#pragma  once
#include "vector.h"
#include "matrix.h"
#include "tool.h"
#include <stdio.h>
vector3 operator+(const vector3& v1,const vector3& v2)
{
	return vector3(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
}

vector3 operator-(const vector3& v1,const vector3& v2)
{
	return vector3(v1.x-v2.x,v1.y-+v2.y,v1.z-v2.z);
}

vector3 operator*(const vector3& v,float f)
{
	vector3 r;
	r.x = v.x*f;
	r.y = v.y*f;
	r.z = v.z*f;
	return r;
}
vector3 operator*(float f,const vector3& v)
{
	vector3 r;
	r.x = v.x*f;
	r.y = v.y*f;
	r.z = v.z*f;
	return r;
}

//行向量 v:1*3  x m:3*3
vector3 operator*(const vector3& v,const Matrix3& m)
{
	float x = v.x * m.m00 
		+ v.y * m.m10
		+ v.z * m.m20;

	float y = v.x * m.m01
		+ v.y * m.m11
		+ v.z * m.m21;

	float z = v.x * m.m02
		+ v.y * m.m12
		+ v.z * m.m22;
	return vector3(x,y,z);
}

//m:3*3  x v:3*1 列向量
vector3 operator*(const Matrix3& m,const vector3& v)
{
	float x = m.m00 * v.x
		+ m.m01 * v.y
		+ m.m02 * v.z;
	float y = m.m10 * v.x
		+ m.m11 * v.y
		+ m.m12 * v.z;
	float z = m.m20 * v.x
		+ m.m21 * v.y
		+ m.m22 * v.z;
	return vector3(x,y,z);
}

// m:4*4 x v 3*1 (当成4x1矩阵）
vector3 operator * (Matrix4 m,vector3 v)
{
	//vector_dump(v);
	float x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03;
	float y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13;
	float z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23;

	/*float _1 = m.m00 * v.x;
	float _2 = m.m01 * v.y;
	float _3 = m.m02 * v.z + m.m03;

	float _4 = m.m10 * v.x;
	float _5 = m.m11 * v.y;
	float _6 = m.m12 * v.z + m.m13;

	float _7 = m.m20 * v.x;
	float _8 = m.m21 * v.y;
	float _9 = m.m22 * v.z + m.m23;*/

	return vector3(x,y,z);
}

float operator*(const vector3& v1,const vector3& v2)
{
	return 	v1.x * v2.x + v1.y * v2.y +	v1.z * v2.z ;
}
vector3 cross_mul(const vector3& a,const vector3& b)
{
	vector3 v;
	v.x = a.y * b.z - a.z * b.y;
	v.y = a.z * b.x - a.x * b.z;
	v.z = a.x * b.y - a.y * b.x;
	return v;
}

void normalize(vector3 * v)
{
	float invert_sqrt_length = InvSqrt(v->x *v->x + v->y * v->y + v->z * v->z);
	v->x *= invert_sqrt_length;
	v->y *= invert_sqrt_length;
	v->z *= invert_sqrt_length;
}

void vector_dump(const vector3&v)
{
	printf("\n vector x:%f\ty:%f\tz:%f\n",v.x,v.y,v.z);
}

//////////vector4
vector4 operator+(const vector4& v1,const vector4& v2)
{
	return vector4(v1.w + v2.w,v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
}

vector4 operator-(const vector4& v1,const vector4& v2)
{
	return vector4(v1.w - v2.w,v1.x-v2.x,v1.y-+v2.y,v1.z-v2.z);
}

vector4 operator*(const vector4& v,float f)
{
	vector4 r;
	r.w = v.w*f;
	r.x = v.x*f;
	r.y = v.y*f;
	r.z = v.z*f;
	return r;
}
vector4 operator*(float f,const vector4& v)
{
	vector4 r;
	r.w = v.w*f;
	r.x = v.x*f;
	r.y = v.y*f;
	r.z = v.z*f;
	return r;
}

//行向量 v:1*4  x m:4*4
vector4 operator*(const vector4& v,const Matrix4& m)
{
	float w = v.w * m.m00 
		+ v.x * m.m10
		+ v.y * m.m20
		+ v.z * m.m30;

	float x = v.w * m.m01 
		+ v.x * m.m11
		+ v.y * m.m21
		+ v.z * m.m31;

	float y = v.w * m.m02 
		+ v.x * m.m12
		+ v.y * m.m22
		+ v.z * m.m32;
	float z = v.w * m.m03 
		+ v.x * m.m13
		+ v.y * m.m23
		+ v.z * m.m33;
	return vector4(w,x,y,z);
}

//m:4*4  x v:4*1 列向量
vector4 operator*(const Matrix4& m,const vector4& v)
{
	float w = m.m00 * v.w
		+ m.m01 * v.x
		+ m.m02 * v.y
		+ m.m03 * v.z;

	float x = m.m10 * v.w
		+ m.m11 * v.x
		+ m.m12 * v.y
		+ m.m13 * v.z;

	float y = m.m20 * v.w
		+ m.m21 * v.x
		+ m.m22 * v.y
		+ m.m23 * v.z;

	float z = m.m30 * v.w
		+ m.m31 * v.x
		+ m.m32 * v.y
		+ m.m33 * v.z;
	 
	return vector4(w,x,y,z);
}

float operator*(const vector4& v1,const vector4& v2)
{
	return v1.w*v2.w + v1.x * v2.x + v1.y * v2.y +	v1.z * v2.z ;
}
vector4 cross_mul(const vector4& a,const vector4& b)
{
	vector4 v;
	/*v.x = a.y * b.z - a.z * b.y;
	v.y = a.z * b.x - a.x * b.z;
	v.z = a.x * b.y - a.y * b.x;*/
	return v;
}

void vector_dump(const vector4&v)
{
	printf("%f\t%f\t%f\t%f\n",v.w,v.x,v.y,v.z);
}
