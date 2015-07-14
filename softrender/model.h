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
//物体的多边形（三角形的定义）
typedef struct Poly_Type
{
	Ojbect_Ptr p_parent;//指向object的指针
	int vertex_index[3];//p_triangle_list中三个顶点的索引
	vector3 norm;
}Poly,*Poly_Ptr;


//物体的定义
typedef struct Object_Type{
	vector3 world_coord;//物体在世界坐标系的坐标
	//变换坐标时使用
	int vertex_count;//顶点数
	vertex3d vertex_data_local[MAX_OBJECT_VERTICES];//顶点，每个顶点坐标是相对于物体坐标
	vertex3d vertex_data_transformed[MAX_OBJECT_VERTICES];
	
	//绘制的时候使用
	int triangle_count;//三角形数量
	//triangle3d triangles[MAX_OsBJECT_TRIANGLES];//三角形的数据
	Poly_Type triangle_list[MAX_OBJECT_TRIANGLES];//三角形列表数据

	int is_show_obj_coord;
	vertex3d obj_coords[6];
	//vertex3d coord_x_start,coord_x_end;
	//vertex3d coord_y_start,coord_y_end;
	//vertex3d coord_z_start,coord_z_end;


	

}Object,*Ojbect_Ptr;


void ObjectInit(Ojbect_Ptr obj)
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


	obj->obj_coords[0] = vertex3d(0,0,0,ARGB(255,255,0,0));
	obj->obj_coords[1]   = vertex3d(15,0,0,ARGB(255,255,0,0));

	obj->obj_coords[2] = vertex3d(0,0,0,ARGB(255,0,255,0));
	obj->obj_coords[3] = vertex3d(0,15,0,ARGB(255,0,255,0));

	obj->obj_coords[4] = vertex3d(0,0,0,ARGB(255,0,0,255));
	obj->obj_coords[5] = vertex3d(0,0,15,ARGB(255,0,0,255));

}
void ObjectInit_new(Ojbect_Ptr obj)
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
	for (int i=0;i<3;i++)
	{
		vertex3d vx0 = p_obj->obj_coords[2*i];
		vertex3d vx1 = p_obj->obj_coords[2*i+1];
		vertex2d v00,v11;
		v00.x = vx0.x;
		v00.y = vx0.y;
		v00.color = vx0.color;

		v11.x = vx1.x;
		v11.y = vx1.y;
		v11.color = vx1.color;


		if(v00.x < 0 )
			v00.x=0;
		if (v00.y < 0)
			v00.y = 0;
		if(v00.x>screen_w)
			v00.x = screen_w-1;
		if(v00.y>screen_h)
			v00.y = screen_h-1;

		if(v11.x < 0 )
			v11.x=0;
		if (v11.y < 0)
			v11.y = 0;
		if(v11.x>screen_w)
			v11.x = screen_w;
		if(v11.y>screen_h)
			v11.y = screen_h;

		DrawLine(v00.x,v00.y,v11.x,v11.y,v00.color);


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
	for (int i=0;i<6;i++)
	{
		vertex3d vx = p_obj->obj_coords[i];
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].x))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].y))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].z))
			printf("nan...");
		vector3 vr(vx.x,vx.y,vx.z);
		vr = mt*vr;
		if(IS_FLOAT_NAN(vr.x))
			printf("nan...");
		if(IS_FLOAT_NAN(vr.y))
			printf("nan...");
		if(IS_FLOAT_NAN(vr.z))
			printf("nan...");
		p_obj->obj_coords[i].x = vr.x;
		p_obj->obj_coords[i].y = vr.y;
		p_obj->obj_coords[i].z = vr.z;
	}
}
//把物体坐标转换成世界坐标
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
	for (int i=0;i<6;i++)
	{
		vertex3d v = p_obj->obj_coords[i];
		v.x += p_obj->world_coord.x;
		v.y += p_obj->world_coord.y;
		v.z += p_obj->world_coord.z;

		if(IS_FLOAT_NAN(v.x))
			printf("nan...");
		if(IS_FLOAT_NAN(v.y))
			printf("nan...");
		if(IS_FLOAT_NAN(v.z))
			printf("nan...");
		p_obj->obj_coords[i] = v;
	}
}

//物体相机变换
void ObjectCameraTransform(Ojbect_Ptr p_obj,UVNCamera_Ptr p_camera)
{	
	//再旋转
	/*Matrix3 camera_rotate_matrix = get_matrix_from_x_y_z_axis_angle(-camera->angle_x,-camera->angle_y,-camera->angle_z);
	//先平移
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
		//vector_dump(v);
		//执行相机变换
		v = p_camera->matrix_camera * v;
		//vector_dump(v);
		//
		p_obj->vertex_data_transformed[i].x = v.x/v.z;
		p_obj->vertex_data_transformed[i].y = v.y/v.z;
		p_obj->vertex_data_transformed[i].z = v.z;
	}

	for (int i=0;i<6;i++)
	{	
		vector3 vv;
		vv.x = p_obj->obj_coords[i].x;
		vv.y = p_obj->obj_coords[i].y;
		vv.z = p_obj->obj_coords[i].z;

		if(IS_FLOAT_NAN(vv.x))
			printf("nan...");
		if(IS_FLOAT_NAN(vv.y))
			printf("nan...");
		if(IS_FLOAT_NAN(vv.z))
			printf("nan...");


		if(IS_FLOAT_NAN(p_obj->obj_coords[i].x))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].y))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].z))
			printf("nan...");
		//vector_dump(v);
		//执行相机变换
		v = p_camera->matrix_camera * vv;
		if(IS_FLOAT_NAN(v.x))
			printf("nan...");
		if(IS_FLOAT_NAN(v.y))
			printf("nan...");
		if(IS_FLOAT_NAN(v.z))
			printf("nan...");
		//
		p_obj->obj_coords[i].x = v.x/v.z;
		p_obj->obj_coords[i].y = v.y/v.z;
		 
		p_obj->obj_coords[i].z = v.z;
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].x))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].y))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].z))
			printf("nan...");
	}

}

//透视变换，把3D坐标转换成2D坐标
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

		//执行投影变换
		v = p_camera->matrix_projection * v;
		//
		p_obj->vertex_data_transformed[i].x = v.x/v.z;
		p_obj->vertex_data_transformed[i].y = v.y/v.z;
		p_obj->vertex_data_transformed[i].z = v.z;
	}

	for (int i=0;i<6;i++)
	{	
		v.x = p_obj->obj_coords[i].x;
		v.y = p_obj->obj_coords[i].y;
		v.z = p_obj->obj_coords[i].z;

		if(IS_FLOAT_NAN(p_obj->obj_coords[i].x))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].y))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].z))
			printf("nan...");
		//vector_dump(v);
		//执行相机变换
		v = p_camera->matrix_projection * v;
		//vector_dump(v);
		if(IS_FLOAT_NAN(v.x))
			printf("nan...");
		if(IS_FLOAT_NAN(v.y))
			printf("nan...");
		if(IS_FLOAT_NAN(v.z))
			printf("nan...");
		p_obj->obj_coords[i].x = v.x/v.z;
		p_obj->obj_coords[i].y = v.y/v.z;
		p_obj->obj_coords[i].z = v.z;
		
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].x))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].y))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].z))
			printf("nan...");
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

		//执行屏幕变换
		v = p_camera->matrix_screen * v;
		//把变换完的坐标拷贝回去
		 p_obj->vertex_data_transformed[i].x = v.x;
		 p_obj->vertex_data_transformed[i].y = v.y;
		 p_obj->vertex_data_transformed[i].z = v.z;
	}

	for (int i=0;i<6;i++)
	{	
		v.x = p_obj->obj_coords[i].x;
		v.y = p_obj->obj_coords[i].y;
		v.z = p_obj->obj_coords[i].z;
		//执行屏幕变换
		v = p_camera->matrix_screen * v;
		if(IS_FLOAT_NAN(v.x))
			printf("nan...");
		if(IS_FLOAT_NAN(v.y))
			printf("nan...");
		if(IS_FLOAT_NAN(v.z))
			printf("nan...");
		p_obj->obj_coords[i].x = v.x/v.z;
		p_obj->obj_coords[i].y = v.y/v.z;
		p_obj->obj_coords[i].z = v.z;

		if(IS_FLOAT_NAN(p_obj->obj_coords[i].x))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].y))
			printf("nan...");
		if(IS_FLOAT_NAN(p_obj->obj_coords[i].z))
			printf("nan...");
	}
}

#endif
