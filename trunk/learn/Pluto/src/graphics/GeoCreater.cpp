#include <graphics/GeoCreater.h>

void GeoCreater::createSprite(Mesh* m){
	m->vertexNum = 4;
	m->vertexList[0] = Vector3D(-1.0f, 1.0f, 0.0f);
	m->vertexList[1] = Vector3D(-1.0f, -1.0f, 0.0f);
	m->vertexList[2] = Vector3D(1.0f, -1.0f, 0.0f);
	m->vertexList[3] = Vector3D(1.0f, 1.0f, 0.0f);

	m->uvNum = 4;
	m->uvList[0] = Vector2D(0, 0);
	m->uvList[1] = Vector2D(0, 1.0f);
	m->uvList[2] = Vector2D(1.0f, 1.0f);
	m->uvList[3] = Vector2D(1.0f, 0);

	m->indexNum = 6;
	m->indexList[0] = 1;
	m->indexList[1] = 0;
	m->indexList[2] = 2;
	m->indexList[3] = 2;
	m->indexList[4] = 0;
	m->indexList[5] = 3;

	m->uvIndexList[0] = 1;
	m->uvIndexList[1] = 0;
	m->uvIndexList[2] = 2;
	m->uvIndexList[3] = 2;
	m->uvIndexList[4] = 0;
	m->uvIndexList[5] = 3;
}