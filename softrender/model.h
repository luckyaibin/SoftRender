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
	triangle3d *pv;//ָ�������ζ��������б����ݵ�ָ��
	int vertex_index[3];//�������������
};

typedef struct Object_Type{
	vector3 world_coord;//��������������ϵ������
	vertex3d vertex_data_local[MAX_OBJECT_VERTICES];//���㣬ÿ�������������������������
	vertex3d vertex_data_transformed[MAX_OBJECT_VERTICES];
	int vertex_count;//������
	int triangle_count;//����������
	triangle_type triangle_list[MAX_OBJECT_TRIANGLES];//�������б�
}Object,*Ojbect_Ptr;

//����������ת������������
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

//��������任
void ObjectCameraTransform(Ojbect_Ptr obj,Camera_Ptr camera)
{

}

