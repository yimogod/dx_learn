#pragma once

#include <iostream>
#include "Vertex.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include "Scene.h"

class SteadyLine{
public:
    SteadyLine();
    ~SteadyLine();

	void run(Scene* scene);
    
private:
    void localToWorld(Vertex* vertex, Vertex* global);
	void localToWorld(Mesh* mesh);
    bool cullObject(Vertex* vertex, Camera* camera, float radius);
	void removeBackface(Mesh* mesh, Camera* camera);
	//void lightPoly(Triangle* poly, Camera* camera, Light* lightList[], int lightNum);
	void lightObject(Mesh* mesh, Camera* camera, Light* lightList[], int lightNum);
    void worldToCamera(Vertex* vertex, Camera* camera);
	void worldToCamera(Mesh* mesh, Camera* camera);
    void cameraToViewPort(Vertex* vertex, Camera *camera);
	void cameraToViewPort(Mesh* mesh, Camera *camera);
    void viewPortToScreen(Vertex* vertex, float screenWidth, float screenHeight);
	void viewPortToScreen(Mesh* mesh, float screenWidth, float screenHeight);
};
