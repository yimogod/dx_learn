#include <fbxsdk.h>
#include <fbxsdk\scene\geometry\fbxlayer.h>
#include <FBXReader.h>
#include <FBXUtil.h>
#include <Scene.h>

#include <iostream>

using namespace std;

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
	
	readVertex(mesh, fmesh);
	readIndex(mesh, fmesh);
	readUV(mesh, fmesh);

	_scene->meshList[_scene->meshNum] = mesh;
	_scene->meshNum += 1;
}

/* 顶点数组 */
void FBXReader::readVertex(Mesh* mesh, FbxMesh* fmesh){
	mesh->vertexNum = fmesh->GetControlPointsCount();
	FbxVector4* ctrlPoint = fmesh->GetControlPoints();
	/* 读取顶点信息 */
	for(int i = 0; i < mesh->vertexNum; ++i){
		Vector3D v = Vector3D(ctrlPoint[i][0], ctrlPoint[i][1], ctrlPoint[i][2]);
		mesh->vertexList[i] = v;
	}
}

/*读取索引*/
void FBXReader::readIndex(Mesh* mesh, FbxMesh* fmesh){
	/* 三角形顶点索引数量, cube = 36 */
	mesh->indexNum = fmesh->GetPolygonVertexCount();
	/* 索引数组指针 */
	//int* index = fmesh->GetPolygonVertices();
	/* 读取顶点索引信息 */
	//for(int i = 0; i < mesh->indexNum; ++i){
	//	mesh->indexList[i] = index[i];
	//}

	 
	/*遍历所有顶点*/
	int triNum = fmesh->GetPolygonCount();
	int temp = 0;
	FbxVector2 uv_value;
	bool unmap, result;
	for(int i = 0; i < triNum; i++){
		for(int j = 0; j < 3; j++){
			//mesh->indexList[temp] = fmesh->getpoi 

			temp++;
		}
	}
}

void FBXReader::readUV(Mesh* mesh, FbxMesh* fmesh){
	//get all UV set names
	FbxStringList lUVSetNameList;
	fmesh->GetUVSetNames(lUVSetNameList);

	//iterating over all uv sets
	for(int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++){
		//get lUVSetIndex-th uv set
		const char* uv_name = lUVSetNameList.GetStringAt(lUVSetIndex);
		const FbxGeometryElementUV* lUVElement = fmesh->GetElementUV(uv_name);

		if(!lUVElement)continue;
		// only support mapping mode eByPolygonVertex
		if(lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex)return;
		

		/*遍历所有顶点*/
		int triNum = fmesh->GetPolygonCount();
		int temp = 0;
		FbxVector2 uv_value;
		bool unmap, result;
		for(int i = 0; i < triNum; i++){
			for(int j = 0; j < 3; j++){
				result = fmesh->GetPolygonVertexUV(i, j, uv_name, uv_value, unmap);
				mesh->uvList[temp * 2] = uv_value[0];
				mesh->uvList[temp * 2 + 1] = uv_value[1];

				temp++;
			}
		}
		
		
		
		/*const int lIndexCount = lUVElement->GetIndexArray().GetCount();
		for(int polyIndex = 0; polyIndex < lIndexCount; polyIndex++){
			int lUVIndex = lUVElement->GetIndexArray().GetAt(polyIndex);
			FbxVector2 lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

			mesh->uvList[polyIndex * 2] = lUVValue[0];
			mesh->uvList[polyIndex * 2 + 1] = lUVValue[1];
		}*/

	}

}