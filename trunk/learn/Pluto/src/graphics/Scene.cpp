#include <graphics/Scene.h>

Scene::Scene(){
	meshNum = 0;
}

Scene::~Scene(){}

 Mesh* Scene::getMesh(int i) {
	 if (i >= meshNum)return NULL;
	 return meshList[i];
 }

