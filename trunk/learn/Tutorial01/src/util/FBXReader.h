#pragma once

#include <fbxsdk.h>
#include "../core/Scene.h"

class FBXReader{
public:
	FBXReader();
    
	void read(char* name, Scene* scene);

private:
	Scene* _scene;
	void processNode(FbxNode* node);
	void processMesh(FbxNode* node);
};
