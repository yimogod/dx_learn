#pragma once
#include <fbxsdk.h>
#include <scene/Scene.h>

class FBXParser{
public:
	FBXParser();
    
	void read(char* name, Scene* scene);

private:
	Scene* _scene;
	void processNode(FbxNode* node);
	void processMesh(FbxNode* node);

	void readVertex(Mesh* mesh, FbxMesh* fmesh);
	void readIndex(Mesh* mesh, FbxMesh* fmesh);
	void readUV(Mesh* mesh, FbxMesh* fmesh);
};
