#ifndef __PLANE_H__
#define __PLANE_H__
#include "vector.h"
#include "tool.h"
//һ���������ȷ��һ��ƽ��
struct plane3d
{
	vector3 p;
	vector3 n;
};


float normalize(plane3d * plane)
{
	float invert_sqrt_length = InvSqrt(plane->n.x * plane->n.x + plane->n.y * plane->n.y + plane->n.z * plane->n.z);
	plane->n.x *= invert_sqrt_length;
	plane->n.y *= invert_sqrt_length;
	plane->n.z *= invert_sqrt_length;
}

#endif