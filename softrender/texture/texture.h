#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "../../thirdparty/include/libpng/png.h"
#include "../draw.h"
#include <assert.h>
#include <malloc.h>

#ifndef MAX_PATH
	#define MAX_PATH 256
#endif

typedef struct palette{
	unsigned int r,g,b,a;
}texture_palette,*texture_palettep;
typedef 
struct SR_Texture_TYPE
{
	char name[MAX_PATH];
	unsigned int w;
	unsigned int h;
	int pixel_bit_size;//�������ص�bit���ݴ�С
	int pixel_byte_size;
	int bit_depth;
	int color_type;
	unsigned char **row_pointers;
	unsigned char * pixel_data;

	//color map color��ʽ����ɫ�壩
	texture_palettep palettep;
	unsigned int num_palette;
}Texture,*Texture_p;



//colormap�� bpp Ϊ8bit ,��ɫ������û��alphaͨ��
inline int get_pixel_at_8bpp_palette(Texture_p tp,unsigned int row,unsigned int col,
	unsigned int *r,unsigned int *g,unsigned int *b,unsigned int *a)
{
	unsigned char * row_pointer = tp->row_pointers[row];
	unsigned char *pixel_start = &row_pointer[col];
	unsigned int palette_index = (unsigned int)(pixel_start[0]);
	if (tp->num_palette)
	{
		*r = tp->palettep[palette_index].r;
		*g = tp->palettep[palette_index].g;
		*b = tp->palettep[palette_index].b;
	}
	 
	*a = 0;
	return 0;
}

//gray�� bpp Ϊ8bit һ����ɫ��ȣ�bpc)��СΪ16bit��û��alphaͨ��
inline int get_pixel_at_8bpp_gray(Texture_p tp,unsigned int row,unsigned int col,
	unsigned int *r,unsigned int *g,unsigned int *b,unsigned int *a)
{
	unsigned char * row_pointer = tp->row_pointers[row];
	unsigned char *pixel_start = &row_pointer[col];
	*r = (unsigned int)(pixel_start[0]);
	*g = (unsigned int)(pixel_start[0]);
	*b = (unsigned int)(pixel_start[0]);
	*a = 0;
	return 0;
}

//gray alpha��bbcλ8bit һ����ɫ��ȣ�bpc)��СΪ8bit����alphaͨ��
inline int get_pixel_at_8bpp_graya(Texture_p tp,unsigned int row,unsigned int col,
	unsigned int *r,unsigned int *g,unsigned int *b,unsigned int *a)
{
	unsigned char * row_pointer = tp->row_pointers[row];
	unsigned char *pixel_start = &row_pointer[col<<1];//col*2
	*r = (unsigned int)(pixel_start[0]);
	*g = (unsigned int)(pixel_start[0]);
	*b = (unsigned int)(pixel_start[0]);
	*a = (unsigned int)(pixel_start[1]);

	return 0;
}

//gray�� bpp Ϊ16 bit һ����ɫ��ȣ�bpc)��СΪ16bit��û��alphaͨ��
inline int get_pixel_at_16bpp_gray(Texture_p tp,unsigned int row,unsigned int col,
	unsigned int *r,unsigned int *g,unsigned int *b,unsigned int *a)
{
	unsigned char * row_pointer = tp->row_pointers[row];
	unsigned char *pixel_start = &row_pointer[col<<1];
	*r = (unsigned int)(pixel_start[0]);
	*g = (unsigned int)(pixel_start[0]);
	*b = (unsigned int)(pixel_start[0]);
	*a = 0;
	return 0;
}

//gray alpha��bbcλ16bit һ����ɫ��ȣ�bpc)��СΪ16bit����alphaͨ��
inline int get_pixel_at_16bpp_graya(Texture_p tp,unsigned int row,unsigned int col,
	unsigned int *r,unsigned int *g,unsigned int *b,unsigned int *a)
{
	unsigned char * row_pointer = tp->row_pointers[row];
	unsigned char *pixel_start = &row_pointer[col<<2];//col*4 gray16bit ,alpha16bit 
	*r = (unsigned int)(pixel_start[0]);
	*g = (unsigned int)(pixel_start[0]);
	*b = (unsigned int)(pixel_start[0]);
	*a = (unsigned int)(pixel_start[2]);

	return 0;
}

//RGB�� bpcλ8bit һ����ɫ��ȣ�bpc)��СΪ16bit��û��alphaͨ��
inline int get_pixel_at_8bpc_rgb(Texture_p tp,unsigned int row,unsigned int col,
	unsigned int *r,unsigned int *g,unsigned int *b,unsigned int *a)
{
	unsigned char * row_pointer = tp->row_pointers[row];
	unsigned char *pixel_start = &row_pointer[col*3];
	*r = (unsigned int)(pixel_start[0]);
	*g = (unsigned int)(pixel_start[1]);
	*b = (unsigned int)(pixel_start[2]);
	*a = 0;
	return 0;
}

//RGBA��bpcλ8bit һ����ɫ��ȣ�bpc)��СΪ8bit����alphaͨ��
inline int get_pixel_at_8bpc_rgba(Texture_p tp,unsigned int row,unsigned int col,
	unsigned int *r,unsigned int *g,unsigned int *b,unsigned int *a)
{
	unsigned char * row_pointer = tp->row_pointers[row];
	unsigned char *pixel_start = &row_pointer[col<<2];//col*4	
	*r = (unsigned int)(pixel_start[0]);
	*g = (unsigned int)(pixel_start[1]);
	*b = (unsigned int)(pixel_start[2]);
	*a = (unsigned int)(pixel_start[3]);

	return 0;
}


//RGB: pbcλ16bit һ����ɫ��ȣ�bpc)��СΪ16bit��û��alphaͨ��
inline int get_pixel_at_16bpc_rgb(Texture_p tp,unsigned int row,unsigned int col,
	unsigned int *r,unsigned int *g,unsigned int *b,unsigned int *a)
{
	unsigned char * row_pointer = tp->row_pointers[row];
	unsigned char *pixel_start = &row_pointer[col*6];//x*6
	*r = (unsigned int)(pixel_start[0]);
	*g = (unsigned int)(pixel_start[2]);
	*b = (unsigned int)(pixel_start[4]);
	*a = 0;
	return 0;
}

//RGBA: pbcλ16bit һ����ɫ��ȣ�bpc)��СΪ16bit����alphaͨ��
inline int get_pixel_at_16bpc_rgba(Texture_p tp,unsigned int row,unsigned int col,
	unsigned int *r,unsigned int *g,unsigned int *b,unsigned int *a)
{
	unsigned char * row_pointer = tp->row_pointers[row];
	unsigned char *pixel_start = &row_pointer[col<<3];//col*8
	/*
	*r = (unsigned int)(pixel_start[0]) << 8 | pixel_start[1];
	*g = (unsigned int)(pixel_start[2]) << 8 | pixel_start[3];
	*b = (unsigned int)(pixel_start[4]) << 8 | pixel_start[5];
	*a = (unsigned int)(pixel_start[6]) << 8 | pixel_start[7];

	*r = *r * (1.0f/256);
	*g = *g * (1.0f/256);
	*b = *b * (1.0f/256);
	*a = *a * (1.0f/256);*/

	/**r = *r >> 8;
	*g = *g >> 8;
	*b = *b >> 8;
	*a = *a >> 8;*/
	
	*r = (unsigned int)(pixel_start[0]);
	*g = (unsigned int)(pixel_start[2]);
	*b = (unsigned int)(pixel_start[4]);
	*a = (unsigned int)(pixel_start[6]);

	return 0;
}
typedef int (*get_pixel_functionp) (Texture_p tp,unsigned int row,unsigned int col,unsigned int *r,unsigned int *g,unsigned int *b,unsigned int *a);

inline get_pixel_functionp get_get_pixel_function(unsigned int bit_depth,unsigned int color_type)
{
	//����ֻ֧��bit_depthΪ8�� ������ɫ���Ҷȡ�bit_depthΪ8��16�Ĳ�ɫ��
	/***************************************************
	bit_depth :	������ɫͼ��1��2��4��8 
				�Ҷ�ͼ��1��2��4��8��16 
				���ɫͼ��8��16
	****************************************************/
	get_pixel_functionp p = 0;
	//from pngvalid.c bit_size() function
	int per_pixel_bit_size;
	switch (color_type)
	{
	default: 
		break;
	case 0:  //PNG_COLOR_TYPE_GRAY	0��ֻ�лҶȡ�
		{
			if (bit_depth==8)
				p=get_pixel_at_8bpp_gray;
			if(bit_depth==16)
				p=get_pixel_at_16bpp_gray;
		}
			break;
	case 2: //PNG_COLOR_MASK_COLOR(PNG_COLOR_TYPE_RGB) 2����GBA ��3���ֽ�
		{
			if (bit_depth==8)
				p=get_pixel_at_8bpc_rgb;
			if(bit_depth==16)
				p=get_pixel_at_16bpc_rgb;
		}
			break;
	case 3: //PNG_COLOR_TYPE_PALETTE 3����ɫ����ɫ
		{
			if (bit_depth==8)
				p=get_pixel_at_8bpp_palette;
			else
				assert("not supported color-map png.");
		}
		break;
	case 4: //PNG_COLOR_TYPE_GRAY_ALPHA 4���лҶ�ֵ��alpha
		{
			if (bit_depth==8)
				p=get_pixel_at_8bpp_graya;
			if(bit_depth==16)
				p=get_pixel_at_16bpp_graya;
		}
		break;
	case 6://PNG_COLOR_TYPE_RGB_ALPHA 6 ����RGBA ��4 ���ֽ�
		{
			if (bit_depth==8)
				p=get_pixel_at_8bpc_rgba;
			if(bit_depth==16)
				p=get_pixel_at_16bpc_rgba;
		}
		break;
	}
	return p;
}
inline int draw_texture(Texture_p t,int x=0,int y=0)
{

	get_pixel_functionp get_pixel_fp = get_get_pixel_function(t->bit_depth,t->color_type);
	int row=0;
	for (row=0;row<t->h;)
	{
		for (int col=0;col<t->w;)
		{

			unsigned int r;
			unsigned int g;
			unsigned int b;
			unsigned int a;
			unsigned char * row_pointer = t->row_pointers[row];

			//get_pixel_at_16bpc_rgb(t,row,col,&r,&g,&b,&a);
			//get_pixel_at_16bpc_rgba(t,row,col,&r,&g,&b,&a);
			//get_pixel_at_8bpc_rgba(t,row,col,&r,&g,&b,&a);
			//get_pixel_at_8bpc_rgb(t,row,col,&r,&g,&b,&a);
			//get_pixel_at_8bpp_graya(t,row,col,&r,&g,&b,&a);

			//get_pixel_at_8bpp_palette(t,row,col,&r,&g,&b,&a);
			/*r = t->row_pointers[row][4*col+0];
			g = t->row_pointers[row][4*col+1];
			b = t->row_pointers[row][4*col+2];
			a = t->row_pointers[row][4*col+3];*/

			/*r = t->row_pointers[row][t->pixel_byte_size*col+0];
			g = t->row_pointers[row][t->pixel_byte_size*col+1];
			b = t->row_pointers[row][t->pixel_byte_size*col+2];
			a = t->row_pointers[row][t->pixel_byte_size*col+3];*/
			get_pixel_fp(t,row,col,&r,&g,&b,&a);
			DrawPixel(x+col,y+row, ARGB(a,r,g,b));
			col += 1;
		}
		row++;
	}
	return 0;
}

//֧��һ�¸�ʽ��pngͼƬ
//֧��RGB8��RGB16��RGBA8��RGBA16.
//8bitÿ���صĵĵ�ɫ��
//GRAY8��GRAY16
inline int load_texture_png(Texture_p t,char *filename)
{
	

	png_FILE_p fp = fopen(filename,"rb");
	if (!fp)
		return 0;

	unsigned char header[8]={0};
	if (fread(header,1,8,fp) != 8)
		return 0;

	int is_png = !png_sig_cmp(&header[0],0,8);

	if (!is_png)
		return 0;

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
	if (!png_ptr)
		return 0;

	png_infop   info_ptr= png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr,NULL,NULL);
		return 0;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr,&info_ptr,png_infopp(NULL));
		fclose(fp);
		return 0;
	}
	//int res = png_image_begin_read()
	png_init_io(png_ptr,fp);
	
	//��Ϊǰ���Ѿ����õ�fread ��8���ֽڣ�������Ҫ����libpng��һЩ�ֽ��Ѿ����ļ���ʼ������ȡ��ȥ��
	png_set_sig_bytes(png_ptr, 8);
 
	//�� png_read_png֮ǰ���û�ȡ����ֵ
	t->w = png_get_image_width(png_ptr,info_ptr);
	
	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr,info_ptr,&t->w,&t->h,&t->bit_depth,&t->color_type,NULL,NULL,NULL);

	
	if (t->color_type == PNG_COLOR_TYPE_PALETTE)
	{
		printf("png_set_palette_to_rgb\n");
		//png_set_palette_to_rgb(png_ptr);
	}
		
	if (png_get_valid(png_ptr, info_ptr,PNG_INFO_tRNS)) 
		png_set_tRNS_to_alpha(png_ptr);

	if (t->color_type == PNG_COLOR_TYPE_GRAY &&
		t->bit_depth < 8) 
	{
		printf("png_set_expand_gray_1_2_4_to_8\n");
		//png_set_expand_gray_1_2_4_to_8(png_ptr);
	}

	//�����alpha��
	unsigned int filler = 255;
	if (t->color_type == PNG_COLOR_TYPE_RGB ||
		t->color_type == PNG_COLOR_TYPE_GRAY)
	{
		printf("png_set_add_alpha..\n");
		//png_set_add_alpha(png_ptr, filler, PNG_FILLER_AFTER);
	}

#if 0
	float screen_gamma = 0.45455;
	double file_gamma;
	if (png_get_gAMA(png_ptr, info_ptr, &file_gamma))
		png_set_gamma(png_ptr, screen_gamma, file_gamma);

	else
		png_set_gamma(png_ptr, screen_gamma, 0.45455);
#endif
	
	//������õ�ɫ��ͼ��ת����rgbͼ��
	//png_set_expand(png_ptr);

	//���Ƶ�ɫ������
	if (t->color_type == PNG_COLOR_TYPE_PALETTE)
	{
		printf("get png palette .. \n");
		//png_set_palette_to_rgb(png_ptr);

		printf("copy png palette .. \n");
		png_colorp palette;
		int num_palette;
		png_get_PLTE(png_ptr,info_ptr,&palette,&num_palette);
		if (num_palette>0)
		{
			t->num_palette = num_palette;
			t->palettep=(texture_palettep)malloc(sizeof(texture_palette) * num_palette);
			for (unsigned int i=0;i<num_palette;i++)
			{
				t->palettep[i].r = palette[i].red;
				t->palettep[i].g = palette[i].green;
				t->palettep[i].b = palette[i].blue;
				printf("%d,%d,%d\n",t->palettep[i].r,t->palettep[i].g,t->palettep[i].b);
			}
		}
	}

	png_read_update_info(png_ptr,info_ptr);
	png_get_IHDR(png_ptr,info_ptr,&t->w,&t->h,&t->bit_depth,&t->color_type,NULL,NULL,NULL);
	//png_read_png(png_ptr,info_ptr,PNG_TRANSFORM_IDENTITY,NULL);

	/*t->w = png_get_image_width(png_ptr,info_ptr);
	t->h = png_get_image_height(png_ptr,info_ptr);

	int color_type = png_get_color_type(png_ptr,info_ptr);
	int bit_depth = png_get_bit_depth(png_ptr,info_ptr);*/

	/***************************************************
	bit_depth :	������ɫͼ��1��2��4��8 
				�Ҷ�ͼ��1��2��4��8��16 
				���ɫͼ��8��16
	****************************************************/
	//ÿ�����������bit
	int per_pixel_bit_size = 0;
	//from pngvalid.c bit_size() function
	switch (t->color_type)
	{
	default: png_error(png_ptr, "invalid color type");
	case 0:  per_pixel_bit_size = t->bit_depth;	//PNG_COLOR_TYPE_GRAY	0��ֻ�лҶȡ�
			break;
	case 2:  per_pixel_bit_size = 3*t->bit_depth;//PNG_COLOR_MASK_COLOR(PNG_COLOR_TYPE_RGB) 2����GBA ��3���ֽ�
			break;
	case 3:  per_pixel_bit_size = t->bit_depth;	//PNG_COLOR_TYPE_PALETTE 3����ɫ����ɫ
			break;
	case 4:  per_pixel_bit_size = 2*t->bit_depth;//PNG_COLOR_TYPE_GRAY_ALPHA 4���лҶ�ֵ��alpha
			break;
	case 6: per_pixel_bit_size = 4*t->bit_depth;//PNG_COLOR_TYPE_RGB_ALPHA 6 ����RGBA ��4 ���ֽ�
			break;
	}

	t->pixel_bit_size = per_pixel_bit_size;
	t->pixel_byte_size = per_pixel_bit_size / 8;
	assert(per_pixel_bit_size % 8==0);//pixel_size һ���������ֽڲ��аɣ�

	if (t->h > PNG_UINT_32_MAX / (sizeof(png_byte)))
	{
		assert( 0 && "Image is too tall to process in memory");
	}
	if (t->w > PNG_UINT_32_MAX / t->pixel_byte_size)
	{
		assert( 0 && "Image is too wide to process in memory");
	}

	//�ȷ�����ָ��
	t->row_pointers = (unsigned char**)malloc(t->h* sizeof(png_bytep));
	//�ٷ��������ڴ�
	t->pixel_data	= (unsigned char *)malloc( (t->w*t->h) * t->pixel_byte_size);
	//ÿһ��ռ�õ��ֽ�
	int row_byte_size = t->w * t->pixel_byte_size;
	//����ָ��ָ���Ӧ�ĵ�ַ
	for (int i=0;i<t->h;i++)
	{
		t->row_pointers[i] = NULL;
		t->row_pointers[i] = &(t->pixel_data[i * row_byte_size]);
	}
	//If you use png_set_rows(), the application is responsible for freeing
	//row_pointers (and row_pointers[i], if they were separately allocated).
	png_set_rows(png_ptr,info_ptr,t->row_pointers);

	png_read_image(png_ptr,&t->row_pointers[0]);
	//png_read_png(png_ptr,info_ptr,PNG_TRANSFORM_IDENTITY,NULL);
	
	//number of bytes needed to hold a row
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	
	png_bytepp row_pointers = png_get_rows(png_ptr,info_ptr);
 	return 0;
}

// x y start form 0
inline void * get_texture_at(Texture_p tp,int x,int y)
{
	char *p = (char*)(tp->row_pointers);
	char *pd = p + y*tp->pixel_bit_size * tp->w + x * tp-> pixel_bit_size; 
	return pd;	
}

inline void get_rgba_at(Texture_p tp,int x,int y,int *r,int *g,int *b,int *a)
{

}

#endif