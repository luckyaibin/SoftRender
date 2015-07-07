#include "matrix.h"
#include "tool.h"
#include "triangle.h"
#include "vector.h"
#include "comm_headers.h"
#include "camera.h"
#include <fstream>
#define MAX_OBJECT_VERTICES 1024
#define MAX_OBJECT_TRIANGLES 1024
typedef struct Poly_Type
{
	triangle3d_vertex *p_triangle_list;//指向object三角形顶点所在列表数据的指针
	int vertex_index[3];//三个顶点的索引
}Poly,*Poly_Ptr;

typedef struct Object_Type{
	vector3 world_coord;//物体在世界坐标系的坐标
	
	int vertex_count;//顶点数
	vertex3d vertex_data_local[MAX_OBJECT_VERTICES];//顶点，每个顶点坐标是相对于物体坐标
	vertex3d vertex_data_transformed[MAX_OBJECT_VERTICES];
	

	int triangle_count;//三角形数量
	//triangle3d triangles[MAX_OsBJECT_TRIANGLES];//三角形的数据
	Poly_Type triangle_list[MAX_OBJECT_TRIANGLES];//三角形列表数据
}Object,*Ojbect_Ptr;

//把物体坐标转换成世界坐标
void ObjectWorldTransform(Ojbect_Ptr p_ojb)
{
	for (int i=0;i<p_ojb->vertex_count;i++)
	{
		vertex3d v =  p_ojb->vertex_data_local[i];
		v.x += p_ojb->world_coord.x;
		v.y += p_ojb->world_coord.y;
		v.z += p_ojb->world_coord.z;
		p_ojb->vertex_data_transformed[i] = v;
	}
}

//物体相机变换
void ObjectCameraTransform(Ojbect_Ptr p_ojb,Camera_Ptr camera)
{	
	//再旋转
	Matrix3 camera_rotate_matrix = get_matrix_from_x_y_z_axis_angle(-camera->angle_x,-camera->angle_y,-camera->angle_z);
	//先平移
	for (int i=0;i<p_ojb->vertex_count;i++)
	{
		vertex3d v =  p_ojb->vertex_data_local[i];
		v.x -= camera->word_pos.x;
		v.y -= camera->word_pos.y;
		v.z -= camera->word_pos.z;

		vector3 vv(v.x,v.y,v.z);
		vv = camera_rotate_matrix*vv;
		v.x = vv.x;
		v.y = vv.y;
		v.z = vv.z;

		p_ojb->vertex_data_transformed[i] = v;
	}

}

//透视变换，把3D坐标转换成2D坐标
void ObjectProjectTransform(Ojbect_Ptr p_obj)
{
	for (int i=0;i<p_obj->vertex_count;i++)
	{
		p_obj->vertex_data_transformed[i].x /= p_obj->vertex_data_transformed[i].z;
		p_obj->vertex_data_transformed[i].y /= p_obj->vertex_data_transformed[i].z;
	}
}

void ObjectDraw(Ojbect_Ptr p_obj)
{
	for (int i=0;i<p_obj->triangle_count;i++)
	{

	}
	p_obj->triangle_count
}
