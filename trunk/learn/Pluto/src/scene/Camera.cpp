#include <scene/Camera.h>
#include <math/algebra/Matrix1x4.h>

Camera::Camera(){
	heading = pitch = bank = 0;
}

Camera::~Camera(){
}

void Camera::SetPos(float px, float py, float pz){
	position.x = px;
	position.y = py;
	position.z = pz;
}

void Camera::SetPos(const Vector3D &pos){
	position = pos;
}

void Camera::SetEulerAngle(float pheading, float ppitch, float pbank){
	heading = pheading;
	pitch = ppitch;
	bank = pbank;
}

void Camera::SetFrustum(float viewDis, float pfov, float nz, float fz){
	dis = viewDis;
	fov = pfov;
	nearClipZ = nz;
	farClipZ = fz;
}

void Camera::SetAspect(int pviewportWidth, int pviewportHeight){
	viewportWidth = pviewportWidth;
	viewportHeight = pviewportHeight;
	aspect = (float)pviewportWidth / (float)pviewportHeight;
}

Matrix4x4 Camera::GetWorldToCameraMatrix(){
	Matrix4x4 move(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-position.x, -position.y, -position.z, 1.0f);
	Matrix4x4 m = move.RotateY(heading);
	m = m.RotateX(pitch);
	m = m.rotateZ(bank);

	return m;
}

Matrix4x4 Camera::GetCameraToProjMatrix(){
	Matrix4x4 mat = Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, aspect, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f,-1.0f, 0.0f);

	return mat;
}

Matrix4x4 Camera::GetWorldToProjMatrix(){
	Matrix4x4 a = GetWorldToCameraMatrix();	
	Matrix4x4 b = GetCameraToProjMatrix();
	return a.mul(b);
}

void Camera::Strafe(float d){
	Matrix1x4 worldPos = Matrix1x4(position.x, position.y, position.z, 1.0f);
	Matrix4x4 wvMat = GetWorldToCameraMatrix();
	Matrix1x4 viewPos = worldPos.mul(wvMat);
	viewPos.m00 += d;

	Matrix4x4 vwMat = wvMat.reverse();
	worldPos = viewPos.mul(vwMat);

	position.x = worldPos.m00;
	position.y = worldPos.m01;
	position.z = worldPos.m02;
}

void Camera::Walk(float d){
	Matrix1x4 worldPos = Matrix1x4(position.x, position.y, position.z, 1.0f);
	Matrix4x4 wvMat = GetWorldToCameraMatrix();
	Matrix1x4 viewPos = worldPos.mul(wvMat);
	viewPos.m02 += d;

	Matrix4x4 vwMat = wvMat.reverse();
	worldPos = viewPos.mul(vwMat);

	position.x = worldPos.m00;
	position.y = worldPos.m01;
	position.z = worldPos.m02;
}

void Camera::HeadingRotate(float radian){

}

void Camera::PitchRotate(float radian){
	pitch += radian;
}

void Camera::RotateY(float radian){
	heading += radian;
}