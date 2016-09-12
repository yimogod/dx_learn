#pragma once
#include <scene/Mesh.h>
#include <scene/Camera.h>
#include <scene/Light.h>

class Scene{
public:
	static const int RENDER_TYPE_FRAME = 0;
	static const int RENDeR_TYPE_FLAT = 1;
	static const int RENDER_TYPE_COLOR = 2;
	static const int RENDER_TYPE_TEXTURE = 3;
	
	Scene();
	~Scene();

	Mesh* GetMesh(int i);
	void AddMesh(Mesh* mesh);
public:
	int renderType;
	
	Camera* camera;

	int meshNum = 0;
	Mesh* meshList[10];

	int lightNum = 0;
	Light* lightList[4];
};