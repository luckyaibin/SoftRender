#ifndef __CAMERA_H__
#define __CAMERA_H__
#include"triangle.h"

typedef struct Camera_Type
{
	vector3 word_pos;
	float angle_x,angle_y,angle_z;//相机朝向，绕x，y，z分别旋转多少弧度
}Camera,*Camera_Ptr;


#endif