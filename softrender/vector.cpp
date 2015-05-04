#pragma  once
#include "vector.h"
#include "matrix.h"

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

void vector_dump(const vector3&v)
{
	printf("%f\t%f\t%f\n",v.x,v.y,v.z);
}
