#ifndef __COMM_HEADERS_H__
#define __COMM_HEADERS_H__
#include "base_type.h"
#define ARGB_MASK 0xff

#define ARGB(a, r, g, b) ((b&ARGB_MASK) + ((g&ARGB_MASK) << 8) + ((r&ARGB_MASK) << 16) + ((a&ARGB_MASK) << 24))

#define GET_ARGB_A(argb) ( (argb>>24) & ARGB_MASK)
#define GET_ARGB_R(argb) ( (argb>>16) & ARGB_MASK)
#define GET_ARGB_G(argb) ( (argb>>8) & ARGB_MASK)
#define GET_ARGB_B(argb) ( (argb>>0) & ARGB_MASK)

#ifndef MACRO_SWAP
	#define MACRO_SWAP(a,b,type) do{type c = (a);a=(b);b=c;}while(0)

	#define MACRO_MIN2(a,b) ((a)<(b)?(a):(b))
	#define MACRO_MAX2(a,b)  ((a)>(b)?(a):(b))
	#define MACRO_MIN3(a,b,c) ( ( (a)<(b) ) ? ( (a)<(c)?(a):(c)):( ( (b)<(c)?(b):(c))) )
	#define MACRO_MAX3(a,b,c) ( ( (a)>(b) )?( (a)>(c)?(a):(c) ):( (b)>(c)?(b):(c) ) )
#endif

#define FRACBITS 12
#define epsilon 0.000001f
#define equal_zero(f) ( (f) < epsilon && (f)> -epsilon )
//#define PI 3.1415926535f
//#define TWO_PI  6.283185307f
//#define HALF_PI 1.57079632675f

const static float PI = 3.1415926535f;
const static float TWO_PI = 6.283185307f;
const static float HALF_PI = 1.57079632675f;

//用抛物线近似sin 曲线所用到的常量
/**/
const float const_fast_sin_4_d_pi = 4/PI;
const float const_fast_sin_4_d_pi2 = 4/(PI*PI);
const float const_fast_sin_Q =0.775;
const float const_fast_sin_P =0.225;

typedef uint32_t ARGB;
typedef uint64_t ARGB64;
#endif