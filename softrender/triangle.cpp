#include "draw.h"
#include "triangle.h"
#include "comm_headers.h"

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
int32_t is_point_in_triangle_by_crossproduct(vertex2d p,vertex2d v1,vertex2d v2, vertex2d v3,int32_t is_clockwise/*=-1*/)
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
int32_t is_point_in_triangle_by_barycentric_algo(vertex2d p,vertex2d v1,vertex2d v2, vertex2d v3,int32_t is_clockwise/*=0*/)
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
	//const static int32_t FRACBITS=12;
	double a = v0.y - v1.y;
	double b = v1.x - v0.x;
	double c = -0.5f * (a * (v0.x + v1.x) + b*(v0.y + v1.y));//其实也就是 x0*y1 - x1*y0
	/*
	利用平面几何能够推导出来三角形的面积 area = [(x0*y1 - x1*y0) + (x1*y2-x2*y1) + (x2*y0 - x0*y2) ]/2
	刚好等于三个edge equation的 c 相加！
	area = c0+c1+c2
	if area==0，是退化的三角形（degenerate triangle),不显示
	if area<0,说明是back facing，如果开启了back-face culling，不显示

	*/
	edge->A = (int32_t)(a* (1<<FRACBITS));
	edge->B = (int32_t)(b* (1<<FRACBITS));
	edge->C = (int32_t)(c* (1<<FRACBITS));
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

int32_t evaluate_edge(struct edge_eqn * edge,int32_t x,int32_t y)
{
	return (edge->A * x + edge->B * y + edge->C);
}

void DrawTriangleWithEdgeEquation(vertex2d v0,vertex2d v1,vertex2d v2,ARGB color)
{
	edge_eqn edge0,edge1,edge2;
	init_edge_eqn(&edge0,v0,v1);
	init_edge_eqn(&edge1,v1,v2);
	init_edge_eqn(&edge2,v2,v0);

	int32_t area = edge0.C + edge1.C + edge2.C;
	int32_t back_face_culling = 0;
	if (area == 0)
	{
		return;
	}
	else if(area < 0)//是反向的，如果不开启backface culling，那么翻转边方程
	{
		if (back_face_culling)
			return;
		flip_edge_eqn(&edge0);
		flip_edge_eqn(&edge1);
		flip_edge_eqn(&edge2);
	}

	int32_t xMin,xMax,yMin,yMax;
	xMin = (int32_t)MACRO_MIN3(v0.x,v1.x,v2.x);
	xMax = (int32_t)MACRO_MAX3(v0.x,v1.x,v2.x);

	yMin = (int32_t)MACRO_MIN3(v0.y,v1.y,v2.y);
	yMax = (int32_t)MACRO_MAX3(v0.y,v1.y,v2.y);

	int32_t A0 = edge0.A;
	int32_t A1 = edge1.A;
	int32_t A2 = edge2.A;

	int32_t B0 = edge0.B;
	int32_t B1 = edge1.B;
	int32_t B2 = edge2.B;

	int32_t C0 = edge0.C;
	int32_t C1 = edge1.C;
	int32_t C2 = edge2.C;

	int32_t t0 = A0*xMin + B0*yMin + C0;
	int32_t t1 = A1*xMin + B1*yMin + C1;
	int32_t t2 = A2*xMin + B2*yMin + C2;

	interpolate_eqn inter;
	init_interpolate_eqn(&inter,v0,v1,v2);
	//printf("x0=%f, y0=%f \n",v0.x,v0.y);
	//printf("x1=%f, y1=%f \n",v1.x,v1.y);
	//printf("x2=%f, y2=%f \n\n",v2.x,v2.y);
	for (int32_t y=yMin;y<=yMax;y++)
	{
		int32_t e0 = t0;
		int32_t e1 = t1;
		int32_t e2 = t2;
		int32_t xflag = 0;
		for (int32_t x = xMin; x <= xMax; x++) {
			if ((e0|e1|e2) >= 0) {      // all 3 edges must be >= 0
				//差值三个颜色分量
				int32_t a = (inter.interpolation_alpha_a *x + inter.interpolation_alpha_b*y + inter.interpolation_alpha_c)*inter.interpolate_argb_divider_reciprocal;
				int32_t r = (inter.interpolation_red_a*x + inter.interpolation_red_b*y + inter.interpolation_red_c)*inter.interpolate_argb_divider_reciprocal;
				int32_t g = (inter.interpolation_greed_a*x + inter.interpolation_greed_b*y + inter.interpolation_greed_c)*inter.interpolate_argb_divider_reciprocal;
				int32_t b = (inter.interpolation_blue_a*x + inter.interpolation_blue_b*y + inter.interpolation_blue_c)*inter.interpolate_argb_divider_reciprocal;
				DrawPixel(x,y,ARGB(a,r,g,b));
				/*int32_t color = interpolate_color_triangle(x,y,v0,v1,v2);
				DrawPixel(x,y,color);*/
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

/*
根据当前光栅化的x，y坐标，使用三角形的颜色进行插值，得到x，y坐标的颜色值
设x，y坐标点的颜色为红色r（其他颜色分量g，b也是一样的），而这个r是和三个顶点相关的线性变化
所以有:
r(x,y) 满足某个线性方程 Ax + By + C,
即：
r(x,y) = Ax + By + C成立，
把三个顶点也满足这个方程，所以坐标和颜色带入进去方程可以得到:
r0 = A*x0 + B*y0 + C
r1 = A*x1 + B*y1 + C
r2 = A*x2 + B*y2 + C

解方程可得:
A = y0*(r2 - r1) + y1*(r0 - r2) + y2*(r1 - r0)
	------------------------------------------
	(x0*y1 - x1*y0) + (x1*y2 - x2*y1) + (x2*y0 - x0*y2)

B = x0*(r2 - r1) + x1*(r0 - r2) + x2*(r1 - r0)
  - ---------------------------------------- 注意前面有个负号！
	(x0*y1 - x1*y0) + (x1*y2 - x2*y1) + (x2*y0 - x0*y2)


C = r0*(x1*y2 - x2*y1) + r1*(x2*y0 - x0*y2) + r2(x0*y1-x1*y0)
	--------------------------------------------------------
	(x0*y1 - x1*y0) + (x1*y2 - x2*y1) + (x2*y0 - x0*y2)


所以方程为:

r(x,y) = Ax + By + C
	  
r(x,y) = 【y0*(r2 - r1) + y1*(r0 - r2) + y2*(r1 - r0)】*x - 【x0*(r2 - r1) + x1*(r0 - r2) + x2*(r1 - r0)】*y + 【r0*(x1*y2 - x2*y1) + r1*(x2*y0 - x0*y2) + r2(x0*y1-x1*y0)】
		  -------------------------------------------------------------------------------------------------------------------------------------------------------------------
		  (x0*y1 - x1*y0) + (x1*y2 - x2*y1) + (x2*y0 - x0*y2);

		上面有推导出来的三角形的面积  area = [(x0*y1 - x1*y0) + (x1*y2-x2*y1) + (x2*y0 - x0*y2) ]/2


更简单的推导
A*x0 + B*y0 + C = z0
A*x1 + B*y1 + C = z1
A*x2 + B*y2 + C = z2

i substract 2 and 3 row from 1

A*(x0-x1) + B*(y0-y1) = z0-z1
A*(x0-x2) + B*(y0-y2) = z0-z2

now

let P=x0-x1 Q=y0-y1
R=x0-x2 S=y0-y2
T=z0-z1 U=z0-z2

A*P+B*Q=T
A*R+B*S=U

det=P*S-R*Q
idet=1/det
A=(T*S-U*Q)*idet
B=(P*U-R*T)*idet
C=z0-A*x0-B*x0
 */

ARGB interpolate_color_triangle(int32_t x,int32_t y,vertex2d v0,vertex2d v1,vertex2d v2)
{
	int32_t x0 = v0.x;
	int32_t y0 = v0.y;
	int32_t x1 = v1.x;
	int32_t y1 = v1.y;
	int32_t x2 = v2.x;
	int32_t y2 = v2.y;
	//三个顶点的alpha分量
	ARGB a0 = GET_ARGB_A(v0.color);
	ARGB a1 = GET_ARGB_A(v1.color);
	ARGB a2 = GET_ARGB_A(v2.color);

	//三个顶点的红色分量
	ARGB r0 = GET_ARGB_R(v0.color);
	ARGB r1 = GET_ARGB_R(v1.color);
	ARGB r2 = GET_ARGB_R(v2.color);

	//三个顶点的绿色分量
	ARGB g0 = GET_ARGB_G(v0.color);
	ARGB g1 = GET_ARGB_G(v1.color);
	ARGB g2 = GET_ARGB_G(v2.color);

	//三个顶点的蓝色分量
	ARGB b0 = GET_ARGB_B(v0.color);
	ARGB b1 = GET_ARGB_B(v1.color);
	ARGB b2 = GET_ARGB_B(v2.color);
	//下面几个临时变量减少几次乘法操作
	int32_t x0_y1 = x0*y1;
	int32_t x1_y2 = x1*y2;
	int32_t x2_y0 = x2*y0;
	
	int32_t x2_y1 = x2*y1;
	int32_t x0_y2 = x0*y2;
	int32_t x1_y0 = x1*y0;

	int32_t a = (	( y0*(a2-a1) + y1*(a0-a2) + y2*(a1-a0) )* x - (x0 * (a2-a1) + x1*(a0 - a2) + x2*(a1-a0))*y 
				+ (a0 * (x1_y2 - x2_y1) + a1*(x2_y0 - x0_y2) + a2*(x0_y1 - x1_y0))
			)/( (x0_y1 - x1_y0) + (x1_y2 - x2_y1) + (x2_y0 - x0_y2) );
	int32_t interpolate_argb_divider = ( (x0_y1 - x1_y0) + (x1_y2 - x2_y1) + (x2_y0 - x0_y2) );

	int32_t interpolate_argb_divider__ = 1;
	int32_t interpolation_alpha_a = ( y0*(a2-a1) + y1*(a0-a2) + y2*(a1-a0) )/interpolate_argb_divider;
	int32_t interpolation_alpha_b = -( x0 * (a2-a1) + x1*(a0 - a2) + x2*(a1-a0))/interpolate_argb_divider;
	int32_t interpolation_alpha_c = (a0 * (x1_y2 - x2_y1) + a1*(x2_y0 - x0_y2) + a2*(x0_y1 - x1_y0))/interpolate_argb_divider;

	int32_t aa = interpolation_alpha_a*x + interpolation_alpha_b*y + interpolation_alpha_c;
	aa = aa/interpolate_argb_divider;
	//alpha 的差值 a=interpolation_alpha_a*x + interpolation_alpha_b*y + interpolation_alpha_c;
	////////////////////////////////////////////////////////////////////////////////////////////
	int32_t r = (	( y0*(r2-r1) + y1*(r0-r2) + y2*(r1-r0) )* x - (x0 * (r2-r1) + x1*(r0 - r2) + x2*(r1-r0))*y 
	+ (r0 * (x1_y2 - x2_y1) + r1*(x2_y0 - x0_y2) + r2*(x0_y1 - x1_y0))
	)/( (x0_y1 - x1_y0) + (x1_y2 - x2_y1) + (x2_y0 - x0_y2) );
	int32_t interpolation_red_a = ( y0*(r2-r1) + y1*(r0-r2) + y2*(r1-r0) )/interpolate_argb_divider;
	int32_t interpolation_red_b = -(x0 * (r2-r1) + x1*(r0 - r2) + x2*(r1-r0))/interpolate_argb_divider;
	int32_t interpolation_red_c = (r0 * (x1_y2 - x2_y1) + r1*(x2_y0 - x0_y2) + r2*(x0_y1 - x1_y0))/interpolate_argb_divider;

	int32_t rr = interpolation_red_a*x + interpolation_red_b*y + interpolation_red_c;
	rr = rr/interpolate_argb_divider;
	//red 的差值 a=interpolation_red_a*x + interpolation_red_b*y + interpolation_red_c;
	int32_t g = (	( y0*(g2-g1) + y1*(g0-g2) + y2*(g1-g0) )* x - (x0 * (g2-g1) + x1*(g0 - g2) + x2*(g1-g0))*y 
	+ (g0 * (x1_y2 - x2_y1) + g1*(x2_y0 - x0_y2) + g2*(x0_y1 - x1_y0))
	)/( (x0_y1 - x1_y0) + (x1_y2 - x2_y1) + (x2_y0 - x0_y2) );
	int32_t interpolation_greed_a = ( y0*(g2-g1) + y1*(g0-g2) + y2*(g1-g0) )/interpolate_argb_divider;
	int32_t interpolation_greed_b = -(x0 * (g2-g1) + x1*(g0 - g2) + x2*(g1-g0))/interpolate_argb_divider;
	int32_t interpolation_greed_c = (g0 * (x1_y2 - x2_y1) + g1*(x2_y0 - x0_y2) + g2*(x0_y1 - x1_y0))/interpolate_argb_divider;
	int32_t gg = interpolation_greed_a*x + interpolation_greed_b*y + interpolation_greed_c;
	gg = gg/interpolate_argb_divider;
	//green 的差值 a=interpolation_greed_a*x + interpolation_green_b*y + interpolation_green_c;

	int32_t b = (	( y0*(b2-b1) + y1*(b0-b2) + y2*(b1-b0) )* x - (x0 * (b2-b1) + x1*(b0 - b2) + x2*(b1-b0))*y 
				+ (b0 * (x1_y2 - x2_y1) + b1*(x2_y0 - x0_y2) + b2*(x0_y1 - x1_y0))
			)/( (x0_y1 - x1_y0) + (x1_y2 - x2_y1) + (x2_y0 - x0_y2) );
	int32_t interpolation_blue_a = ( y0*(b2-b1) + y1*(b0-b2) + y2*(b1-b0) )/interpolate_argb_divider;
	int32_t interpolation_blue_b = -(x0 * (b2-b1) + x1*(b0 - b2) + x2*(b1-b0))/interpolate_argb_divider;
	int32_t interpolation_blue_c = (b0 * (x1_y2 - x2_y1) + b1*(x2_y0 - x0_y2) + b2*(x0_y1 - x1_y0))/interpolate_argb_divider;
	int32_t bb = interpolation_blue_a*x + interpolation_blue_b*y + interpolation_blue_c;
	bb = bb/interpolate_argb_divider;
	//blue 的差值 a=interpolation_blue_a*x + interpolation_blue_b*y + interpolation_blue_c;
	ARGB color = ARGB(a,r,g,b);
	return color;
}

void init_interpolate_eqn( struct interpolate_eqn * inter, vertex2d v0,vertex2d v1,vertex2d v2 )
{
	int32_t x0 = v0.x;
	int32_t y0 = v0.y;
	int32_t x1 = v1.x;
	int32_t y1 = v1.y;
	int32_t x2 = v2.x;
	int32_t y2 = v2.y;
	//三个顶点的alpha分量
	ARGB a0 = GET_ARGB_A(v0.color);
	ARGB a1 = GET_ARGB_A(v1.color);
	ARGB a2 = GET_ARGB_A(v2.color);

	//三个顶点的红色分量
	ARGB r0 = GET_ARGB_R(v0.color);
	ARGB r1 = GET_ARGB_R(v1.color);
	ARGB r2 = GET_ARGB_R(v2.color);

	//三个顶点的绿色分量
	ARGB g0 = GET_ARGB_G(v0.color);
	ARGB g1 = GET_ARGB_G(v1.color);
	ARGB g2 = GET_ARGB_G(v2.color);

	//三个顶点的蓝色分量
	ARGB b0 = GET_ARGB_B(v0.color);
	ARGB b1 = GET_ARGB_B(v1.color);
	ARGB b2 = GET_ARGB_B(v2.color);
	//下面几个临时变量减少几次乘法操作
	int32_t x0_y1 = x0*y1;
	int32_t x1_y2 = x1*y2;
	int32_t x2_y0 = x2*y0;
	
	int32_t x2_y1 = x2*y1;
	int32_t x0_y2 = x0*y2;
	int32_t x1_y0 = x1*y0;

	/*int32_t a = (	( y0*(a2-a1) + y1*(a0-a2) + y2*(a1-a0) )* x - (x0 * (a2-a1) + x1*(a0 - a2) + x2*(a1-a0))*y 
				+ (a0 * (x1_y2 - x2_y1) + a1*(x2_y0 - x0_y2) + a2*(x0_y1 - x1_y0))
			)/( (x0_y1 - x1_y0) + (x1_y2 - x2_y1) + (x2_y0 - x0_y2) );*/
	int32_t interpolate_argb_divider = ( (x0_y1 - x1_y0) + (x1_y2 - x2_y1) + (x2_y0 - x0_y2) );
	inter->interpolate_argb_divider_reciprocal = 1/(float)interpolate_argb_divider;
	int32_t interpolation_alpha_a = ( y0*(a2-a1) + y1*(a0-a2) + y2*(a1-a0) );
	int32_t interpolation_alpha_b = -( x0 * (a2-a1) + x1*(a0 - a2) + x2*(a1-a0));
	int32_t interpolation_alpha_c = (a0 * (x1_y2 - x2_y1) + a1*(x2_y0 - x0_y2) + a2*(x0_y1 - x1_y0));

	//alpha 的差值 a=interpolation_alpha_a*x + interpolation_alpha_b*y + interpolation_alpha_c;

	/*int32_t r = (	( y0*(r2-r1) + y1*(r0-r2) + y2*(r1-r0) )* x - (x0 * (r2-r1) + x1*(r0 - r2) + x2*(r1-r0))*y 
	+ (r0 * (x1_y2 - x2_y1) + r1*(x2_y0 - x0_y2) + r2*(x0_y1 - x1_y0))
	)/( (x0_y1 - x1_y0) + (x1_y2 - x2_y1) + (x2_y0 - x0_y2) );*/
	int32_t interpolation_red_a = ( y0*(r2-r1) + y1*(r0-r2) + y2*(r1-r0) );
	int32_t interpolation_red_b = -(x0 * (r2-r1) + x1*(r0 - r2) + x2*(r1-r0));
	int32_t interpolation_red_c = (r0 * (x1_y2 - x2_y1) + r1*(x2_y0 - x0_y2) + r2*(x0_y1 - x1_y0));
	//red 的差值 a=interpolation_red_a*x + interpolation_red_b*y + interpolation_red_c;

	/*int32_t g = (	( y0*(g2-g1) + y1*(g0-g2) + y2*(g1-g0) )* x - (x0 * (g2-g1) + x1*(g0 - g2) + x2*(g1-g0))*y 
	+ (g0 * (x1_y2 - x2_y1) + g1*(x2_y0 - x0_y2) + g2*(x0_y1 - x1_y0))
	)/( (x0_y1 - x1_y0) + (x1_y2 - x2_y1) + (x2_y0 - x0_y2) );*/
	int32_t interpolation_greed_a = ( y0*(g2-g1) + y1*(g0-g2) + y2*(g1-g0) );
	int32_t interpolation_greed_b = -(x0 * (g2-g1) + x1*(g0 - g2) + x2*(g1-g0));
	int32_t interpolation_greed_c = (g0 * (x1_y2 - x2_y1) + g1*(x2_y0 - x0_y2) + g2*(x0_y1 - x1_y0));
	//green 的差值 a=interpolation_greed_a*x + interpolation_green_b*y + interpolation_green_c;

	/*int32_t b = (	( y0*(b2-b1) + y1*(b0-b2) + y2*(b1-b0) )* x - (x0 * (b2-b1) + x1*(b0 - b2) + x2*(b1-b0))*y 
				+ (b0 * (x1_y2 - x2_y1) + b1*(x2_y0 - x0_y2) + b2*(x0_y1 - x1_y0))
			)/( (x0_y1 - x1_y0) + (x1_y2 - x2_y1) + (x2_y0 - x0_y2) );*/
	int32_t interpolation_blue_a = ( y0*(b2-b1) + y1*(b0-b2) + y2*(b1-b0) );
	int32_t interpolation_blue_b = -(x0 * (b2-b1) + x1*(b0 - b2) + x2*(b1-b0));
	int32_t interpolation_blue_c = (b0 * (x1_y2 - x2_y1) + b1*(x2_y0 - x0_y2) + b2*(x0_y1 - x1_y0));

	inter->interpolation_alpha_a = interpolation_alpha_a;
	inter->interpolation_alpha_b = interpolation_alpha_b;
	inter->interpolation_alpha_c = interpolation_alpha_c;

	inter->interpolation_red_a = interpolation_red_a;
	inter->interpolation_red_b = interpolation_red_b;
	inter->interpolation_red_c = interpolation_red_c;

	inter->interpolation_greed_a = interpolation_greed_a;
	inter->interpolation_greed_b = interpolation_greed_b;
	inter->interpolation_greed_c = interpolation_greed_c;

	inter->interpolation_blue_a = interpolation_blue_a;
	inter->interpolation_blue_b = interpolation_blue_b;
	inter->interpolation_blue_c = interpolation_blue_c;

}

