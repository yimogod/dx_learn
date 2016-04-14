#include <ObjParser.h>
#include <Scene.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

ObjParser::ObjParser(){}

void ObjParser::read(char* name, Scene* pscene){
	_scene = pscene;

	ifstream fs(name);
	if(!fs.is_open()){
		cout << "obj parse file path error!" << endl;
		fs.close();
		return;
	}

	Mesh* mesh = new Mesh();

	string line;
	while(getline(fs, line)){
		if(line.size() == 0)continue;
		if(line[0] == 'v'){
			if(line[1] == 't'){//纹理坐标
				readUV(*mesh, line);
			}else if(line[1] == ' '){//顶点数据
				readVertex(*mesh, line);
			}
		}else if(line[0] == 'f'){
			readIndex(*mesh, line);
		}
	}

	fs.close();

	mesh->calVertexNormal();

	_scene->meshList[_scene->meshNum] = mesh;
	_scene->meshNum += 1;
}


/* 顶点数组 */
void ObjParser::readVertex(Mesh& mesh, string &line){
	string s1;
	float f2, f3, f4;

	istringstream ss(line);
	ss >> s1 >> f2 >> f3 >> f4;

	Vector3D v(f2, f4, f3);
	mesh.vertexList[mesh.vertexNum] = v;
	mesh.vertexNum++;
}

/*读取索引*/
void ObjParser::readIndex(Mesh& mesh, string& line){
	string s1, s2, s3, s4;

	istringstream ss(line);
	ss >> s1 >> s2 >> s3 >> s4;

	mesh.indexList[mesh.indexNum] = (int)s1;
	mesh.indexNum++;
}

void ObjParser::readUV(Mesh& mesh, string& line){
	string s1;
	float f2, f3;

	istringstream ss(line);
	ss >> s1 >> f2 >> f3;

	mesh.uvList[_uvIndex * 2] = f2;
	mesh.uvList[_uvIndex * 2 + 1] = f3;
	_uvIndex++;
}