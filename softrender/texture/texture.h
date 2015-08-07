#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "../../thirdparty/include/libpng/png.h"
#include <assert.h>
#include <malloc.h>

#ifndef MAX_PATH
	#define MAX_PATH 256
#endif

typedef 
struct SR_Texture_TYPE
{
	char name[MAX_PATH];
	unsigned int w;
	unsigned int h;
	int pixel_bit_size;//纹理像素的bit数据大小
	int pixel_size;
	int bit_depth;
	int color_type;
	unsigned char **row_pointers;
	unsigned char * pixel_data;
}Texture,*Texture_p;



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

	//把所有数据读取到info_ptr里
	/*if (t->bit_depth<16)
	{
	png_set_expand(png_ptr);
	}*/
	
	if (t->color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	if (png_get_valid(png_ptr, info_ptr,PNG_INFO_tRNS)) 
		png_set_tRNS_to_alpha(png_ptr);

	if (t->color_type == PNG_COLOR_TYPE_GRAY &&
		t->bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);

	unsigned int filler = 255;
	if (t->color_type == PNG_COLOR_TYPE_RGB ||
		t->color_type == PNG_COLOR_TYPE_GRAY)
		png_set_add_alpha(png_ptr, filler, PNG_FILLER_AFTER);

#if 0
	float screen_gamma = 0.45455;
	double file_gamma;
	if (png_get_gAMA(png_ptr, info_ptr, &file_gamma))
		png_set_gamma(png_ptr, screen_gamma, file_gamma);

	else
		png_set_gamma(png_ptr, screen_gamma, 0.45455);
#endif

	png_set_expand(png_ptr);


	png_read_update_info(png_ptr,info_ptr);
	png_get_IHDR(png_ptr,info_ptr,&t->w,&t->h,&t->bit_depth,&t->color_type,NULL,NULL,NULL);
	//png_read_png(png_ptr,info_ptr,PNG_TRANSFORM_IDENTITY,NULL);

	/*t->w = png_get_image_width(png_ptr,info_ptr);
	t->h = png_get_image_height(png_ptr,info_ptr);

	int color_type = png_get_color_type(png_ptr,info_ptr);
	int bit_depth = png_get_bit_depth(png_ptr,info_ptr);*/

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
	t->pixel_size = per_pixel_bit_size / 8;
	assert(per_pixel_bit_size % 8==0);//pixel_size 一定是整数字节才行吧？

	if (t->h > PNG_UINT_32_MAX / (sizeof(png_byte)))
	{
		assert( 0 && "Image is too tall to process in memory");
	}
	if (t->w > PNG_UINT_32_MAX / t->pixel_size)
	{
		assert( 0 && "Image is too wide to process in memory");
	}

	//先分配行指针
	t->row_pointers = (unsigned char**)malloc(t->h* sizeof(png_bytep));
	//再分配真正内存
	t->pixel_data	= (unsigned char *)malloc( (t->w*t->h) * t->pixel_size);
	//每一行占用的字节
	int row_byte_size = t->w * t->pixel_size;
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

	png_colorp palette;
	int num_palette;
	png_get_PLTE(png_ptr,info_ptr,&palette,&num_palette);

	//number of bytes needed to hold a row
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);


	png_bytepp row_pointers = png_get_rows(png_ptr,info_ptr);

	//png_get_color_type()

	/*png_bytepp image;
	png_read_image(png_ptr,image);*/

	return 0;
}

// x y start form 0
inline void * get_texture_at(Texture_p tp,int x,int y)
{
	char *p = (char*)(tp->row_pointers);
	char *pd = p + y*tp->pixel_bit_size * tp->w + x * tp-> pixel_bit_size; 
	return pd;	
}

#endif