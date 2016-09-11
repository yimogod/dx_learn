#pragma once
#include <scene/Scene.h>
#include <string>

class ObjParser{
public:
	ObjParser();
    
	void Read(const char* name, Scene* scene);

private:
	Scene* _scene;
	/*������obj���赽��mesh*/
	Mesh* _mesh;
	
	//vert������
	int _vertexNum = 0;
	Vector3D _vertexList[256];

	//uv����, obj�����洢uv����, Ȼ����index�б���uv����������Ϣ
	int _uvNum = 0;
	Vector2D _uvList[128];

	void ReadVertex(std::string& line);
	void ReadIndex(std::string& line);
	void ReadUV(std::string& line);

	/*������vertext_index/un_index/normal_index��ɵ��ַ���*/
	void ParseVUNStr(std::string& line);
};
