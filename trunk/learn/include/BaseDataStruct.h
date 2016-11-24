#pragma once
#include <math/algebra/Matrix4x4.h>

struct Float2{
	float x;
	float y;

	Float2() : x(0.0f), y(0.0f){}
	Float2(float _x, float _y) : x(_x), y(_y){}
	Float2& operator= (const Float2& src) { x = src.x; y = src.y; return *this; }
};

struct Float2A{
	float u;
	float v;

	Float2A() : u(0.0f), v(0.0f){}
	Float2A(float _u, float _v) : u(_u), v(_v){}
	Float2A& operator= (const Float2A& src) { u = src.u; v = src.v; return *this; }
};

struct Float3{
	float x;
	float y;
	float z;

	Float3() : x(0.0f), y(0.0f), z(0.0f){}
	Float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z){}
	Float3& operator= (const Float3& src) { x = src.x; y = src.y; z = src.z;  return *this; }
};

struct Float4{
	float x;
	float y;
	float z;
	float w;

	Float4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f){}
	Float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w){}
	Float4& operator= (const Float4& src) { x = src.x; y = src.y; z = src.z; w = src.w; return *this; }
};

struct Float4A{
	float r;
	float g;
	float b;
	float a;

	Float4A() : r(0.0f), g(0.0f), b(0.0f), a(1.0f){}
	Float4A(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a){}
	Float4A& operator= (const Float4A& src) { r = src.r; g = src.g; b = src.b; a = src.a; return *this; }
};

struct  Int2{
	int x;
	int y;
};

struct Int2A{
	int u;
	int v;
};

//���ﲻ��Ҫ��16byte�ı���
struct Vertex{
	Float4 pos;
	Float2A uv;
	Float4 normal;//���ʹ�÷�����ͼ, ��ônormal�Ͳ����淨����. �������߿ռ�ķ�����, �������ϸ��淨�ߵ�ֵ��һ����
	Float4 tangent;
};

struct Color{
	/* 0.0f ~ 1.0f */
	float r;
	float g;
	float b;
	/* 0.0f ~ 1.0f */
	float a;

	Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f){}
	Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.0f) {}

	Color& operator= (const Color& color) { r = color.r; g = color.g; b = color.b; a = color.a; return *this; }
};

/*�����*/
struct DirectionLight{
	Float4A ambientColor;
	Float4A diffuseColor;
	Float4A specularColor;
	Float4 direction;
};

/*���Դ*/
struct PointLight{
	Float4A ambientColor; //������
	Float4A diffuseColor; //ɢ���
	Float4A specularColor; //�߹�
	Float4 worldPos; //��������
	Float3 attenuate; //˥����
	float range; //Ӱ�췶Χ(��)
};

struct PhongConstBuffer{
	Float4 eyeWorldPos; //�۾�����������
	DirectionLight directionLight; //�����
	PointLight pointLight; //���
};

//����mvpת����buffer
struct MVPConstBuffer{
	Matrix4x4 view;
	Matrix4x4 perspective;
};

struct InstancePos{
	Float3 position;
	float pad;
};