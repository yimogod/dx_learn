#include <scene/Scene.h>

Scene::Scene(){
	meshNum = 0;
}

Scene::~Scene(){}

 Mesh* Scene::GetMesh(int i) {
	 if (i >= meshNum)return nullptr;
	 return meshList[i];
 }

 void Scene::AddMesh(Mesh* mesh){
	 meshList[meshNum] = mesh;
	 meshNum++;
 }