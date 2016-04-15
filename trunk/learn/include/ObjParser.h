#pragma once
#include <Scene.h>
#include <string>

using namespace std;

class ObjParser{
public:
	ObjParser();
    
	void read(char* name, Scene* scene);

private:
	Scene* _scene;
	/*解析的obj赋予到的mesh*/
	Mesh* _mesh;
	void readVertex(string& line);
	void readIndex(string& line);
	void readUV(string& line);

private:
	/*解析由vertext_index/un_index/normal_index组成的字符串*/
	void parseVUNStr(string& line);
};
