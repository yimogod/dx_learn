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
	void readVertex(Mesh& mesh, string& line);
	void readIndex(Mesh& mesh, string& line);
	void readUV(Mesh& mesh, string& line);

private:
	int _uvIndex = 0;//¸¨Öú´æ´¢uvÊý¾Ý
};
