#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "vector.h"
#include "comm_headers.h"

struct vertex3d{
	float x,y,z;
	ARGB color;
};
struct triangle3d{
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

//多边形
struct polygon2d 
{
	vertex2d * v_array;
	int	size;
};

//3D点的外积（叉乘、cross product）
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
//叉乘算法判断一个点是不是在三角形内部。is_clockwise:-1,三个顶点是逆时针顺序。1，顺时针。0，不知道
int is_point_in_triangle_by_crossproduct(vertex2d p,vertex2d v1,vertex2d v2, vertex2d v3,int is_clockwise=0);


//质心算法断一个点是不是在三角形内部
int is_point_in_triangle_by_barycentric_algo(vertex2d p,vertex2d v1,vertex2d v2, vertex2d v3,int is_clockwise=0);



struct edge_eqn{
	
	int A,B,C;
	int flag;
};

//差值方程color(x,y) = A*x + B*y + C
struct interpolate_eqn{
	int interpolation_alpha_a;
	int interpolation_alpha_b;
	int interpolation_alpha_c;

	int interpolation_red_a;
	int interpolation_red_b;
	int interpolation_red_c;

	int interpolation_greed_a;
	int interpolation_greed_b;
	int interpolation_greed_c;

	int interpolation_blue_a;
	int interpolation_blue_b;
	int interpolation_blue_c;

	float interpolate_argb_divider_reciprocal;
};
//初始化差值方程
void init_interpolate_eqn(struct interpolate_eqn * inter, vertex2d v0,vertex2d v1,vertex2d v2);
void init_edge_eqn(struct edge_eqn *edge,vertex2d v0,vertex2d v1);

void flip_edge_eqn(struct edge_eqn *edge);
int evaluate_edge(struct edge_eqn * edge,int x,int y);

void DrawTriangleWithEdgeEquation(vertex2d v0,vertex2d v1,vertex2d v2,ARGB color);
ARGB interpolate_color_triangle(int x,int y,vertex2d v0,vertex2d v1,vertex2d v2);
#endif