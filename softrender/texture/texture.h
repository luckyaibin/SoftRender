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
	int pixel_bit_size;//纹理像素的bit数据大小
	int pixel_byte_size;
	int bit_depth;
	int color_type;
	unsigned char **row_pointers;
	unsigned char * pixel_data;

	//color map color形式（调色板）
	texture_palettep palettep;
	unsigned int num_palette;
}Texture,*Texture_p;



//colormap： bpp 为8bit ,调色板数据没有alpha通道
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

//gray： bpp 为8bit 一个颜色深度（bpc)大小为16bit，没有alpha通道
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

//gray alpha：bbc位8bit 一个颜色深度（bpc)大小为8bit，有alpha通道
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

//gray： bpp 为16 bit 一个颜色深度（bpc)大小为16bit，没有alpha通道
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

//gray alpha：bbc位16bit 一个颜色深度（bpc)大小为16bit，有alpha通道
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

//RGB： bpc位8bit 一个颜色深度（bpc)大小为16bit，没有alpha通道
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

//RGBA：bpc位8bit 一个颜色深度（bpc)大小为8bit，有alpha通道
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


//RGB: pbc位16bit 一个颜色深度（bpc)大小为16bit，没有alpha通道
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

//RGBA: pbc位16bit 一个颜色深度（bpc)大小为16bit，有alpha通道
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
	//现在只支持bit_depth为8的 索引彩色、灰度。bit_depth为8或16的彩色。
	/***************************************************
	bit_depth :	索引彩色图像：1，2，4或8 
				灰度图像：1，2，4，8或16 
				真彩色图像：8或16
	****************************************************/
	get_pixel_functionp p = 0;
	//from pngvalid.c bit_size() function
	int per_pixel_bit_size;
	switch (color_type)
	{
	default: 
		break;
	case 0:  //PNG_COLOR_TYPE_GRAY	0。只有灰度。
		{
			if (bit_depth==8)
				p=get_pixel_at_8bpp_gray;
			if(bit_depth==16)
				p=get_pixel_at_16bpp_gray;
		}
			break;
	case 2: //PNG_COLOR_MASK_COLOR(PNG_COLOR_TYPE_RGB) 2。有GBA 共3个字节
		{
			if (bit_depth==8)
				p=get_pixel_at_8bpc_rgb;
			if(bit_depth==16)
				p=get_pixel_at_16bpc_rgb;
		}
			break;
	case 3: //PNG_COLOR_TYPE_PALETTE 3。调色板颜色
		{
			if (bit_depth==8)
				p=get_pixel_at_8bpp_palette;
			else
				assert("not supported color-map png.");
		}
		break;
	case 4: //PNG_COLOR_TYPE_GRAY_ALPHA 4。有灰度值和alpha
		{
			if (bit_depth==8)
				p=get_pixel_at_8bpp_graya;
			if(bit_depth==16)
				p=get_pixel_at_16bpp_graya;
		}
		break;
	case 6://PNG_COLOR_TYPE_RGB_ALPHA 6 。有RGBA 共4 个字节
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

//支持一下格式的png图片
//支持RGB8，RGB16，RGBA8，RGBA16.
//8bit每像素的的调色板
//GRAY8，GRAY16
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
	
	//因为前面已经调用的fread 了8个字节，所以需要告诉libpng有一些字节已经从文件开始处被读取出去了
	png_set_sig_bytes(png_ptr, 8);
 
	//在 png_read_png之前调用获取不到值
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

	//不添加alpha了
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
	
	//这个会让调色板图像转换成rgb图像。
	//png_set_expand(png_ptr);

	//复制调色板数据
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
	bit_depth :	索引彩色图像：1，2，4或8 
				灰度图像：1，2，4，8或16 
				真彩色图像：8或16
	****************************************************/
	//每个像素所需的bit
	int per_pixel_bit_size = 0;
	//from pngvalid.c bit_size() function
	switch (t->color_type)
	{
	default: png_error(png_ptr, "invalid color type");
	case 0:  per_pixel_bit_size = t->bit_depth;	//PNG_COLOR_TYPE_GRAY	0。只有灰度。
			break;
	case 2:  per_pixel_bit_size = 3*t->bit_depth;//PNG_COLOR_MASK_COLOR(PNG_COLOR_TYPE_RGB) 2。有GBA 共3个字节
			break;
	case 3:  per_pixel_bit_size = t->bit_depth;	//PNG_COLOR_TYPE_PALETTE 3。调色板颜色
			break;
	case 4:  per_pixel_bit_size = 2*t->bit_depth;//PNG_COLOR_TYPE_GRAY_ALPHA 4。有灰度值和alpha
			break;
	case 6: per_pixel_bit_size = 4*t->bit_depth;//PNG_COLOR_TYPE_RGB_ALPHA 6 。有RGBA 共4 个字节
			break;
	}

	t->pixel_bit_size = per_pixel_bit_size;
	t->pixel_byte_size = per_pixel_bit_size / 8;
	assert(per_pixel_bit_size % 8==0);//pixel_size 一定是整数字节才行吧？

	if (t->h > PNG_UINT_32_MAX / (sizeof(png_byte)))
	{
		assert( 0 && "Image is too tall to process in memory");
	}
	if (t->w > PNG_UINT_32_MAX / t->pixel_byte_size)
	{
		assert( 0 && "Image is too wide to process in memory");
	}

	//先分配行指针
	t->row_pointers = (unsigned char**)malloc(t->h* sizeof(png_bytep));
	//再分配真正内存
	t->pixel_data	= (unsigned char *)malloc( (t->w*t->h) * t->pixel_byte_size);
	//每一行占用的字节
	int row_byte_size = t->w * t->pixel_byte_size;
	//让行指针指向对应的地址
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