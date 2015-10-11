#include "Camera.h"

Camera::Camera(){
}

Camera::Camera(float px, float py, float pz, float rx, float ry, float rz){
    position.x = px;
    position.y = py;
    position.z = pz;
    
    rotateX = rx;
    rotateY = ry;
    rotateZ = rz;
}

Camera::~Camera(){
}

void Camera::setProperty(float viewDis, float pfov, float nz, float fz,
                         float pviewportWidth, float pviewportHeight){
    dis = viewDis;
    fov = pfov;
    nearClipZ = nz;
    farClipZ = fz;
    viewportWidth = pviewportWidth;
    viewportHeight = pviewportHeight;
    aspect = pviewportWidth / pviewportHeight;
}

Matrix4x4 Camera::getWorldToCameraMatrix(){
	Matrix4x4 move(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-position.x, -position.y, -position.z, 1.0f);
	Matrix4x4 rotate;

	return move.mul(rotate);
}