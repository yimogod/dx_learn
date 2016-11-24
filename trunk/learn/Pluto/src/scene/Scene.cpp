#include <graphics/DXEngine.h>
#include <scene/Scene.h>

Scene::Scene(){
	transformNum = 0;
}

Scene::~Scene(){}

Mesh* Scene::GetMesh(int i) {
	if (i >= transformNum)return nullptr;

	Transform* trans = transformList[i];
	if(trans->dataType & Transform::TT_Mesh){
		return transformList[i]->GetData<Mesh*>();
	}
	return nullptr;
}

Transform* Scene::GetTransform(int i) {
	if(i >= transformNum)return nullptr;
	return transformList[i];
}

void Scene::AddTransform(Transform* trans){
	if(trans->dataType & Transform::TT_Mesh){
		Mesh* mesh = trans->GetData<Mesh*>();
		mesh->InitStatic(*trans);
	}

	transformList[transformNum] = trans;
	transformNum++;
}

void Scene::Draw(){
	ID3D11DeviceContext* context = DXEngine::Instance().GetContext();

	for(int i = 0; i < transformNum; i++){
		Transform* trans = transformList[i];
		if(trans->dataType & Transform::TT_Mesh){
			Mesh* mesh = trans->GetData<Mesh*>();
			mesh->visual.Draw(context);
		}
	}
}