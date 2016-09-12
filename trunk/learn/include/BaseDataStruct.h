#pragma once
#include <math/algebra/Matrix4x4.h>

struct Float2{
	float x;
	float y;
};

struct Float2A{
	float u;
	float v;
};

struct Float3{
	float x;
	float y;
	float z;
};

struct Float4{
	float x;
	float y;
	float z;
	float w;
};

struct Float4A{
	float r;
	float g;
	float b;
	float a;
};

struct  Int2{
	int x;
	int y;
};

struct Int2A{
	int u;
	int v;
};

struct Vertex{
	Float4 pos;
	Float4A color;
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
	Matrix4x4 model;
	Matrix4x4 view;
	Matrix4x4 perspective;
};