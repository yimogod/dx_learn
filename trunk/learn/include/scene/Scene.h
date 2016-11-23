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

	//获取第i个transform挂着的mesh
	Mesh* GetMesh(int i);
	//获取第i个transform
	Transform* GetTransform(int i);
	void AddTransform(Transform* trans);

	//绘制整个场景中的mesh
	void Draw();
public:
	int renderType;
	
	Camera* camera;
	Transform root;

	//存储所有的tranform--包含子transform
	//即不考虑transform的继承关系, 完全扁平保存
	int transformNum = 0;
	Transform* transformList[100];

	int lightNum = 0;
	Light* lightList[4];
};