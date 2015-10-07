#include <math.h>
#include "SteadyLine.h"

using namespace std;

SteadyLine::SteadyLine(){

}

SteadyLine::~SteadyLine(){
    
}

void SteadyLine::run(Scene* scene) {
	Camera* camera = scene->camera;

	for (int i = 0; i < scene->meshNum; i++){
		Mesh* mesh = scene->getMesh(i);
		localToWorld(mesh);
		removeBackface(mesh, camera);
		worldToCamera(mesh, camera);
		lightObject(mesh, camera, scene->lightList, scene->lightNum);
		cameraToViewPort(mesh, camera);
		viewPortToScreen(mesh, camera->viewportWidth, camera->viewportHeight);
	}
}

void SteadyLine::localToWorld(Vertex* vertex, Vertex* global){
    vertex->x += global->x;
    vertex->y += global->y;
    vertex->z += global->z;
}

void SteadyLine::localToWorld(Mesh* mesh){
	for (int i = 0; i < mesh->vertexNum; ++i) {
		localToWorld(&mesh->vertexTransList[i], &mesh->position);
    }
}

bool cullObject(Vertex* vertex, Camera* camera, float radius){
    /* 最近点在远截面外 */
    float pz = vertex->z - radius;
    if (pz > camera->farClipZ)return true;
    
    /* 最远点在近截面外 */
    pz = vertex->z + radius;
    if (pz < camera->nearClipZ)return true;
    
    //通过相似三角形计算左右裁剪面
    float zTest = 0.5f * camera->viewportWidth * vertex->z / camera->dis;
    /* 最右端点在左截面外 */
    pz = vertex->x + radius;
    if (pz < -zTest)return true;
    /* 最左端点在右截面外 */
    pz = vertex->x - radius;
    if (pz > zTest)return true;
    
    //通过相似三角形计算左右裁剪面
    zTest = 0.5f * camera->viewportHeight * vertex->z / camera->dis;
    /* 最下端点在上截面外 */
    pz = vertex->y - radius;
    if (pz > zTest)return true;
    /* 最上端点在下截面外 */
    pz = vertex->z + radius;
    if (pz < zTest)return true;
    
    return false;
}

void SteadyLine::removeBackface(Mesh* mesh, Camera* camera){
	for (int i = 0; i < mesh->triangleNum; ++i) {
		Triangle* poly = &mesh->triangleList[i];
		poly->state = Triangle::STATE_ACTIVE;

        /* normal vector */
        Vector3D n = poly->getNormal();
        
		/* view vector */
        int index0 = poly->vIndex[0];
        Vertex v0 = poly->vertList[index0];
        Vector3D view(camera->position.x - v0.x, camera->position.y - v0.y, camera->position.z - v0.z);
        
        float d = view.dot(&n);
        if (d > 0)
            poly->state = Triangle::STATE_BACKFACE;
    }
}

void SteadyLine::lightPoly(Triangle* poly, Camera* camera, Light* lightList[], int lightNum){
	if (poly->state != Triangle::STATE_ACTIVE)return;
    
    int r_sum = 0, g_sum = 0, b_sum = 0;
    /* 光照强度 */
    float li = 0;
    /* 光源到表面的距离 */
    float dist = 0;
    
    for (int i = 0; i < lightNum; ++i) {
		Light* light = lightList[i];
        /* 灯光关闭 */
        if (!light->turnON)continue;
            
        if (light->type == Light::TYPE_AMBIENT){
            r_sum += poly->orginColor.r * light->ambientColor.r / 256.0f;
            g_sum += poly->orginColor.g * light->ambientColor.g / 256.0f;
            b_sum += poly->orginColor.b * light->ambientColor.b / 256.0f;
			

			//三角形顶点赋值颜色
			poly->getVertexByOrder(0)->setColor(r_sum, g_sum, b_sum);
			poly->getVertexByOrder(1)->setColor(r_sum, g_sum, b_sum);
			poly->getVertexByOrder(2)->setColor(r_sum, g_sum, b_sum);

        }else if (light->type == Light::TYPE_INFINITE){
            /* normal vector */
            Vector3D n = poly->getNormal();
                
            float d = -light->dir.dot(&n);
            if (d > 0){
                li = d / n.getLength();
                r_sum += (int)(poly->orginColor.r * light->diffuseColor.r * li) / 256.0f;
				g_sum += (int)(poly->orginColor.g * light->diffuseColor.g * li) / 256.0f;
				b_sum += (int)(poly->orginColor.b * light->diffuseColor.b * li) / 256.0f;
            }


			//三角形顶点赋值颜色
			poly->getVertexByOrder(0)->setColor(r_sum, g_sum, b_sum);
			poly->getVertexByOrder(1)->setColor(r_sum, g_sum, b_sum);
			poly->getVertexByOrder(2)->setColor(r_sum, g_sum, b_sum);
                
        }else if (light->type == Light::TYPE_POINT){
            /* normal vector */
            Vector3D n = poly->getNormal();

            /* light vector */
			for (int ii = 0; ii < 3; ii++){
				//第ii个顶点
				Vertex* v0 = poly->getVertexByOrder(ii);
				//顶点与光源之间的矢量, 由光看向顶点
				Vector3D view(light->pos.x - v0->x, light->pos.y - v0->y, light->pos.z - v0->z);

				//光是否能照耀到
				float d = view.dot(&n);
				if (d > 0){
					dist = view.getLength();
					float attenuate = light->kc + light->kl * dist;
					li = d / (n.getLength() * dist * attenuate);
					r_sum += (int)(poly->orginColor.r * light->diffuseColor.r * li) / 256;
					g_sum += (int)(poly->orginColor.g * light->diffuseColor.g * li) / 256;
					b_sum += (int)(poly->orginColor.b * light->diffuseColor.b * li) / 256;
				}

				v0->setColor(r_sum, g_sum, b_sum);
			}
            
        }
    }
    
}

void SteadyLine::lightObject(Mesh* mesh, Camera* camera, Light* lightList[], int lightNum){
	if (mesh->state != Mesh::STATE_ACTIVE)return;
    
	for (int i = 0; i < mesh->triangleNum; ++i) {
		lightPoly(&mesh->triangleList[i], camera, lightList, lightNum);
    }
}

void SteadyLine::worldToCamera(Vertex* vertex, Camera* camera){
    vertex->x -= camera->position.x;
    vertex->y -= camera->position.y;
    vertex->z -= camera->position.z;
    
    Vertex result;
    result.copyFrom(vertex);
    
    /* cal radius */
    float yr = sqrtf(vertex->x * vertex->x + vertex->z * vertex->z);
    float xr = sqrtf(vertex->y * vertex->y + vertex->z * vertex->z);
    float zr = sqrtf(vertex->x * vertex->x + vertex->y * vertex->y);
    
    //rotate y
    /* we dismiss so little rotation */
    if (yr > 0.0001f && (camera->rotateY > 0.001f || camera->rotateY < -0.001f)) {
        float ya = acosf(result.x / yr) - camera->rotateY;
        vertex->x = yr * cosf(ya);
        vertex->z = yr * sinf(ya);
    }
    
    //rotate x
    if (xr > 0.0001f && (camera->rotateX > 0.001f || camera->rotateX < -0.001f)) {
        float xa = acosf(result.z / xr) - camera->rotateX;
        vertex->z = xr * cosf(xa);
        vertex->y = xr * sinf(xa);
    }
    
    //rotate z
    if (zr > 0.0001f && (camera->rotateZ > 0.001f || camera->rotateZ < -0.001f)) {
        float za = acosf(result.x / zr) - camera->rotateZ;
        vertex->x = zr * cosf(za);
        vertex->y = zr * sinf(za);
    }
}

void SteadyLine::worldToCamera(Mesh* mesh, Camera* camera){
	for (int i = 0; i < mesh->vertexNum; ++i){
		worldToCamera(&mesh->vertexTransList[i], camera);
    }
}

/* IMPORTANT, viewport make vertex value between -1~1 */
void SteadyLine::cameraToViewPort(Vertex* vertex, Camera* camera){
    float fix = camera->dis / vertex->z;
    vertex->x *= fix;
    vertex->y *= fix;
}

void SteadyLine::cameraToViewPort(Mesh* mesh, Camera *camera){
	for (int i = 0; i < mesh->vertexNum; ++i){
		cameraToViewPort(&mesh->vertexTransList[i], camera);
    }
}

/* IMPORTANT, ZERO POINT is top-left, not center of the screen */
void SteadyLine::viewPortToScreen(Vertex* vertex, float screenWidth, float screenHeight){
    float a = screenWidth * 0.5f - 0.5f;
    float b = screenHeight * 0.5f - 0.5f;
    
    vertex->x = a + a * vertex->x;
	vertex->y = a + a * vertex->y;
    //code from book
	//vertex->y = b + b * vertex->y;
    //vertex->y *= screenWidth / screenHeight;

	/* for d2d y-axix, we need modify y value */
	vertex->y -= (a - b);
}

void SteadyLine::viewPortToScreen(Mesh* mesh, float screenWidth, float screenHeight){
	for (int i = 0; i < mesh->vertexNum; ++i){
		viewPortToScreen(&mesh->vertexTransList[i], screenWidth, screenHeight);
    }
}
