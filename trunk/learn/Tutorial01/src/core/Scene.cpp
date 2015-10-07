#include "Scene.h"

Scene::Scene(){}

Scene::~Scene(){}

 Mesh* Scene::getMesh(int i) {
	 if (i >= meshNum)return NULL;
	 return meshList[i];
 }

void Scene::buildMesh() {
	for (int i = 0; i < meshNum; i++){
		meshList[i]->setTriangleByVertex();
		meshList[i]->copyLocalVertexToTrans();
	}
}

void Scene::resetMesh() {
	for (int i = 0; i < meshNum; i++) {
		meshList[i]->copyLocalVertexToTrans();
	}
}