#pragma once
#include <scene/Scene.h>
#include <string>

class ObjParser{
public:
	ObjParser();
    
	void Read(const char* name, Scene* scene);

private:
	Scene* _scene;
	/*解析的obj赋予到的mesh*/
	Mesh* _mesh;
	
	//vert的坐标
	int _vertexNum = 0;
	Vector3D _vertexList[256];

	//uv坐标, obj单独存储uv坐标, 然后在index中保存uv坐标索引信息
	int _uvNum = 0;
	Vector2D _uvList[128];

	void ReadVertex(std::string& line);
	void ReadIndex(std::string& line);
	void ReadUV(std::string& line);

	/*解析由vertext_index/un_index/normal_index组成的字符串*/
	void ParseVUNStr(std::string& line);
};
