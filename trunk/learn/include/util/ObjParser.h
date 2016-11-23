#pragma once
#include <scene/Scene.h>
#include <string>

class ObjParser{
public:
	ObjParser();
    
	std::shared_ptr<Mesh> Read(const char* name);

private:
	//vert������
	int _vertexNum = 0;
	Vector3D _vertexList[256];

	//uv����, obj�����洢uv����, Ȼ����index�б���uv����������Ϣ
	int _uvNum = 0;
	Vector2D _uvList[128];

	void ReadVertex(std::string& line);
	void ReadUV(std::string& line);

	void ReadIndex(Mesh& mesh, std::string& line);
	/*������vertext_index/un_index/normal_index��ɵ��ַ���*/
	void ParseVUNStr(Mesh& mesh, std::string& line);
};
