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
	Matrix4x4 m = move.rotateY(rotateY);
	m = m.rotateZ(rotateZ);
	m = m.rotateX(rotateX);

	return m;
}

Matrix4x4 Camera::getCameraToProjMatrix(){
	Matrix4x4 mat = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, aspect, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	return mat;
}

Matrix4x4 Camera::getWorldToProjMatrix(){
	Matrix4x4 a = getWorldToCameraMatrix();
	Matrix4x4 b = getCameraToProjMatrix();
	return a.mul(b);

}