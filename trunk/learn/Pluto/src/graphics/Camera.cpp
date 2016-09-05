#include <graphics/Camera.h>
#include <math/algebra/Matrix1x4.h>

Camera::Camera(){
	position = Vector3D();
	heading = pitch = bank = 0;
}

Camera::~Camera(){
}

void Camera::setPos(float px, float py, float pz){
	position.x = px;
	position.y = py;
	position.z = pz;
}

void Camera::setEulerAngle(float pheading, float ppitch, float pbank){
	heading = heading;
	pitch = ppitch;
	bank = pbank;
}

void Camera::setFrustum(float viewDis, float pfov, float nz, float fz){
	dis = viewDis;
	fov = pfov;
	nearClipZ = nz;
	farClipZ = fz;
}

void Camera::setAspect(int pviewportWidth, int pviewportHeight){
	viewportWidth = pviewportWidth;
	viewportHeight = pviewportHeight;
	aspect = (float)pviewportWidth / (float)pviewportHeight;
}

Matrix4x4 Camera::getWorldToCameraMatrix(){
	Matrix4x4 move(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-position.x, -position.y, -position.z, 1.0f);
	Matrix4x4 m = move.rotateY(heading);
	m = m.rotateX(pitch);
	m = m.rotateZ(bank);

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

void Camera::strafe(float d){
	Matrix1x4 worldPos = Matrix1x4(position.x, position.y, position.z, 1.0f);
	Matrix4x4 wvMat = getWorldToCameraMatrix();
	Matrix1x4 viewPos = worldPos.mul(wvMat);
	viewPos.m00 += d;

	Matrix4x4 vwMat = wvMat.reverse();
	worldPos = viewPos.mul(vwMat);

	position.x = worldPos.m00;
	position.y = worldPos.m01;
	position.z = worldPos.m02;
}

void Camera::walk(float d){
	Matrix1x4 worldPos = Matrix1x4(position.x, position.y, position.z, 1.0f);
	Matrix4x4 wvMat = getWorldToCameraMatrix();
	Matrix1x4 viewPos = worldPos.mul(wvMat);
	viewPos.m02 += d;

	Matrix4x4 vwMat = wvMat.reverse();
	worldPos = viewPos.mul(vwMat);

	position.x = worldPos.m00;
	position.y = worldPos.m01;
	position.z = worldPos.m02;
}

void Camera::headingRotate(float radian){

}

void Camera::pitchRotate(float radian){
	pitch += radian;
}

void Camera::rotateY(float radian){
	heading += radian;
}