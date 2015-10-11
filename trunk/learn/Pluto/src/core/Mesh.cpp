#include "Mesh.h"
#include "Vector3D.h"
#include "Vertex.h"

Mesh::Mesh(){
	state = Mesh::STATE_ACTIVE;
}

Mesh::~Mesh(){}

void Mesh::setWorldPos(float x, float y, float z){
	position = Vector3D(x, y, z);
}

void Mesh::getVertexList(Vertex list[]){
	for(int i = 0; i < indexNum; i++){
		int index = indexList[i];
		list[i].pos = Float4{vertexList[index].x, vertexList[index].y, vertexList[index].z, 1.0f};
		list[i].color = Float4A{colorList[index].r, colorList[index].g, colorList[index].b, 1.0f};
		list[i].uv = Float2A{uvList[index* 2], uvList[index* 2 + 1]};
	}
}

