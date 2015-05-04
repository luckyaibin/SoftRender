#include "triangle.h"


float cross_product(vertex2d v1,vertex2d v2)
{
	return v1.x*v2.y - v1.y * v2.x;
}
/*
		v1
	   /  \
	 /     \	p(x,y)
	v2-----v3
*/
int is_point_in_triangle_by_crossproduct(vertex2d p,vertex2d v1,vertex2d v2, vertex2d v3,int is_clockwise/*=-1*/)
{
	//向量：v1->v2,也用vertex2d存储
	vertex2d V1_2,V2_3,V3_1;
	vertex2d V1_p,V2_p,V3_p;


	V1_2.x = v2.x - v1.x;
	V1_2.y = v2.y - v1.y;
	V1_p.x = p.x - v1.x;
	V1_p.y = p.y - v1.y;

	V2_3.x = v3.x - v2.x;
	V2_3.y = v3.y - v2.y;
	V2_p.x = p.x - v2.x;
	V2_p.y = p.y - v2.y;

	V3_1.x = v1.x - v3.x;
	V3_1.y = v1.y - v3.y;
	V3_p.x = p.x - v3.x;
	V3_p.y = p.y - v3.y;

	float res1,res2,res3;
	res1 = cross_product(V1_2,V1_p);
	res2 = cross_product(V2_3,V2_p);
	res3 = cross_product(V3_1,V3_p);

	//三个顶点数顺时针给出的，res1 ，res2，res3是小于0
	if (is_clockwise==1)
		return (res1 < 0 && res2 < 0 && res3 < 0);
	else if (is_clockwise == -1)
		return (res1 > 0 && res2 > 0 && res3 > 0);
	else
		return (res1 < 0 && res2 < 0 && res3 < 0) || (res1 > 0 && res2 > 0 && res3 > 0);
}


/*
		v3
	   /  \
	 /     \	p(x,y)
	v1-----v2
*/
int is_point_in_triangle_by_barycentric_algo(vertex2d p,vertex2d v1,vertex2d v2, vertex2d v3,int is_clockwise/*=0*/)
{
	/*
	(v2-v1)*s + (v3-v1)*t = (p-v1)
	其中: s>=0,t>=0,s+t<=1。

	xp - x1 = (x2-x1)*s + (x3-x1)*t;
	yp - y1 = (y2-y1)*s + (y3-y1)*t;

	|
	V

	t = (xp-x1)*(y2-y1) - (yp-y1)*(x2-x1)
		---------------------------------
		(x3-x1)*(y2-y1) - (y3-y1)*(x2-x1)

	s = (yp-y1)*(x3-x1) - (xp-x1)*(y3-y1) 
		---------------------------------
		(x3-x1)*(y2-y1) - (y3-y1)*(x2-x1)


	*/

	float denominator=(v3.x-v1.x)*(v2.y-v1.y) - (v3.y-v1.y)*(v2.x-v1.x);

	float t = ( (p.x-v1.x)*(v2.y-v1.y) - (p.y-v1.y)*(v2.x-v1.x) )/denominator;
	float s = ( (p.y-v1.y)*(v3.x-v1.x) - (p.x-v1.x)*(v3.y-v1.y) )/denominator ;

	//printf("t:%f,s:%f",t,s);

	return (s>=0)&&(t>=0)&&((s+t) <=1);
}

void init_edge_eqn(struct edge_eqn *edge,vertex2d v0,vertex2d v1)
{
	const static int FRACBITS=12;
	double a = v0.y - v1.y;
	double b = v1.x - v0.x;
	double c = -0.5f * (a * (v0.x + v1.x) + b*(v0.y + v1.y));//其实也就是 x0*y1 - x1*y0
	/*
	能够推导出来三角形的面积 area = [(x0*y1 - x1*y0) + (x1*y2-x2*y1) + (x2*y0 - x0*y2) ]/2
	刚好等于三个edge equation的 c 相加！
	area = c0+c1+c2
	if area==0，是退化的三角形（degenerate triangle),不显示
	if area<0,说明是back facing，如果开启了back-face culling，不显示

	*/
	edge->A = (int)(a* (1<<FRACBITS));
	edge->B = (int)(b* (1<<FRACBITS));
	edge->C = (int)(c* (1<<FRACBITS));
	edge->flag = 0;
	if (edge->A>=0) edge->flag += 8;//y方向上的最大最小值
	if (edge->B>=0) edge->flag += 1;
}

void flip_edge_eqn(struct edge_eqn *edge)
{
	edge->A = -edge->A;
	edge->B = -edge->B;
	edge->C = -edge->C;
}

int evaluate_edge(struct edge_eqn * edge,int x,int y)
{
	return (edge->A * x + edge->B * y + edge->C);
}

void DrawTriangleWithEdgeEquation(vertex2d v0,vertex2d v1,vertex2d v2,ARGB color)
{
	edge_eqn edge0,edge1,edge2;
	init_edge_eqn(&edge0,v0,v1);
	init_edge_eqn(&edge1,v1,v2);
	init_edge_eqn(&edge2,v2,v0);

	int area = edge0.C + edge1.C + edge2.C;
	int back_face_culling = 0;
	if (area == 0)
	{
		return;
	}
	else if(area < 0)//是反向的，如果不开启backface culling，那么翻转边方程
	{
		if (back_face_culling)
			return;
		flip_edge_eqn(&edge0);
		flip_edge_eqn(&edge0);
		flip_edge_eqn(&edge0);
	}

	int xMin,xMax,yMin,yMax;
	xMin = (int)MACRO_MIN3(v0.x,v1.x,v2.x);
	xMax = (int)MACRO_MAX3(v0.x,v1.x,v2.x);

	yMin = (int)MACRO_MIN3(v0.y,v1.y,v2.y);
	yMax = (int)MACRO_MAX3(v0.y,v1.y,v2.y);

	int A0 = edge0.A;
	int A1 = edge1.A;
	int A2 = edge2.A;

	int B0 = edge0.B;
	int B1 = edge1.B;
	int B2 = edge2.B;

	int C0 = edge0.C;
	int C1 = edge1.C;
	int C2 = edge2.C;

	int t0 = A0*xMin + B0*yMin + C0;
	int t1 = A1*xMin + B1*yMin + C1;
	int t2 = A2*xMin + B2*yMin + C2;

	for (int y=yMin;y<=yMax;y++)
	{
		int e0 = t0;
		int e1 = t1;
		int e2 = t2;
		int xflag = 0;
		for (int x = xMin; x <= xMax; x++) {
			if ((e0|e1|e2) >= 0) {      // all 3 edges must be >= 0

				DrawPixel(x,y,color);
				xflag++;
			} else if (xflag != 0) break;
			e0 += A0;
			e1 += A1;
			e2 += A2;
		}
		t0 += B0;
		t1 += B1;
		t2 += B2;
	}
}
