#pragma once

#include "Color.h"
#include "Vector3D.h"

class Vertex{
public:
    float x;
    float y;
    float z;
    float w;

	/* 顶点法线 */
	Vector3D normal;

	/* 顶点颜色, 实时值, 被光照后存入这里 */
	Color color;
    
public:
    Vertex();
    Vertex(float px, float py, float pz);
    Vertex(float px, float py, float pz, float pw);

	void setColor(int red, int green, int blue);
    void copyFrom(Vertex* source);
    void copyNormal(Vector3D* source);
    void toString();
};
