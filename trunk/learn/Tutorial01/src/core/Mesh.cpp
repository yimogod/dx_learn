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
	for(int i = 0; i < vertexNum; i++){
		list[i].pos = Float3{vertexList[i].x, vertexList[i].y, vertexList[i].z};
		list[i].color = Float3{colorList[i].r, colorList[i].g, colorList[i].b};
		list[i].uv = Float2A{uvList[i * 2], uvList[i * 2 + 1]};
	}
}

