#pragma once
#include <BaseDataStruct.h>

struct Vertex{
	Float4 pos;
	Float4A color;
	Float2A uv;
	Float4 normal;//���ʹ�÷�����ͼ, ��ônormal�Ͳ����淨����. �������߿ռ�ķ�����, �������ϸ��淨�ߵ�ֵ��һ����
	Float4 tangent;
};