#include "Scene.h"

Scene::Scene(){}

Scene::~Scene(){}

 Mesh* Scene::getMesh(int i) {
	 if (i >= meshNum)return NULL;
	 return meshList[i];
 }

