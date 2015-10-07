#include <fbxsdk.h>
#include "FBXReader.h"
#include "FBXUtil.h"
#include "../core/Scene.h"

FBXReader::FBXReader(){}

void FBXReader::read(char* name, Scene* pscene){
	_scene = pscene;

	FbxManager* lSdkManager = NULL;

	//Scene to load from file
	FbxScene* scene = NULL;

	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager, scene);

	// Load the scene.
	bool lResult = LoadScene(lSdkManager, scene, name);
	if (lResult == false){
		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
	}else {
		//Get the first node in the scene
		FbxNode* rootNode = scene->GetRootNode();
		processNode(rootNode);
	}

	// Destroy all objects created by the FBX SDK.
	DestroySdkObjects(lSdkManager, lResult);
}

void FBXReader::processNode(FbxNode* node){
	FbxNodeAttribute::EType attType;

	if (node->GetNodeAttribute()){
		switch (node->GetNodeAttribute()->GetAttributeType()){
		case FbxNodeAttribute::eMesh:
			processMesh(node);
			break;
		default:
			break;
		}
	}


	for (int i = 0; i < node->GetChildCount(); ++i){
		processNode(node->GetChild(i));
	}
}

void FBXReader::processMesh(FbxNode* node){
	FbxMesh* fmesh = node->GetMesh();
	if (fmesh == NULL)return;

	Mesh* mesh = new Mesh();
	FbxDouble3 pos = node->LclTranslation.Get();
	mesh->setWorldPos(pos[0], pos[1], pos[2]);
	/* 三角形顶点索引数量, cube = 36 */
	mesh->indexNum = fmesh->GetPolygonVertexCount();
	/* 索引数组指针 */
	int* index = fmesh->GetPolygonVertices();
	/* 顶点数组 */
	mesh->vertexNum = fmesh->GetControlPointsCount();
	FbxVector4* ctrlPoint = fmesh->GetControlPoints();

	/* 读取顶点信息 */
	for (int i = 0; i < mesh->vertexNum; ++i){
		Vertex v = Vertex(ctrlPoint[i][0], ctrlPoint[i][1], ctrlPoint[i][2]);
		mesh->vertexLocalList[i] = v;
	}

	/* 读取顶点索引信息 */
	for (int i = 0; i < mesh->indexNum; ++i){
		mesh->indexList[i] = index[i];
	}

	_scene->meshList[_scene->meshNum] = mesh;
	_scene->meshNum += 1;
}