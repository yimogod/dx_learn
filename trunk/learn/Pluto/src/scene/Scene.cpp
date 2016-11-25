#include <graphics/DXEngine.h>
#include <scene/Scene.h>

Scene::Scene(){
	transformNum = 0;
}

Scene::~Scene(){}

Mesh* Scene::GetMesh(int i) {
	if(i >= transformNum){
		std::cout << "GetMesh -- > i is bigger than max transformNum" << std::endl;
		return nullptr;
	}

	Transform* trans = transformList[i];
	if(trans->dataType & Transform::TT_Mesh){
		return transformList[i]->GetData<Mesh*>();
	}
	return nullptr;
}

Transform* Scene::GetTransform(int i) {
	if(i >= transformNum){
		std::cout << "GetTransform -- > i is bigger than max transformNum" << std::endl;
		return nullptr;
	}
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


std::shared_ptr<Material> Scene::GetMaterial(int i){
	if(i >= materialNum){
		std::cout << "GetMaterial -- > i is bigger than max materialNum" << std::endl;
		return nullptr;
	}
	return materialList[i];
}

void Scene::AddMaterial(std::shared_ptr<Material> material){
	materialList[materialNum] = material;
	materialNum++;
}

void Scene::Update(){
	MVPConstBuffer cb;
	cb.view = camera.GetWorldToCameraMatrix().transpose();
	cb.perspective = camera.GetCameraToProjMatrix().transpose();
	for(int i = 0; i < materialNum; i++){
		std::shared_ptr<Material> mat = GetMaterial(i);
		mat->Set_VS_CBufferData(i, (char *)&cb);
	}

	for(int i = 0; i < transformNum; i++){
		Transform* trans = GetTransform(i);
		Mesh* mesh = trans->GetData<Mesh*>();
		mesh->UpdateCB();
	}
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