#include <util/CUtil.h>
#include <util/ObjParser.h>
#include <scene/Scene.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

ObjParser::ObjParser(){}

Mesh* ObjParser::Read(Mesh* mesh, const char* name){
	ifstream fs(name);
	if(!fs.is_open()){
		cout << "obj parse file path error!" << endl;
		fs.close();
		return nullptr;
	}

	string line;
	while(getline(fs, line)){
		if(line.size() == 0)continue;
		if(line[0] == 'v'){
			if(line[1] == 't'){//纹理坐标
				ReadUV(line);
			}else if(line[1] == ' '){//顶点数据
				ReadVertex(line);
			}
		}else if(line[0] == 'f'){
			ReadIndex(*mesh, line);
		}
	}

	fs.close();

	mesh->CalVertexNormal();
	return mesh;
}


/* 顶点数组 */
void ObjParser::ReadVertex(string &line){
	string s1;
	float f2, f3, f4;

	istringstream ss(line);
	ss >> s1 >> f2 >> f3 >> f4;

	_vertexList[_vertexNum] = Vector3D(f2, f3, -1.0f * f4);
	_vertexNum++;
}

/*读取索引*/
void ObjParser::ReadIndex(Mesh& mesh, string& line){
	string s1, s2, s3, s4;

	istringstream ss(line);
	ss >> s1 >> s2 >> s3 >> s4;
	ParseVUNStr(mesh, s4);
	ParseVUNStr(mesh, s3);
	ParseVUNStr(mesh, s2);
}

void ObjParser::ParseVUNStr(Mesh& mesh, string& str){
	int k, a = 0;
	//read index
	for(k = 0; str[k] != '/'; k++){
		a = a * 10 + char_2_int(str[k]);
	}

	int vertIndex = a - 1;

	//read uv
	a = 0;
	for(k = k + 1; str[k] != '/'; k++){
		a = a * 10 + char_2_int(str[k]);
	}
	int uvIndex = a - 1;

	//read normal
	//a = 0;
	//for(k = k + 1; str[k] != '/'; k++){
	//	a = a * 10 + char_2_int(str[k]);
	//}

	mesh.AddVertexPos(_vertexList[vertIndex]);
	mesh.AddUVPos(_uvList[uvIndex]);
}

void ObjParser::ReadUV(string& line){
	string s1;
	float f2, f3;

	istringstream ss(line);
	ss >> s1 >> f2 >> f3;

	_uvList[_uvNum].x = f2;
	_uvList[_uvNum].y = 1.0f - f3;
	_uvNum++;
}