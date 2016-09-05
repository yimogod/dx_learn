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

struct ConstantBuffer{
	Matrix4x4 model;
	Matrix4x4 view;
	Matrix4x4 perspective;
};