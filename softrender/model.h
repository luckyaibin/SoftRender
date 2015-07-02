#include "matrix.h"
#include "tool.h"
#include "triangle.h"
#include "vector.h"
#include "comm_headers.h"
#include <fstream>
#define MAX_OBJECT_VERTICES 1024;
#define MAX_OBJECT_TRIANGLES 1024;
struct triangle_type
{
	vertex3d *pv;//ָ�������ζ��������б����ݵ�ָ��
	int vertex_index[3];//�������������
};

struct Object_Type{
	vector3 world_coord;//��������������ϵ������
	vertex3d vertex_data_local[MAX_OBJECT_VERTICES];//���㣬ÿ�������������������������
	vertex3d vertex_data_transformed[MAX_OBJECT_VERTICES];

	int triangle_count;//����������
	triangle_type triangle_list[MAX_OBJECT_TRIANGLES];//�������б�
}Object,*Ojbect_Ptr;



