#pragma once
#include <scene/Mesh.h>
#include <scene/Camera.h>
#include <scene/Transform.h>
#include <scene/Light.h>

class Scene{
public:
	static const int RENDER_TYPE_FRAME = 0;
	static const int RENDER_TYPE_FLAT = 1;
	static const int RENDER_TYPE_COLOR = 2;
	static const int RENDER_TYPE_TEXTURE = 3;
	
	Scene();
	~Scene();

	//��ȡ��i��transform���ŵ�mesh
	Mesh* GetMesh(int i);
	//��ȡ��i��transform
	Transform* GetTransform(int i);
	void AddTransform(Transform* trans);

	std::shared_ptr<Material> GetMaterial(int i);
	void AddMaterial(std::shared_ptr<Material> material);

	//���ݳ��������ݵĸı�������dx�еĸ���buffer
	void Update();
	//�������������е�mesh
	void Draw();
public:
	int renderType;
	
	Camera camera;
	Transform root;

	//�洢���е�tranform--������transform
	//��������transform�ļ̳й�ϵ, ��ȫ��ƽ����
	int transformNum = 0;
	Transform* transformList[100];

	//�����õ��Ĳ�����
	int materialNum = 0;
	std::shared_ptr<Material> materialList[20];

	int lightNum = 0;
	Light* lightList[4];
};