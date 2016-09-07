#include <util/CUtil.h>
#include <util/ObjParser.h>
#include <scene/Scene.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

ObjParser::ObjParser(){}

void ObjParser::read(const char* name, Scene* pscene){
	_scene = pscene;

	ifstream fs(name);
	if(!fs.is_open()){
		cout << "obj parse file path error!" << endl;
		fs.close();
		return;
	}

	_mesh = new Mesh();

	string line;
	while(getline(fs, line)){
		if(line.size() == 0)continue;
		if(line[0] == 'v'){
			if(line[1] == 't'){//纹理坐标
				readUV(line);
			}else if(line[1] == ' '){//顶点数据
				readVertex(line);
			}
		}else if(line[0] == 'f'){
			readIndex(line);
		}
	}

	fs.close();

	_mesh->calVertexNormal();

	_scene->meshList[_scene->meshNum] = _mesh;
	_scene->meshNum += 1;
}


/* 顶点数组 */
void ObjParser::readVertex(string &line){
	string s1;
	float f2, f3, f4;

	istringstream ss(line);
	ss >> s1 >> f2 >> f3 >> f4;

	_mesh->vertexList[_mesh->vertexNum] = Vector3D(f2, f3, -1.0f * f4);
	_mesh->vertexNum++;
}

/*读取索引*/
void ObjParser::readIndex(string& line){
	string s1, s2, s3, s4;

	istringstream ss(line);
	ss >> s1 >> s2 >> s3 >> s4;
	parseVUNStr(s4);
	parseVUNStr(s3);
	parseVUNStr(s2);
}

void ObjParser::parseVUNStr(string& str){
	int k, a = 0;
	for(k = 0; str[k] != '/'; k++){
		a = a * 10 + char_2_int(str[k]);
	}
	
	_mesh->indexList[_mesh->indexNum] = a - 1;

	a = 0;
	for(k = k + 1; str[k] != '/'; k++){
		a = a * 10 + char_2_int(str[k]);
	}
	_mesh->uvIndexList[_mesh->indexNum] = a - 1;

	//读取法线, 我不需要. 因为自己计算法线
	//a = 0;
	//for(k = k + 1; str[k]; k++)
	//	a = a * 10 + (str[k] - 48);

	_mesh->indexNum++;
}

void ObjParser::readUV(string& line){
	string s1;
	float f2, f3;

	istringstream ss(line);
	ss >> s1 >> f2 >> f3;

	_mesh->uvList[_mesh->uvNum] = Vector2D(f2, 1.0f - f3);
	_mesh->uvNum++;
}