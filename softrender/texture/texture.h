#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "../../thirdparty/include/libpng/png.h"


#ifndef MAX_PATH
	#define MAX_PATH 256
#endif

typedef 
struct SR_Texture_TYPE
{
	char name[MAX_PATH];
	int w;
	int h;
	int pixel_bit_size;//纹理像素的bit数据大小
	int bit_depth;
	void * pixel_data;
}Texture,*Texture_p;



inline int load_texture_png(Texture_p t,char *filename)
{
	

	png_FILE_p fp = fopen(filename,"rb");
	if (!fp)
	{
		return 0;
	}
	unsigned char header[8]={0};
	if (fread(header,1,8,fp) != 8)
	{
		return 0;
	}
	int is_png = !png_sig_cmp(&header[0],0,8);
	if (!is_png)
	{
		return 0;
	}
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
	if (!png_ptr)
	{
		return 0;
	}

	

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
	 
	//把所有数据读取到info_ptr里
	png_set_expand(png_ptr);
	png_read_png(png_ptr,info_ptr,PNG_TRANSFORM_IDENTITY,NULL);

	t->w = png_get_image_width(png_ptr,info_ptr);
	t->h = png_get_image_height(png_ptr,info_ptr);
	
	int color_type = png_get_color_type(png_ptr,info_ptr);
	int bit_depth = png_get_bit_depth(png_ptr,info_ptr);

	//每个像素所需的bit
	int per_pixel_bit_size = 0;
	//from pngvalid.c bit_size() function
	switch (color_type)
	{
	default: png_error(png_ptr, "invalid color type");
	case 0:  per_pixel_bit_size = bit_depth;	//PNG_COLOR_TYPE_GRAY	0。只有灰度。
			break;
	case 2:  per_pixel_bit_size = 3*bit_depth;//PNG_COLOR_MASK_COLOR(PNG_COLOR_TYPE_RGB) 2。有GBA 共3个字节
			break;
	case 3:  per_pixel_bit_size = bit_depth;	//PNG_COLOR_TYPE_PALETTE 3。调色板颜色
			break;
	case 4:  per_pixel_bit_size = 2*bit_depth;//PNG_COLOR_TYPE_GRAY_ALPHA 4。有灰度值和alpha
			break;
	case 6: per_pixel_bit_size = 4*bit_depth;//PNG_COLOR_TYPE_RGB_ALPHA 6 。有RGBA 共4 个字节
			break;
	}

	t->pixel_bit_size = per_pixel_bit_size;
	t->bit_depth = bit_depth;
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
	char *p = (char*)(tp->pixel_data);
	char *pd = p + y*tp->pixel_bit_size * tp->w + x * tp-> pixel_bit_size; 
	return pd;	
}

#endif