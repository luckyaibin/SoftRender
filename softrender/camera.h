#ifndef __CAMERA_H__
#define __CAMERA_H__
#include"triangle.h"
#include"plane.h"
//������ͣ�ŷ�������unv���
enum CAMERA_TYPE
{
	CT_EULER,
	CT_UVN,
};

typedef struct Camera_Type
{
	vector3 world_pos;
	float angle_x,angle_y,angle_z;//���������x��y��z�ֱ���ת���ٻ���
}Camera,*Camera_Ptr;

typedef struct UVNCamera_Type
{
	CAMERA_TYPE camera_type;

	vector3 world_pos;
	vector3 direction;	//�������
	vector3 euler_cam_direction;//eruler���ʱ��Ĭ�ϳ���

	vector3 n;			//camera��ע�ӵ�����
	vector3 v;			//����Ϸ�������
	vector3 u;			//����ҷ�������
	vector3 uvn_target_pos;	//unv�����Ŀ���

	int is_target_need_compute;//uvn�Ƿ���Ҫ���ݳ�����㣬1��Ҫ��0����Ҫ��uvn_target���Ѹ���

	float view_distance;//�Ӿ�
	float FOV;			//��Ұ�Ƕ�
	float near_z;		//���ü�����
	float far_z;		//Զ�ü�����

	plane3d clipplane_left,clipplane_right,clipplane_up,clipplane_down;
	float view_plane_width,view_plane_height;//��ƽ��Ŀ�͸ߣ�����
	float screen_width,screen_height;		//��Ļ���

	float screen_center_x,screen_center_y;//��Ļ��������

	float aspect_ratio;						//��߱�

	Matrix4 matrix_camera;					//����任����
	Matrix4 matrix_projection;				//͸��ͶӰ�任����
	Matrix4 matrix_screen;					//��Ļ�任����
} UVNCamera,* UVNCamera_Ptr;



void InitUVNCamera(
	UVNCamera_Ptr	p_uvn_camera,
	CAMERA_TYPE		cam_type,
	vector3			cam_pos,
	vector3_ptr		p_euler_cam_direction,//��ʱeruler����ǣ�����������ĳ���
	vector3_ptr		p_target_pos,
	vector3_ptr		p_v,
	int				is_target_need_compute,//�Ƿ���Ҫ���ݳ���������
	float			near_z,
	float			far_z,
	float			fov,
	float			screen_w,
	float			screen_h
	)
{
	p_uvn_camera->camera_type = cam_type;
	p_uvn_camera->world_pos = cam_pos;
	if (p_euler_cam_direction)
		p_uvn_camera->euler_cam_direction = *p_euler_cam_direction;
	//Ŀ��������
	if(p_target_pos)
		p_uvn_camera->uvn_target_pos = *p_target_pos;

	//������Ϊ�գ�������
	if ( (NULL == p_euler_cam_direction)&&(NULL==p_target_pos) )
	{
		exit(-1);
	}
	//������
	if (p_v)
		p_uvn_camera->v = *p_v;	
	//���Ϊunv�����һ��Ҫ����v����
	if ( (CT_UVN == cam_type) && (NULL == p_v))
	{
		exit(-1);
	}
	p_uvn_camera->is_target_need_compute = is_target_need_compute;

	p_uvn_camera->near_z = near_z;
	p_uvn_camera->far_z = far_z;
	p_uvn_camera->screen_width = screen_w;
	p_uvn_camera->screen_height = screen_h;
	p_uvn_camera->screen_center_x = (screen_w-1)/2;
	p_uvn_camera->screen_center_y = (screen_h-1)/2;
	p_uvn_camera->aspect_ratio = (float)screen_w/(float)screen_h;//��Ļ�Ŀ�߱�

	p_uvn_camera->FOV = fov;

	p_uvn_camera->view_plane_width = 2.0;//��̶�Ϊ2������
	p_uvn_camera->view_plane_height = 2.0/p_uvn_camera->aspect_ratio;//��ƽ��ĸ߸�����Ļ�ı����仯

	p_uvn_camera->view_distance = 1/tan(fov*0.5); //�Ӿ�

	vector3 point_on_plane(0,0,0);
	vector3 plane_normal;//ƽ�淨��
	plane3d plane;
	plane.p = point_on_plane;//���вü��涼�ǹ�ԭ���
	/*if (fov==90.0)
	{
		//�Ҳ�
		plane_normal = vector3(1,0,1);
		plane.n = plane_normal;
		normalize(&plane);
		p_uvn_camera->clipplane_right = plane;

		//���
		plane_normal = vector3(-1,1,0);
		plane.n = plane_normal;
		normalize(&plane);
		p_uvn_camera->clipplane_left = plane;

		//�ϲ�
		plane_normal = vector3(-1,1,0);
		plane.n = plane_normal;
		normalize(&plane);
		p_uvn_camera->clipplane_left = plane;
	}*/
	//�Ҳ�
	plane_normal = vector3(p_uvn_camera->view_distance,0,p_uvn_camera->view_plane_width*0.5);
	plane.n = plane_normal;
	normalize(&plane);
	p_uvn_camera->clipplane_right = plane;

	//���
	plane_normal = vector3(-p_uvn_camera->view_distance,0,p_uvn_camera->view_plane_width*0.5);
	plane.n = plane_normal;
	normalize(&plane);
	p_uvn_camera->clipplane_left = plane;

	//�ϲ�
	plane_normal = vector3(0,p_uvn_camera->view_distance,p_uvn_camera->view_plane_height*0.5);
	plane.n = plane_normal;
	normalize(&plane);
	p_uvn_camera->clipplane_left = plane;

	//�²�
	plane_normal = vector3(0,-p_uvn_camera->view_distance,p_uvn_camera->view_plane_height*0.5);
	plane.n = plane_normal;
	normalize(&plane);
	p_uvn_camera->clipplane_left = plane;
	 
	plane_normal.x = (p_uvn_camera->view_plane_width/2,p_uvn_camera->view_plane_height/2,-p_uvn_camera->view_distance);
}

void CameraUpdateMatrix(UVNCamera_Ptr	p_uvn_camera)
{
	Matrix4 m_move(	1,0,0,-p_uvn_camera->world_pos.x,
				0,1,0,-p_uvn_camera->world_pos.y,
				0,0,1,-p_uvn_camera->world_pos.z,
				1,0,0,1);

	//��ʱeuler����ǣ�������Direction
	if (p_uvn_camera->camera_type == CT_EULER)//ŷ�����
	{
		Matrix3 mEuler = get_matrix_from_x_y_z_axis_angle(p_uvn_camera->euler_cam_direction.x,
			p_uvn_camera->euler_cam_direction.y,
			p_uvn_camera->euler_cam_direction.z);

		p_uvn_camera->matrix_camera =mEuler * m_move;
	}
	else if(p_uvn_camera->camera_type == CT_UVN)
	{
		//����ŷ���ǵ�ֵ������Ŀ���
		if (p_uvn_camera->is_target_need_compute)
		{
			Matrix3 mEuler = get_matrix_from_x_y_z_axis_angle(p_uvn_camera->euler_cam_direction.x,
				p_uvn_camera->euler_cam_direction.y,
				p_uvn_camera->euler_cam_direction.z);
			vector3 v(0,0,-1);
			v =mEuler * v;
			p_uvn_camera->uvn_target_pos.x = v.x;
			p_uvn_camera->uvn_target_pos.y = v.y;
			p_uvn_camera->uvn_target_pos.z = v.z;

		}

		vector3 u,v,n;
		n = p_uvn_camera->uvn_target_pos - p_uvn_camera->world_pos;
		v = p_uvn_camera->v;
		u = cross_mul(n,v);

		v = cross_mul(u,n);

		normalize(&u);
		normalize(&v);
		normalize(&n);

		Matrix4 m4(
			p_uvn_camera->u.x,	p_uvn_camera->v.x,	p_uvn_camera->n.x,	0,
			p_uvn_camera->u.y,	p_uvn_camera->v.y,	p_uvn_camera->n.y,	0,
			p_uvn_camera->u.z,	p_uvn_camera->v.z,	p_uvn_camera->n.z,	0,
			0,					0,					0,					1);
		p_uvn_camera->matrix_camera = m4*m_move;
	}

	//����ͶӰ����
	Matrix4 m_projection(
							p_uvn_camera->view_distance,0,							0,								0,
							0,							p_uvn_camera->view_distance,0,								0,
							0,							0,							1,								0,
							0,							0,							1,								0);
	p_uvn_camera->matrix_projection = m_projection;

	//������Ļת������
	Matrix4 m_screen(
						p_uvn_camera->screen_width/2,			0,									0,				p_uvn_camera->screen_width/2,
						0,										p_uvn_camera->screen_height/2,		0,				p_uvn_camera->screen_height/2,
						0,										0,									1,				0,
						0,										0,									0,				1);
	p_uvn_camera->matrix_screen = m_screen;
}
#endif