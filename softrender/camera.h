#ifndef __CAMERA_H__
#define __CAMERA_H__
#include"triangle.h"

typedef struct Camera_Type
{
	vector3 word_pos;
	float angle_x,angle_y,angle_z;//���������x��y��z�ֱ���ת���ٻ���
}Camera,*Camera_Ptr;


#endif