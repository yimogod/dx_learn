#pragma once

#include "Vertex.h"
#include "Vector3D.h"
#include "Color.h"

/* 三个顶点组成的三角形, mesh的基本组成单元 */
class Triangle{
public:
	/* 活动的 */
    static const int STATE_ACTIVE = 1;
    /* 被裁剪 */
    static const int STATE_CLIPPED = 2;
    /* 背面删除 */
    static const int STATE_BACKFACE = 4;
    
	/* 退化为直线 */
	static const int GEO_TYPE_NONE = 0;
	/* 平顶三角形 */
	static const int GEO_TYPE_TOP = 1;
	/* 平底三角形 */
	static const int GEO_TYPE_BOTTOM = 2;
	/* 一般三角形 */
	static const int GEO_TYPE_GENERAL = 3;

	Triangle();
    
    /* 状态, 比如是否被背面剔除 */
    int state;

	/* 形状类型, 平顶, 平底, 还是一般, 或者退化成直线 */
	int geoType;
    
    /* 整个面的原始颜色 */
    Color orginColor;
    
    /* vertex array point, translated, 来自于mesh的translate vertex */
    Vertex* vertList;
    int vIndex[3];
    
    /* 获取当前三角形的法线 */
    Vector3D getNormal();

	/* 通过顺序获取对应的vertex */
	Vertex* getVertexByOrder(int order);

	/* 填充光栅化信息. */
	void setGouraudTrangleInfo();

	void clone(Triangle* tri);

private:
	void swapVertexOrder(Vertex* vert0, Vertex* vert1);
};
