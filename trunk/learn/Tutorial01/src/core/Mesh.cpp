#include "Mesh.h"

Mesh::Mesh(){
	state = Mesh::STATE_ACTIVE;
}

void Mesh::setWorldPos(float x, float y, float z) {
	position = Vertex(x, y, z);
}

void Mesh::setTriangleByVertex(){
	triangleNum = (int)(indexNum / 3);
	/*for (int i = 0; i < triangleNum; ++i){
		triangleList[i].vertList = vertexTransList;
		triangleList[i].vIndex[0] = indexList[i * 3 + 0];
		triangleList[i].vIndex[1] = indexList[i * 3 + 1];
		triangleList[i].vIndex[2] = indexList[i * 3 + 2];

		triangleList[i].orginColor.setColor(255, 0, 0);
	}*/


	/* 计算三角形每个顶点的法线 */
	/*for (int i = 0; i < triangleNum; ++i) {
		Vector3D n = triangleList[i].getNormal();
		triangleList[i].getVertexByOrder(0)->normal.add(&n);
		triangleList[i].getVertexByOrder(1)->normal.add(&n);
		triangleList[i].getVertexByOrder(2)->normal.add(&n);
	}*/

	/* normalize vertext normal */
	for (int i = 0; i < vertexNum; ++i)
		vertexLocalList[i].normal.normalize();
}

void Mesh::copyLocalVertexToTrans(){
    for (int i = 0; i < vertexNum; i++){
        vertexTransList[i].copyFrom(&vertexLocalList[i]);
        vertexTransList[i].copyNormal(&vertexLocalList[i].normal);
    }
}