#include <iostream>
#include "matrix.h"
#include "vector.h"

using namespace std;

int32_t main__()
{
	Matrix4 m1;
	Matrix3 m2;
	vector3 vec(1,2,3);
	int32_t v = 1;
	for(int32_t i=0;i<4;i++)
	{
		for (int32_t j=0;j<4;j++)
		{
			get_element_ref(m1,i,j) = v++;
		}
	}
	v=1;

	vector3 res1 = vec*m2;
	vector_dump(res1);

	vector3 res2 = m2*vec;
	vector_dump(res2);
	/*for(int32_t i=0;i<3;i++)
	{
	for (int32_t j=0;j<3;j++)
	{
	m2.arr[i][j] = v++;
	}
	}
	matrix_dump(m2);
	Matrix4 P;
	int32_t l = -1;
	int32_t r = 1;
	int32_t n = 1;
	int32_t f = -1;
	int32_t b = -1;
	int32_t t = 1;
	P.a.a00 = 2/(r-l);
	P.a.a11 = 2/(t-b);
	P.a.a22 = 2/(f-n);

	P.a.a03 = -(r+l)/(r-l);
	P.a.a13 = -(t+b)/(t-b);
	P.a.a23 = -(f+n)/(f-n);
	P.a.a33 = 1;
	matrix_dump(P);

	Matrix3 res = P*m2;

	matrix_dump(res);
	*/
	return 0;
}