#include <math.h>
#include "matrix.h"
struct complex_number
{
	float r;
	float i;
};


complex_number complex_number_i = {0,1};

complex_number complex_number_div(complex_number c,float d)
{
	c.r = c.r/d;
	c.i = c.i/d;
	return c;
}

complex_number complex_number_mul(complex_number c1,complex_number c2)
{
	complex_number result;
	result.r = c1.r*c2.r - c1.i*c2.i;
	result.i = c1.r*c2.i + c1.i*c2.r;
	return result;
}

void complex_number_dump(complex_number c)
{
	printf("t:%f,i:%f\n",c.r,c.i);
}

/*
https://www.youtube.com/watch?v=uRKZnFAR7yw
FamousMathProbs13a: The rotation problem and Hamilton's discovery of quaternions I 

complex number is 
z = (a,b),一个实数对
计算法则是：
(a,b) + (c,d) = (a+c,b+d)
(a,b)x(c,d) = (ac - bd,ad+bc)


laws:
z+w = w+z
z*w = w*z
(z+w)+u = z + (w+u)
(z*w)*u = z*(w*u)
z(w+u) = z*w + z*u

special complex numbers:
I = (1,0)   I*z = z*I = z
O = (0,0)   O+ z = z+ O = z

前方高能：
A：
用（a,0)乘以一个复数z的作用是把z缩放a倍:
(a,0) * (c,d) = (ac,ad) = a(c,d)
B：
用(0,a)乘以一个复数z的所用是把z缩放a倍，并旋转1/4圈：
(0,a)*(c,d) = (-ad,ac) = a(-d,c)

其实复数部分的定义是：
i = （0，1）
所以i^2 = (0,1)*(0,1) = (-1,0)
so i^2 = -1;

引进（a,b) = a(1,0) + b(0,1) = a + bi

这样的话仅仅需要记住：
(3+5i)(-2+i) = -6 + 3i - 10i + 5i^2
			 = (-6-5) + (3-10)i
			 = -11-7i

(0,a) = ai;

///////////////////////


令 ž = （a,-b)
quadrance(欧式距离？) of z is
Q(z) = z*ž = a^2 + b^2
(a,b)*(a,-b) = (a*a - (b*（-b)),a*(-b)+(b*a)
			 = (a^2 + b^2,0)
			 = a^2 + b^2

/////////////////////
主定理:
Q(zw) = Q(z)Q(w)

proof:
if z = a+bi,w = c+di 
(ac-bd)^2 + (ad+bc)^2 
=a^2*c^2 + b^2*d^2 - 2*abcd + a^2*d^2 + b^2*c^2 + 2*abcd
=(a^2 + b^2)*(c^2 + d^2)
//////////////////////

quadrance 是 长度的合理的近似物
那么角度的合理近似物是什么呢？spead?turn? half-turn?

z = a + bi
旋转u=turn of z 是 b/a


z1 = a1+b1*i
z2 = a2+b2*i
the turn form z1 to z2:

u(z1,z2)
= (a1*b2 - a2*b1) / (a1*a2 + b1*b2)

//
主定理：

u(z,w) = u(z*v,w*v)

proof:
if z = a+bi,w = c+di, v = x+yi

省略。。。





*/

// z = u + i*v;
// order pair of numbers(u,v)


//orderd pair of numbers (t,x,y,z)
/*
(t1,x1,y1,z1) + (t2,x2,y2,z2) = (t1+t2,x1+x2,y1+y2,z1+z2)
(t1,x1,y1,z1) * (t2,x2,y2,z2) =
(t1t2 - x1x2 - y1y2 - z1z2,
 t1x2 + t2x1 + y1z2 - y2z1,
 t1y2 + t2y1 + z1x2 - x2x1,
 t1z2 + t2z1 + x1y2 - x2y1)

 write q = (t,x,y,z) = (t,v),where v = (x,y,z)

 (t1,v1) + (t2,v2) = (t1+t2,v1+v2)
 (t1,v1) * (t2,v2) = (t1t2 - v1.v2,t1v2 + t2v1 + v1xv2)

 /////////////////////////

 laws:
 q1 + q2 = q2 + q1
 (q1+q2)+q3 = q1 + (q2+q2)
			q1xq2 != q2xq1	//communitive 交换律
(q1xq2)xq3 = q1x(q2xq3)		//associativity 结合律
q1(q2+q3) = q1q2 + q1q3
(q1+q2)q3 = q1q3 + q2q3

它是一个非交换代数的例子

I = (1,0,0,0) Ixq = qxI = q
O = (0,0,0,0) O+q = q+O = q

证明这些laws：
我们把quaternions 和 2x2负数矩阵联系起来
q = (t,x,y,z) <-----> Mq = { t + xi		y + zi
							-y + zi		t - xi}
https://www.youtube.com/watch?v=g22jAtg3QAk 21分钟

////////////////////////////////////
特殊的quaternions：
I = (1,0,0,0) <-----> (1 0
					   0 1)

l = (0,1,0,0) <-----> (i 0
					   0 -i)
j = (0,0,1,0) <-----> (0 1
					  -1 0)
k = (0,0,0,1) <-----> (0 i
					   i 0)

l^2 = j^2 = k^2 = -1
lj = k,jk=l,kl=j
jl =-k,kj=-l,lk = -j

q = (t,x,y,z) => ǭ = (t,-x,-y,-z)

Q(q) = q*ǭ = t^2 + x^2 + y^2 + z^2

proof:

(t,v)(t,-v) = (t^2 + v.v , t(-v) + tv + v x (-v))
			= (t^2 + v.v,0)
			= (t^2 + v.v)

////////////////////////////////

Inverses qǭ = ǭq = Q(q),表明

q-1 = ǭ/Q(q)

unit quaternions : Q(q) = 1
in this case q-1 = ǭ

///////////////////////////////
the quaternion inner product
(t1,x1,y1,z1).(t2,x2,y2,z2) = t1t2 + x1x2 + y1y2 + z1z2
then Q(q) = q.q


//////////////////////////////
the quaternion inner product
(t1,x1,y1,z1).(t2,x2,y2,z2) = t1t2 + x1x2 + y1y2 + z1z2

所以
Q(q) = q.q

*/


struct quaternion_vector
{
	float x,y,z;
};

quaternion_vector quaternion_vector_add(quaternion_vector v1,quaternion_vector v2)
{
	quaternion_vector q;
	q.x = v1.x + v2.x;
	q.y = v1.y + v2.y;
	q.z = v1.z + v2.z;
	return q;
}

quaternion_vector quaternion_vector_sub(quaternion_vector v1,quaternion_vector v2)
{
	quaternion_vector q;
	q.x = v1.x - v2.x;
	q.y = v1.y - v2.y;
	q.z = v1.z - v2.z;
	return q;
}

quaternion_vector quaternion_vector_mul(quaternion_vector v1,float num)
{
	quaternion_vector q;
	q.x = v1.x * num;
	q.y = v1.y * num;
	q.z = v1.z * num;
	return q;
}

float quaternion_vector_dot_mul(quaternion_vector v1,quaternion_vector v2)
{
	return v1.x*v2.x + v1.y * v2.y + v1.z * v2.z;
}

quaternion_vector quaternion_vector_cross_mul(quaternion_vector q1,quaternion_vector q2)
{
	quaternion_vector v;
	v.x = q1.y * q2.z - q1.z * q2.y;
	v.y = q1.z * q2.x - q1.x * q2.z;
	v.z = q1.x * q2.y - q1.y * q2.x;
	return v;
}

struct quaternion
{
	float t;
	union{
		struct
		{ 
			float x,y,z;
		};
		quaternion_vector vec_x_y_z;
	};
	quaternion():t(0),x(0),y(0),z(0){};
	quaternion(float t,float x,float y,float z){
		this->t = t;
		this->x = x;
		this->y = y;
		this->z = z;
	};
	void dump()
	{
		printf("t:%f,x:%f,y:%f,z:%f \n",t,x,y,z);
		printf("%d,%d\n",sizeof(quaternion::vec_x_y_z),sizeof(quaternion));
	}
	//    i j k 
	//i^2 = j^2 = k^2 = i*j*k = -1
};

quaternion quaternion_conjugate(quaternion q1)
{
	quaternion q;
	q.t = q1.t;
	q.vec_x_y_z.x = -q1.vec_x_y_z.x;
	q.vec_x_y_z.y = -q1.vec_x_y_z.y;
	q.vec_x_y_z.z = -q1.vec_x_y_z.z;
	return q;
}

float quaternion_get_norm(quaternion q1)
{
	q1.dump();

	float norm_v = q1.t * q1.t + q1.x * q1.x + q1.y * q1.y + q1.z * q1.z;
	norm_v = norm_v * InvSqrt(norm_v);
	printf("222normal is %f \n",norm_v);
	return norm_v;
}

quaternion quaternion_normalize(quaternion q1)
{
	float norm = q1.t * q1.t + q1.x * q1.x + q1.y * q1.y + q1.z * q1.z;
	float norm_inverse = 1/sqrt(norm);
	q1.t *= norm_inverse;
	q1.x *= norm_inverse;
	q1.y *= norm_inverse;
	q1.z *= norm_inverse;
	return q1;
}

quaternion quaternion_inverse(quaternion q1)
{
	// q-1 = q* / norm(q)
	//because q*q-1 = q * q* / norm(q) = (t^2 + x^2 + y^2 + z^2)/norm(q) = 1;
	//q*q-1 = 1
	//q-1 = 1/q = cong(q)/(q * conj(q) ) = conj(q)/ norm(q)
	//对于单位向量q，q-1 = conj(q)
	float norm = quaternion_get_norm(q1);
	quaternion q = quaternion_conjugate(q1);
	q.t = q.t/norm;
	q.vec_x_y_z.x = q.vec_x_y_z.x/norm;
	q.vec_x_y_z.y = q.vec_x_y_z.y/norm;
	q.vec_x_y_z.z = q.vec_x_y_z.z/norm;
	return q;
}

quaternion quaternion_add(quaternion q1,quaternion q2)
{
	quaternion q;
	q.t = q1.t + q2.t;
	q.vec_x_y_z = quaternion_vector_add(q1.vec_x_y_z,q2.vec_x_y_z);
	return q;
}

quaternion quaternion_sub(quaternion q1,quaternion q2)
{
	quaternion q;
	q.t = q1.t - q2.t;
	q.vec_x_y_z = quaternion_vector_sub(q1.vec_x_y_z,q2.vec_x_y_z);
	return q;
}

//quaternion 乘以实数scalar
quaternion quaternion_mul_scalar(quaternion q1,float scalar)
{
	q1.t *= scalar;
	q1.x *= scalar;
	q1.y *= scalar;
	q1.z *= scalar;
	return q1;
}
//quaternion的普通乘法 q1*q2
quaternion quaternion_mul(quaternion q1,quaternion q2)
{
	//q = t + v形式的乘法
	/*quaternion q;
	q.t = (q1->t * q2->t) - (quaternion_vector_dot_mul(&q1->vec_x_y_z,&q2->vec_x_y_z)) ;

	quaternion_vector t1_mul_v2 = quaternion_vector_mul(&q2->vec_x_y_z,q1->t);
	quaternion_vector t2_mul_v1 = quaternion_vector_mul(&q1->vec_x_y_z,q2->t);
	quaternion_vector v1_corss_v2 = quaternion_vector_cross_mul(&q1->vec_x_y_z,&q2->vec_x_y_z);
	q.vec_x_y_z = quaternion_vector_add(&t1_mul_v2,&t2_mul_v1);
	q.vec_x_y_z = quaternion_vector_add(&q.vec_x_y_z,&v1_corss_v2);
	return q;*/
	
	//q = (t,x,y,z)形式的乘法
	quaternion q;
	q.t = q1.t*q2.t - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	q.x = q1.t*q2.x + q1.x*q2.t + q1.y*q2.z - q1.z * q2.y;
	q.y = q1.t*q2.y - q1.x*q2.z + q1.y*q2.t + q1.z*q2.x;
	q.z = q1.t*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.t;
	return q;
}

//quaternion的dot product（点乘,把quaternion当成4D vector），其实等于quaternion_mul(q1,quaternion_conjugate(q2))
float quaternion_dot_mul(quaternion q1,quaternion q2)
{
	float dot = q1.t * q2.t 
			+	q1.x * q2.x
			+	q1.y * q2.y
			+	q1.z * q2.z;
	return dot;
}

//把quaternion转换成矩阵
Matrix3 quaternion_to_matrix3(quaternion q)
{
	Matrix3 m;
	m.m00 = q.t + q.x * q.x - q.y * q.y - q.z * q.z;
	m.m01 = 2*q.x*q.y - 2*q.t * q.z;
	m.m02 = 2*q.x*q.z + 2*q.t*q.y;

	m.m10 = 2*q.x*q.y + 2 * q.t*q.z;
	m.m11 = q.t*q.t - q.x*q.x + q.y*q.y - q.z*q.z;
	m.m12 = 2*q.y*q.z - 2*q.t*q.x;

	m.m20 = 2*q.x*q.z - 2*q.t* q.y ;
	m.m21 = 2*q.y*q.z + 2*q.t*q.x;
	m.m22 = q.t*q.t - q.x*q.x - q.y*q.y + q.z*q.z;
	
	return m;
}
 
//把quaternion转换成4x4的矩阵m44，用来和另外一个quaternion转换成的4x1的另外向量m41相乘,得到一个4x1向量
// q * p -> matrix_q * p
//把q左乘一个p转换成matrix_q*p
Matrix4 __quaternion_to_matrix4_left(quaternion q)
{
	Matrix4 m;
	m.m00 = q.t;	m.m01 = - q.x;		m.m02 = -q.y;		m.m03 = -q.z;
	m.m10 = q.x;	m.m11 = q.t;		m.m12 = -q.z;		m.m13 = q.y;
	m.m20 = q.y;	m.m21 = q.z;		m.m22 = q.t;		m.m23 = -q.x;
	m.m30 = q.z;	m.m31 = -q.y;		m.m32 = q.x;		m.m33 = q.t;
	return m;
}

//把quaternion转换成4x4的矩阵m44，用来和另外一个quaternion转换成的4x1的另外向量m41相乘,得到一个4x1向量
//p * q - > marix_q * p
//把p坐成一个q转换成matrix_q*p，用来对q*p*q-1 的右边进行处理
Matrix4 __quaternion_to_matrix4_right(quaternion q)
{
	Matrix4 m;
	m.m00 = q.t;	m.m01 = - q.x;		m.m02 = -q.y;		m.m03 = -q.z;
	m.m10 = q.x;	m.m11 = q.t;		m.m12 = q.z;		m.m13 = -q.y;
	m.m20 = q.y;	m.m21 = -q.z;		m.m22 = q.t;		m.m23 = q.x;
	m.m30 = q.z;	m.m31 = q.y;		m.m32 = -q.x;		m.m33 = q.t;
	return m;
}

quaternion quaternion_mul_q_p_q_reverse_matrix_form(quaternion q,quaternion p)
{
	quaternion q_inverse = quaternion_inverse(q);
	Matrix4 m_left = __quaternion_to_matrix4_left(q);
	Matrix4 m_right = __quaternion_to_matrix4_right(q_inverse);
	Matrix4 m = m_right*m_left;

	vector4 v4;
	v4.w = p.t;
	v4.x = p.x;
	v4.y = p.y;
	v4.z = p.z;

	vector4 res = m*v4;

	quaternion res_q;
	res_q.t = res.w;
	res_q.x = res.x;
	res_q.y = res.y;
	res_q.z = res.z;
	return res_q;
}

//对于是unit-norm quaternion的q，先求出4x4矩阵，再计算qpq-1
//unit-norm quaternion的4x4矩阵有一定规律
quaternion quaternion_mul_unit_norm_q_p_q_reverse_matrix_form(quaternion q,quaternion p)
{
	float s = q.t;
	float x = q.x;
	float y = q.y;
	float z = q.z;
	Matrix4 m(
		1,			0,				0,				0,
		0,	1 - 2*(y*y + z*z),	2*(x*y - s*z),	2*(x*z + s*y),
		0,	2*(x*y + s*z),	1-2*(x*x + z*z),	2*(y*z - s*x),
		0,	2*(x*y - s*y),	2*(y*z + s*z),		1-2*(x*x + y*y)
		);

	vector4 v4;
	v4.w = p.t;
	v4.x = p.x;
	v4.y = p.y;
	v4.z = p.z;

	//用矩阵计算
	vector4 res = m*v4;
	//赋值给quaternion
	quaternion res_q;
	res_q.t = res.w;
	res_q.x = res.x;
	res_q.y = res.y;
	res_q.z = res.z;
	return res_q;
}

//对于unint-norm quaterion q ，把qpq-1里的q 和 q-1转换成两个矩阵相乘的形式3X3矩阵m，用m来和unit quaternion p=0+ li+mj+nk的（l,m,n)向量相乘即可
Matrix3 __quaternion_mul_q_p_q_reverse_matrix3(quaternion q)
{
	/*q = x + xi + yj + zk形式
	matrix = 
	2(s^2 + x^2)-1,		2(xy - sz),		2(xz + sy)
	2(xy + sz),			2(x^2 + y^2)-1,	2(yz - sy)
	2(xz - sy),			2(yz + sx),		2(s^2 + z^2) - 1
	*/
	float s = q.t;
	float x = q.x;
	float y = q.y;
	float z = q.z;
	Matrix3 m(
		2*(s*s + x*x)-1,		2*(x*y - s*z),		2*(x*z + s*y),
		2*(x*y + s*z),			2*(x*x + y*y)-1,	2*(y*z - s*y),
		2*(x*z - s*y),			2*(y*z + s*x),		2*(s*s + z*z) - 1
	);
	return m;
}



//把p用qpq-1的形式旋转,q为unit quaternion
quaternion quaternion_mul_q_p_q_reverse_vector_form(quaternion q,quaternion p)
{
	Matrix3 m_for_p;
	m_for_p.m00 = p.x;
	m_for_p.m10 = p.y;
	m_for_p.m20 = p.z;
	Matrix3 m_mul = __quaternion_mul_q_p_q_reverse_matrix3(q);
	Matrix3 res = m_mul*m_for_p;
	quaternion res_q;
	res_q.t = 0;
	res_q.x = res.m00;
	res_q.y = res.m10;
	res_q.z = res.m20;
	return res_q;
}

/*q^t，计算quaternion q 的 t次幂，t为float
根据欧拉公式（euler's identity:

e^(iθ) = cos(θ) + sin(θ)*i;

对于任意的quaternion q = a + xi+yj+zk,都可写成
q = a + tv(v为单位向量）
（因为欧拉公式对于实数部分为0的单位quaternion v，v*v = -1,有着和虚数i同样的性质,推导的话参见euler的级数展开)


所以对于quaternion的euler's identity为:
（v为单位向量；q为unit norm quaternion--长度为1的quaternion）

q = cosθ+ sinθ* v = e^(θv);		(I)

对于实数部分为0的unit quaternion q，有:

e^q = e^(θv) = cosθ + sinθv		(II)

（关于上面这个怎么推导出来的：因为对于实数部分为0的q，肯定可以表示成某一个实数θ乘以单位向量v，θv，根据euler's identity
对quaternion也成立的上面的公式，可以得到e^q = e^(0+q) = e^(0+θv) = e cosθ + sinθ* v）


对于unit form quaternion q:
ln(q)	=	ln(cosθ+sinθv)
		=	ln(e^(θv) )
		=	θv						(III)


q^0	=	e^(	ln(q^0) )
	=	e^(	0*ln(q)	)
	=	e^( 0*θv	) 根据(III)
	=	e^( 0*v		)
	=	cos(0) + sin(0)v  根据(II)
	=	1 + 0v 
	=	1

q^1 = e(ln(q^1) )
	= e(1 * ln(q) )
	= 1* e(ln(q))
	= q;

 

*/

//q的实数部分为0,q is unit quaternion.
quaternion quaternion_unit_quaternion_exp(quaternion q,float t)
{
	/*
	q^t = e^(	ln(	q^t	) )
		= e^(	t*ln(q) )
		= e^(	t*ln(q) )
		= e^(	t*θv	)
		= e^( (t*θ)v	)
		= cos(tθ) + sin(tθ) v;
		正确性？？？
	*/
	quaternion r;
	float rad_cos = acos(q.t);
	float scale = 1/sqrt(q.x*q.x + q.y*q.y+q.z*q.z);

	rad_cos = rad_cos*t;
	r.t = cos(rad_cos);
	r.x = sin(rad_cos)*scale*r.x;
	r.y = sin(rad_cos)*scale*r.y;
	r.z = sin(rad_cos)*scale*r.z;
	return r;
}

/*http://www.3dgep.com/understanding-quaternions/#more-1815 
使用的是里面的 q' = q1(q1-1 q2 )^t的形式来进行quaternion的slerp
*/
quaternion quaternion_slerp1(quaternion q1,quaternion q2,float t)
{
	// diff = q1-1 * q2;
	quaternion diff = quaternion_inverse(q1);
	diff = quaternion_mul(diff,q2);

	quaternion exponent = quaternion_unit_quaternion_exp(diff,t);

	quaternion r;
	r = quaternion_mul(exponent,q1);
	return r;
}


/*对于slerp，有通用公式：
       sin(1-t)θ        sintθ
vt =   --------- * v1 + ------ * v2
		sinθ            sinθ

参数q1，q2必须是unit form quaternion

下面代码的形式经过化简其实就能得到 上面的通用公式，
参见我QQ空间的推导
*/


quaternion quaternion_slerp(quaternion v0,quaternion v1,float t)
{
	//v0 and v1 should be unit length or else
	// something broken will happen.

	 // Compute the cosine of the angle between the two vectors.
	float dot = quaternion_dot_mul(v0,v1);
	quaternion res;
	const float DOT_TRESHOLD = 0.9995;
	if (dot > DOT_TRESHOLD)
	{
		// result = v0 + t*(v1 dot_mul v0);
		// If the inputs are too close for comfort, linearly interpolate
		// and normalize the result.
		res = quaternion_add(v0,quaternion_mul_scalar(quaternion_mul(v0,v1),t) );
		res = quaternion_normalize(res);
		return res;
	}
	// Robustness: Stay within domain of acos()
	if (dot < -1.0f)
		dot = -1.0f;
	if(dot > 1.0f)
		dot = 1.0f;
	// theta_0 = angle between input vectors
	float theta_0 = acos(dot);
	//theta = angle between v0 and result 
	float theta = theta_0*t;
	//quaternion v2 = v1 - v0*dot;
	//v2.normalize();              // { v0, v2 } is now an orthonormal basis

	quaternion v2 = quaternion_sub(v1,quaternion_mul_scalar(v0,dot));
	v2 = quaternion_normalize(v2);

	quaternion res_1 = quaternion_mul_scalar(v0,cos(theta));
	quaternion res_2 = quaternion_mul_scalar(v2,sin(theta));
	res = quaternion_add(res_1,res_2);
	return res;
}

float slerp_linear_correction(float t,float alpha,float k,float attenuation)
{
	double factor = 1 - attenuation * cos(alpha);
	factor *= factor;
	k *= factor;

	float b = 2 * k;
	float c = -3 * k;
	float d = 1 + k;

	double tprime_divided_by_t = t * (b*t + c) + d;
	return tprime_divided_by_t;
}
quaternion quaternion_slerp_linear_approximate(quaternion v0,quaternion v1,float t)
{

	float dot = quaternion_dot_mul(v0,v1);
	quaternion res;
	const float DOT_TRESHOLD = 0.9995;
	if (dot > DOT_TRESHOLD)
	{
		// result = v0 + t*(v1 dot_mul v0);
		// If the inputs are too close for comfort, linearly interpolate
		// and normalize the result.
		res = quaternion_add(v0,quaternion_mul_scalar(quaternion_mul(v0,v1),t) );
		res = quaternion_normalize(res);
		return res;
	}

	 
	return res;
}

//围绕向量（x，y，z）为单位向量，旋转theta角度的quaternion
quaternion get_quaternion_from_unit_vector_theta(float x,float y,float z,float theta)
{
	float half_theta = theta*0.5;
	quaternion q;
	q.t = cos(half_theta);
	q.x = x * sin(half_theta);
	q.y = y * sin(half_theta);
	q.z = y * sin(half_theta);
	return q;
}

quaternion get_quaternion_from_euler_angle(float angle_a,float angle_b,float angle_c)
{
	float half_angle_a = angle_a*0.5;
	float half_angle_b = angle_b*0.5;
	float half_angle_c = angle_c*0.5;
	quaternion qa(cos(half_angle_a),sin(half_angle_a),0,0);
	quaternion qb(cos(half_angle_b),0,sin(half_angle_b),0);
	quaternion qc(cos(half_angle_c),0,0,sin(half_angle_c));
	
	qa = quaternion_mul(qa,qb);
	qa = quaternion_mul(qa,qc);
	return qa;
}
/*
quaternion的幂：
e=1 + 1/1 + 1/1*2 + 1/1*2*3 + 1/1*2*3*4 ...

Euler's Identity:

e(ix) = cos(x) + i*sin(x)

*/


//根据旋转轴 v(x,y,z) 和half-turn的值h ，求quaternion
/*
要求的quaternions为:q = t + v

其中v=(x,y,z)
	h= |v|/t = sqrt(x^2+y^2+z^2)/t
所以t= |v|/h = sqrt(x^2+y^2+z^2)/h
*/
quaternion __nouse__get_quaternion_from_v_halfturn(quaternion_vector qv,float ht)
{
	float t = sqrt( qv.x * qv.x + qv.y*qv.y + qv.z * qv.z) / ht;
	quaternion q;
	q.t = t;
	q.vec_x_y_z = qv;
	return q;
}

//根据旋转轴v(x,y,z) 和 旋转的弧度r，求quaternion !!!!!这里有个问题就是rad不能为pi，这时候tan（pi/2)为正无穷，怎么办？
//需要做的仅仅是把旋转的弧度r转变为half-turn,然后代入get_quaternion_from_vht
/*
cos(r) = (1-h^2)/(1+h^2)
sin(r) = 2h/(1+h^2)

或者直接一点tan(r/2) = h;
推导出:


*/
quaternion __nouse__get_quaternion_from_v_rad(quaternion_vector qv,float rad)
{
	float ht = tan(rad/2);
	float t = sqrt( qv.x * qv.x + qv.y*qv.y + qv.z * qv.z) / ht;
	quaternion q;
	q.t = t;
	q.vec_x_y_z = qv;
	return q;
}