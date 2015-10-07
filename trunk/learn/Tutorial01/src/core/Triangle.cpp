#include "../util/CUtil.h"
#include "Triangle.h"

Triangle::Triangle(){
	state = Triangle::STATE_ACTIVE;
	geoType = Triangle::GEO_TYPE_GENERAL;
}

Vector3D Triangle::getNormal(){
    int index0 = vIndex[0];
    int index1 = vIndex[1];
    int index2 = vIndex[2];
    Vertex v0 = vertList[index0];
    Vertex v1 = vertList[index1];
    Vertex v2 = vertList[index2];
    
    Vector3D u(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
    Vector3D v(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

    Vector3D n = u.cross(&v);
    return n;
}

Vertex* Triangle::getVertexByOrder(int order){
	int i = vIndex[order];
	return &vertList[i];
}

void Triangle::clone(Triangle* tri) {
	tri->state = state;
	tri->geoType = geoType;
	tri->orginColor = orginColor;
	tri->vertList = vertList;
	tri->vIndex[0] = vIndex[0];
	tri->vIndex[1] = vIndex[1];
	tri->vIndex[2] = vIndex[2];
}

void Triangle::swapVertexOrder(Vertex* vert0, Vertex* vert1){
	Vertex* temp = vert0;
	vert0 = vert1;
	vert1 = temp;
}

/**
* 绘制方便, 算出三个点的顺序,这里根据管线的算法, Y轴朝下
* 左上原则来排序1,2,3
*/
void Triangle::setGouraudTrangleInfo(){
	geoType = Triangle::GEO_TYPE_GENERAL;
	/* v0为y最小的点,然后逆时针顺序寻找其他点位 */
	Vertex* vert0 = getVertexByOrder(0);
	Vertex* vert1 = getVertexByOrder(1);
	Vertex* vert2 = getVertexByOrder(2);

	/* 退化为直线 */
	if (float_equal(vert0->x, vert1->x) && float_equal(vert0->x, vert2->x)){
		geoType = Triangle::GEO_TYPE_NONE;
		return;
	}
	if (float_equal(vert0->y, vert1->y) && float_equal(vert0->y, vert2->y)){
		geoType = Triangle::GEO_TYPE_NONE;
		return;
	}

	/* 确定vert0 */
	bool swap = false;
	if (vert0->y > vert1->y)swapVertexOrder(vert0, vert1);
	if (vert0->y > vert2->y)swapVertexOrder(vert0, vert2);
	if (vert1->y > vert2->y)swapVertexOrder(vert1, vert2);
	
	/* 平顶三角形, 则选x小的 */
	if (float_equal(vert0->y, vert1->y)){
		geoType = Triangle::GEO_TYPE_TOP;
		if (vert0->x > vert1->x)swapVertexOrder(vert0, vert1);
	}else if (float_equal(vert1->y, vert2->y)){
		geoType = Triangle::GEO_TYPE_BOTTOM;
		if (vert1->x > vert2->x)swapVertexOrder(vert1, vert2);
	}

}
