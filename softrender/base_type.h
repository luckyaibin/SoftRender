#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__
typedef signed char				int8_t;
typedef unsigned char			uint8_t;
typedef short int				int16_t;
typedef unsigned int			uint16_t;
typedef int						int32_t;
typedef unsigned int			uint32_t;

# if __WORDSIZE == 64
typedef long int				int64_t;
typedef unsigned long int		uint64_t;
# else
typedef long long int				int64_t;
typedef unsigned long long int		uint64_t;
#endif

struct FLOAT_FORMAT
{
	uint32_t mantissa:23;
	uint32_t exponent:8;
	uint32_t sign:1;
};

union FLOAT_UINT32_UNION
{
	float f;
	uint32_t ui;
	int32_t i;
	FLOAT_FORMAT ffmat;
};

#endif
