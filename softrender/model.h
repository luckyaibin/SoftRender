#ifndef __MODEL_H__
#define __MODEL_H__

#include "matrix.h"
#include "tool.h"
#include "triangle.h"
#include "vector.h"
#include "comm_headers.h"
#include "camera.h"
#include <fstream>
#define MAX_OBJECT_VERTICES 1024
#define MAX_OBJECT_TRIANGLES 1024

enum TRANS_TYPE{TT_LOCAL,TT_TRANS,TT_LOCAL_TO_TRANS};

struct Object_Type;
typedef Object_Type*  Ojbect_Ptr;
//����Ķ���Σ������εĶ��壩
typedef struct Poly_Type
{
	Ojbect_Ptr p_parent;//ָ��object��ָ��
	int vertex_index[3];//p_triangle_list���������������
	vector3 norm;
}Poly,*Poly_Ptr;


//����Ķ���
typedef struct Object_Type{
	vector3 world_coord;//��������������ϵ������
	//�任����ʱʹ��
	int vertex_count;//������
	vertex3d vertex_data_local[MAX_OBJECT_VERTICES];//���㣬ÿ�������������������������
	vertex3d vertex_data_transformed[MAX_OBJECT_VERTICES];
	
	//���Ƶ�ʱ��ʹ��
	int triangle_count;//����������
	//triangle3d triangles[MAX_OsBJECT_TRIANGLES];//�����ε�����
	Poly_Type triangle_list[MAX_OBJECT_TRIANGLES];//�������б�����

	

}Object,*Ojbect_Ptr;


void ObjectInit_old(Ojbect_Ptr obj)
{
	obj->world_coord = vector3(0,0,0);
	obj->vertex_count = 4;
	//obj.triangle_list[0] = 
	vertex3d *v = new vertex3d();
	v->x = 0;
	v->y = 0;
	v->z = 2;
	v->color = RGB(255,0,0);
	obj->vertex_data_local[0] = *v;

	v = new vertex3d();
	v->x = -1;
	v->y = -1;
	v->z = 0;
	v->color = RGB(0,255,0);
	obj->vertex_data_local[1] = *v;

	v = new vertex3d();
	v->x = 1;
	v->y = -1;
	v->z = 0;
	v->color = RGB(0,0,255);
	obj->vertex_data_local[2] = *v;

	v = new vertex3d();
	v->x = 0;
	v->y = 1;
	v->z = 0;
	v->color = RGB(0,0,100);
	obj->vertex_data_local[3] = *v;

	obj->triangle_count = 4;
	Poly p;
	p.p_parent = obj;
	p.vertex_index[0]=0;
	p.vertex_index[1]=3;
	p.vertex_index[2]=1;
	obj->triangle_list[0] = p;

	p.p_parent = obj;
	p.vertex_index[0]=0;
	p.vertex_index[1]=1;
	p.vertex_index[2]=2;
	obj->triangle_list[1] = p;


	p.p_parent = obj;
	p.vertex_index[0]=0;
	p.vertex_index[1]=2;
	p.vertex_index[2]=3;
	obj->triangle_list[2] = p;


	p.p_parent = obj;
	p.vertex_index[0]=1;
	p.vertex_index[1]=3;
	p.vertex_index[2]=2;
	obj->triangle_list[3] = p;

}
void ObjectInit(Ojbect_Ptr obj)
{
	obj->world_coord = vector3(0,0,0);
	obj->vertex_count = 12;
	//obj.triangle_list[0] = 
	vertex3d *v = new vertex3d();
	v->x = 0;
	v->y = 0;
	v->z = 2;
	v->color = RGB(255,0,0);
	obj->vertex_data_local[0] = *v;

	v = new vertex3d();
	v->x = -1;
	v->y = -1;
	v->z = 0;
	v->color = RGB(0,255,0);
	obj->vertex_data_local[1] = *v;

	v = new vertex3d();
	v->x = 1;
	v->y = -1;
	v->z = 0;
	v->color = RGB(0,0,255);
	obj->vertex_data_local[2] = *v;

	v = new vertex3d();
	v->x = 0;
	v->y = 1;
	v->z = 0;
	v->color = RGB(0,0,100);
	obj->vertex_data_local[3] = *v;

	obj->triangle_count = 4;
	Poly p;
	p.p_parent = obj;
	p.vertex_index[0]=0;
	p.vertex_index[1]=3;
	p.vertex_index[2]=1;
	obj->triangle_list[0] = p;

	p.p_parent = obj;
	p.vertex_index[0]=0;
	p.vertex_index[1]=1;
	p.vertex_index[2]=2;
	obj->triangle_list[1] = p;


	p.p_parent = obj;
	p.vertex_index[0]=0;
	p.vertex_index[1]=2;
	p.vertex_index[2]=3;
	obj->triangle_list[2] = p;


	p.p_parent = obj;
	p.vertex_index[0]=1;
	p.vertex_index[1]=3;
	p.vertex_index[2]=2;
	obj->triangle_list[3] = p;

}

void ObjectDraw(Ojbect_Ptr p_obj,int screen_w,int screen_h)
{
	for (int i=0;i<p_obj->triangle_count;i++)
	{
		Poly_Type poly = p_obj->triangle_list[i];
		vertex3d v0 = p_obj->vertex_data_transformed[poly.vertex_index[0]];
		vertex3d v1 = p_obj->vertex_data_transformed[poly.vertex_index[1]];
		vertex3d v2 = p_obj->vertex_data_transformed[poly.vertex_index[2]];

		vertex2d v00,v11,v22;
		v00.x = v0.x;
		v00.y = v0.y;
		v00.color = v0.color;

		v11.x = v1.x;
		v11.y = v1.y;
		v11.color = v1.color;

		v22.x = v2.x;
		v22.y = v2.y;
		v22.color = v2.color;
		/*
		v00.x += 1;
		v00.y += 1;

		v11.x += 1;
		v11.y += 1;

		v22.x += 1;
		v22.y += 1;

		v00.x *= screen_w;
		v00.y *= screen_h;

		v11.x *= screen_w;
		v11.y *= screen_h;

		v22.x *= screen_w;
		v22.y *= screen_h;
		*/
		/*if (v00.x<0)
			v00.x = 0;
		if (v00.x>screen_w)
			v00.x=screen_w;
		if (v00.y<0)
			v00.y=0;
		if (v00.y>screen_h)
			v00.y=screen_h;

		if (v11.x<0)
			v11.x = 0;
		if (v11.x>screen_w)
			v11.x=screen_w;
		if (v11.y<0)
			v11.y=0;
		if (v11.y>screen_h)
			v11.y=screen_h;

		if (v22.x<0)
			v22.x = 0;
		if (v22.x>screen_w)
			v22.x=screen_w;
		if (v22.y<0)
			v22.y=0;
		if (v22.y>screen_h)
			v22.y=screen_h;*/

		DrawTriangleWithEdgeEquation(v00,v11,v22,0);
	}
}

void ObjectTransform(Ojbect_Ptr p_obj,Matrix4 mt,TRANS_TYPE tt)
{
	switch (tt)
	{
	case TT_LOCAL:
		for (int i=0;i<p_obj->vertex_count;i++)
		{
			vertex3d vx = p_obj->vertex_data_local[i];
			vector3 vr(vx.x,vx.y,vx.z);
			vr = mt*vr;
			p_obj->vertex_data_local[i].x = vr.x;
			p_obj->vertex_data_local[i].y = vr.y;
			p_obj->vertex_data_local[i].z = vr.z;
		}
		break;
	case TT_TRANS:
		for (int i=0;i<p_obj->vertex_count;i++)
		{
			vertex3d vx = p_obj->vertex_data_transformed[i];
			vector3 vr(vx.x,vx.y,vx.z);
			vr = mt*vr;
			p_obj->vertex_data_transformed[i].x = vr.x;
			p_obj->vertex_data_transformed[i].y = vr.y;
			p_obj->vertex_data_transformed[i].z = vr.z;
		}
		break;
	case TT_LOCAL_TO_TRANS:
		for (int i=0;i<p_obj->vertex_count;i++)
		{
			vertex3d vx = p_obj->vertex_data_local[i];
			vector3 vr(vx.x,vx.y,vx.z);
			vr = mt*vr;
			p_obj->vertex_data_transformed[i].x = vr.x;
			p_obj->vertex_data_transformed[i].y = vr.y;
			p_obj->vertex_data_transformed[i].z = vr.z;
			p_obj->vertex_data_transformed[i].color = vx.color;

		}
		break;
	default:break;
	}
}
//����������ת������������
void ObjectWorldTransform(Ojbect_Ptr p_obj,TRANS_TYPE tt)
{
	switch (tt)
	{
	case TT_LOCAL:
		for (int i=0;i<p_obj->vertex_count;i++)
		{
			vertex3d v =  p_obj->vertex_data_local[i];
			v.x += p_obj->world_coord.x;
			v.y += p_obj->world_coord.y;
			v.z += p_obj->world_coord.z;
			p_obj->vertex_data_local[i] = v;
		}
		break;
	case TT_TRANS:
		for (int i=0;i<p_obj->vertex_count;i++)
		{
			vertex3d v =  p_obj->vertex_data_transformed[i];
			v.x += p_obj->world_coord.x;
			v.y += p_obj->world_coord.y;
			v.z += p_obj->world_coord.z;
			p_obj->vertex_data_transformed[i] = v;
		}
		break;
	case TT_LOCAL_TO_TRANS:
		for (int i=0;i<p_obj->vertex_count;i++)
		{
			vertex3d v =  p_obj->vertex_data_local[i];
			v.x += p_obj->world_coord.x;
			v.y += p_obj->world_coord.y;
			v.z += p_obj->world_coord.z;
			vertex3d_dump(v);
			p_obj->vertex_data_transformed[i] = v;
			
		}
		break;
	default:break;
	}
}

//��������任
void ObjectCameraTransform(Ojbect_Ptr p_obj,UVNCamera_Ptr p_camera)
{	
	//����ת
	/*Matrix3 camera_rotate_matrix = get_matrix_from_x_y_z_axis_angle(-camera->angle_x,-camera->angle_y,-camera->angle_z);
	//��ƽ��
	for (int i=0;i<p_ojb->vertex_count;i++)
	{
		vertex3d v =  p_ojb->vertex_data_transformed[i];
		v.x -= camera->world_pos.x;
		v.y -= camera->world_pos.y;
		v.z -= camera->world_pos.z;

		vector3 vv(v.x,v.y,v.z);
		vv = camera_rotate_matrix*vv;
		v.x = vv.x;
		v.y = vv.y;
		v.z = vv.z;

		

		p_ojb->vertex_data_transformed[i] = v;
	}
	*/
	vector3 v;
	for (int i=0;i<p_obj->vertex_count;i++)
	{	
		v.x = p_obj->vertex_data_transformed[i].x;
		v.y = p_obj->vertex_data_transformed[i].y;
		v.z = p_obj->vertex_data_transformed[i].z;
		vector_dump(v);
		//ִ������任
		v = p_camera->matrix_camera * v;
		vector_dump(v);
		//
		p_obj->vertex_data_transformed[i].x = v.x/v.z;
		p_obj->vertex_data_transformed[i].y = v.y/v.z;
		p_obj->vertex_data_transformed[i].z = v.z;
	}

}

//͸�ӱ任����3D����ת����2D����
void ObjectProjectTransform(Ojbect_Ptr p_obj,UVNCamera_Ptr p_camera)
{
	/*for (int i=0;i<p_obj->vertex_count;i++)
	{
		p_obj->vertex_data_transformed[i].x= p_obj->vertex_data_transformed[i].x / p_obj->vertex_data_transformed[i].z;
		p_obj->vertex_data_transformed[i].y= p_obj->vertex_data_transformed[i].y / p_obj->vertex_data_transformed[i].z;
	}*/
	vector3 v;
	for (int i=0;i<p_obj->vertex_count;i++)
	{	
		v.x = p_obj->vertex_data_transformed[i].x;
		v.y = p_obj->vertex_data_transformed[i].y;
		v.z = p_obj->vertex_data_transformed[i].z;

		//ִ��ͶӰ�任
		v = p_camera->matrix_projection * v;
		//
		p_obj->vertex_data_transformed[i].x = v.x/v.z;
		p_obj->vertex_data_transformed[i].y = v.y/v.z;
		p_obj->vertex_data_transformed[i].z = v.z;
	}
}


void ObjectScreenTransform(Ojbect_Ptr p_obj,UVNCamera_Ptr p_camera)
{
	vector3 v;
	for (int i=0;i<p_obj->vertex_count;i++)
	{	
		v.x = p_obj->vertex_data_transformed[i].x;
		v.y = p_obj->vertex_data_transformed[i].y;
		v.z = p_obj->vertex_data_transformed[i].z;

		//ִ����Ļ�任
		v = p_camera->matrix_screen * v;
		//�ѱ任������꿽����ȥ
		 p_obj->vertex_data_transformed[i].x = v.x;
		 p_obj->vertex_data_transformed[i].y = v.y;
		 p_obj->vertex_data_transformed[i].z = v.z;
	}
}

#endif
