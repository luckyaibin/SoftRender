#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "vector.h"
#include "vector.h"
#include "comm_headers.h"

//�����ζ���Ķ��壬�����v0����3d����
struct vertex3d{
	float x,y,z;
	ARGB color;
};
//�����εĶ��壬���������������
struct triangle3d_vertex{
	vertex3d a,b,c;
	vector3 n;
};

struct vector2d{
	float x,y;
};
struct vertex2d{
	float x,y;
	ARGB color;
};



struct triangle2d{
	vertex2d a,b,c;
	vector3 n;
};

//�����
struct polygon2d 
{
	vertex2d * v_array;
	int32_t	size;
};

//3D����������ˡ�cross product��
/*
cross_product(v1, v2) = 0   => v1, v2 are parallel
cross_product(v1, v2) > 0   => v2 is clockwise from v1
cross_product(v1, v2) < 0   => v1 is counterclockwise from v2
*/
//http://www.sunshine2k.de/coding/java/PointInTriangle/PointInTriangle.html#polyconvexity
float cross_product(vertex2d v1,vertex2d v2);

/*
		v1
	   /  \
	 /     \	p(x,y)
	v2-----v3
*/
//����㷨�ж�һ�����ǲ������������ڲ���is_clockwise:-1,������������ʱ��˳��1��˳ʱ�롣0����֪��
int32_t is_point_in_triangle_by_crossproduct(vertex2d p,vertex2d v1,vertex2d v2, vertex2d v3,int32_t is_clockwise=0);


//�����㷨��һ�����ǲ������������ڲ�
int32_t is_point_in_triangle_by_barycentric_algo(vertex2d p,vertex2d v1,vertex2d v2, vertex2d v3,int32_t is_clockwise=0);



struct edge_eqn{
	
	int32_t A,B,C;
	int32_t flag;
};

//��ֵ����color(x,y) = A*x + B*y + C
struct interpolate_eqn{
	int32_t interpolation_alpha_a;
	int32_t interpolation_alpha_b;
	int32_t interpolation_alpha_c;

	int32_t interpolation_red_a;
	int32_t interpolation_red_b;
	int32_t interpolation_red_c;

	int32_t interpolation_greed_a;
	int32_t interpolation_greed_b;
	int32_t interpolation_greed_c;

	int32_t interpolation_blue_a;
	int32_t interpolation_blue_b;
	int32_t interpolation_blue_c;

	float interpolate_argb_divider_reciprocal;
};
//��ʼ����ֵ����
void init_interpolate_eqn(struct interpolate_eqn * inter, vertex2d v0,vertex2d v1,vertex2d v2);
void init_edge_eqn(struct edge_eqn *edge,vertex2d v0,vertex2d v1);

void flip_edge_eqn(struct edge_eqn *edge);
int32_t evaluate_edge(struct edge_eqn * edge,int32_t x,int32_t y);

void DrawTriangleWithEdgeEquation(vertex2d v0,vertex2d v1,vertex2d v2,ARGB color);
ARGB interpolate_color_triangle(int32_t x,int32_t y,vertex2d v0,vertex2d v1,vertex2d v2);
#endif