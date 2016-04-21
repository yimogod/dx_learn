#pragma once
#include <graphics/Scene.h>
#include <string>

using namespace std;

class ObjParser{
public:
	ObjParser();
    
	void read(const char* name, Scene* scene);

private:
	Scene* _scene;
	/*������obj���赽��mesh*/
	Mesh* _mesh;
	void readVertex(string& line);
	void readIndex(string& line);
	void readUV(string& line);

private:
	/*������vertext_index/un_index/normal_index��ɵ��ַ���*/
	void parseVUNStr(string& line);
};