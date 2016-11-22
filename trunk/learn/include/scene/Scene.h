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

	Mesh* GetMesh(int i);
	void AddMesh(Mesh* mesh);

	void Render(ID3D11Device* device, ID3D11DeviceContext* context);
public:
	int renderType;
	
	Camera* camera;
	Transform root;

	//�洢���е�tranform--������transform
	//��������transform�ļ̳й�ϵ, ��ȫ��ƽ����
	int meshNum = 0;
	Transform* meshList[100];

	int lightNum = 0;
	Light* lightList[4];
};