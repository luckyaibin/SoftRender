#pragma  once
#ifndef __VECTOR_H__
#define __VECTOR_H__

struct vector3;
struct Matrix3;
struct Matrix4;
vector3 operator+(const vector3& v1,const vector3& v2);
vector3 operator-(const vector3& v1,const vector3& v2);
vector3 operator*(const vector3& v,float f);
vector3 operator*(float f,const vector3& v);
vector3 operator*(const vector3& v,const Matrix3& m);
vector3 operator*(const Matrix3& m,const vector3& v);
float operator*(const vector3& v1,const vector3& v2);
vector3 cross_mul(const vector3& a,const vector3& b);
void vector_dump(const vector3&v);

struct vector3{
	float x,y,z;
	vector3()
	{
		x=0.0f,y=0.0f,z=0.0f;
	}
	vector3(float x,float y,float z)
	{
		this->x =x,this->y=y,this->z=z;
	}
	friend void vector_dump(const vector3&v);
};

#endif