#include "matrix.h"
#include "tool.h"
#include "triangle.h"
#include "vector.h"
#include "comm_headers.h"
#include "camera.h"
#include <fstream>
#define MAX_OBJECT_VERTICES 1024
#define MAX_OBJECT_TRIANGLES 1024
struct triangle_type
{
	triangle3d *pv;//指向三角形顶点所在列表数据的指针
	int vertex_index[3];//三个顶点的索引
};

typedef struct Object_Type{
	vector3 world_coord;//物体在世界坐标系的坐标
	vertex3d vertex_data_local[MAX_OBJECT_VERTICES];//顶点，每个顶点坐标是相对于物体坐标
	vertex3d vertex_data_transformed[MAX_OBJECT_VERTICES];
	int vertex_count;//定点数
	int triangle_count;//三角形数量
	triangle_type triangle_list[MAX_OBJECT_TRIANGLES];//三角形列表
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
	}
}

//物体相机变换
void ObjectCameraTransform(Ojbect_Ptr obj,Camera_Ptr camera)
{

}

