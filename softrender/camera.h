#ifndef __CAMERA_H__
#define __CAMERA_H__
#include"triangle.h"
#include"plane.h"
//相机类型：欧拉相机，unv相机
enum CAMERA_TYPE
{
	CT_EULER,
	CT_UVN,
};

typedef struct Camera_Type
{
	vector3 world_pos;
	float angle_x,angle_y,angle_z;//相机朝向，绕x，y，z分别旋转多少弧度
}Camera,*Camera_Ptr;

typedef struct UVNCamera_Type
{
	CAMERA_TYPE camera_type;

	vector3 world_pos;
	vector3 direction;	//相机朝向
	vector3 euler_cam_direction;//eruler相机时的默认朝向

	vector3 n;			//camera的注视点向量
	vector3 v;			//相机上方向向量
	vector3 u;			//相机右方向向量
	vector3 uvn_target_pos;	//unv相机的目标点

	int is_target_need_compute;//uvn是否需要根据朝向计算，1需要，0不需要（uvn_target）已给定

	float view_distance;//视距
	float FOV;			//视野角度
	float near_z;		//近裁剪距离
	float far_z;		//远裁剪距离

	plane3d clipplane_left,clipplane_right,clipplane_up,clipplane_down;
	float view_plane_width,view_plane_height;//视平面的宽和高？？？
	float screen_width,screen_height;		//屏幕宽高

	float screen_center_x,screen_center_y;//屏幕中心坐标

	float aspect_ratio;						//宽高比

	Matrix4 matrix_camera;					//相机变换矩阵
	Matrix4 matrix_projection;				//透视投影变换矩阵
	Matrix4 matrix_screen;					//屏幕变换矩阵
} UVNCamera,* UVNCamera_Ptr;



void InitUVNCamera(
	UVNCamera_Ptr	p_uvn_camera,
	CAMERA_TYPE		cam_type,
	vector3			cam_pos,
	vector3			euler_cam_direction,//当时eruler相机是，里面是相机的朝向
	vector3_ptr		p_target_pos,
	vector3_ptr		p_v,
	int				is_target_need_compute,//是否需要根据朝向来计算
	float			near_z,
	float			far_z,
	float			fov,
	float			screen_w,
	float			screen_h
	)
{
	p_uvn_camera->camera_type = cam_type;
	p_uvn_camera->world_pos = cam_pos;
	p_uvn_camera->euler_cam_direction = euler_cam_direction;
	//目标点的坐标
	if(p_target_pos)
		p_uvn_camera->uvn_target_pos = *p_target_pos;
	//上向量
	if (p_v)
		p_uvn_camera->v = *p_v;	

	p_uvn_camera->is_target_need_compute = is_target_need_compute;

	p_uvn_camera->near_z = near_z;
	p_uvn_camera->far_z = far_z;
	p_uvn_camera->screen_width = screen_w;
	p_uvn_camera->screen_height = screen_h;
	p_uvn_camera->screen_center_x = (screen_w-1)/2;
	p_uvn_camera->screen_center_y = (screen_h-1)/2;
	p_uvn_camera->aspect_ratio = (float)screen_w/(float)screen_h;//屏幕的宽高比

	p_uvn_camera->FOV = fov;

	p_uvn_camera->view_plane_width = 2.0;//宽固定为2？？？
	p_uvn_camera->view_plane_height = 2.0/p_uvn_camera->aspect_ratio;//视平面的高根据屏幕的比例变化

	p_uvn_camera->view_distance = 1/tan(fov*0.5); //视距

	vector3 point_on_plane(0,0,0);
	vector3 plane_normal;//平面法线
	plane3d plane;
	plane.p = point_on_plane;//所有裁剪面都是过原点的
	/*if (fov==90.0)
	{
		//右侧
		plane_normal = vector3(1,0,1);
		plane.n = plane_normal;
		normalize(&plane);
		p_uvn_camera->clipplane_right = plane;

		//左侧
		plane_normal = vector3(-1,1,0);
		plane.n = plane_normal;
		normalize(&plane);
		p_uvn_camera->clipplane_left = plane;

		//上侧
		plane_normal = vector3(-1,1,0);
		plane.n = plane_normal;
		normalize(&plane);
		p_uvn_camera->clipplane_left = plane;
	}*/
	//右侧
	plane_normal = vector3(p_uvn_camera->view_distance,0,p_uvn_camera->view_plane_width*0.5);
	plane.n = plane_normal;
	normalize(&plane);
	p_uvn_camera->clipplane_right = plane;

	//左侧
	plane_normal = vector3(-p_uvn_camera->view_distance,0,p_uvn_camera->view_plane_width*0.5);
	plane.n = plane_normal;
	normalize(&plane);
	p_uvn_camera->clipplane_left = plane;

	//上侧
	plane_normal = vector3(0,p_uvn_camera->view_distance,p_uvn_camera->view_plane_height*0.5);
	plane.n = plane_normal;
	normalize(&plane);
	p_uvn_camera->clipplane_left = plane;

	//下侧
	plane_normal = vector3(0,-p_uvn_camera->view_distance,p_uvn_camera->view_plane_height*0.5);
	plane.n = plane_normal;
	normalize(&plane);
	p_uvn_camera->clipplane_left = plane;
	 
	plane_normal.x = (p_uvn_camera->view_plane_width/2,p_uvn_camera->view_plane_height/2,-p_uvn_camera->view_distance);
}

void CameraUpdateMatrix(UVNCamera_Ptr	p_uvn_camera)
{
	Matrix4 m(	1,0,0,-p_uvn_camera->world_pos.x,
				0,1,0,-p_uvn_camera->world_pos.y,
				0,0,1,-p_uvn_camera->world_pos.z,
				1,0,0,1);

	//当时euler相机是，相机里的Direction
	if (p_uvn_camera->camera_type == CT_EULER)//欧拉相机
	{
		Matrix3 mEuler = get_matrix_from_x_y_z_axis_angle(p_uvn_camera->euler_cam_direction.x,
			p_uvn_camera->euler_cam_direction.y,
			p_uvn_camera->euler_cam_direction.z);

		p_uvn_camera->matrix_camera = mEuler * m;
	}
	else if(p_uvn_camera->camera_type == CT_UVN)
	{
		//根据欧拉角的值来计算目标点
		if (p_uvn_camera->is_target_need_compute)
		{
			float rotate_x = p_uvn_camera->euler_cam_direction.x;//绕x轴旋转的角度
			float rotate_y = p_uvn_camera->euler_cam_direction.y;//绕y轴旋转的角度
			float rotate_z = p_uvn_camera->euler_cam_direction.z;//绕z轴旋转的角度

			
			float x =cos(rotate_z);
			float y =sin(rotate_z);



		}
	}
}
#endif