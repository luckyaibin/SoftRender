#ifndef __COMM_HEADERS_H__
#define __COMM_HEADERS_H__
#ifndef MACRO_SWAP
	#define MACRO_SWAP(a,b,type) do{type c = (a);a=(b);b=c;}while(0)

	#define MACRO_MIN2(a,b) ((a)<(b)?(a):(b))
	#define MACRO_MAX2(a,b)  ((a)>(b)?(a):(b))
	#define MACRO_MIN3(a,b,c) ( ( (a)<(b) ) ? ( (a)<(c)?(a):(c)):( ( (b)<(c)?(b):(c))) )
	#define MACRO_MAX3(a,b,c) ( ( (a)>(b) )?( (a)>(c)?(a):(c) ):( (b)>(c)?(b):(c) ) )
#endif

#define epsilon 0.000001f
#define equal_zero(f) ( (f) < epsilon && (f)> -epsilon )
typedef unsigned int ARGB;
typedef unsigned long ARGB64;
#endif