#include <graphics/DXEngine.h>
#include <scene/Scene.h>

Scene::Scene(){
	meshNum = 0;
}

Scene::~Scene(){}

 Mesh* Scene::GetMesh(int i) {
	 if (i >= meshNum)return nullptr;
	 return nullptr;
 }

 void Scene::AddMesh(Mesh& mesh){
	 mesh.Init();
	 //meshList[meshNum] = mesh;
	 meshNum++;
 }

 void Scene::Render(){
	 ID3D11DeviceContext* context = DXEngine::Instance().GetContext();

	 for(int i = 0; i < meshNum; i++){
		 Transform* tran = meshList[i];
		 if(tran->dataType & Transform::TT_Mesh)
		 {
			 Mesh* mesh = tran->GetData<Mesh*>();
			 mesh->visual.Draw(context);
		 }
	 }
 }