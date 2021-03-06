#include <math.h>
#include <math/algebra/Vector3D.h>

Vector3D::Vector3D(){
    x = 0;
    y = 0;
    z = 0;
}

Vector3D::Vector3D(const Vector3D &value){
	x = value.x;
	y = value.y;
	z = value.z;
}

Vector3D::Vector3D(float px, float py, float pz){
    x = px;
    y = py;
    z = pz;
}

float Vector3D::getLength(){
    return (float)sqrt(x * x + y * y + z * z);
}

float Vector3D::getLengthSQ(){
    return x * x + y * y + z * z;
}

Vector3D Vector3D::add(const Vector3D &v){
    return Vector3D(x + v.x, y + v.y, z + v.z);
}

Vector3D Vector3D::sub(const Vector3D &v){
    return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D Vector3D::mul(float k){
    return Vector3D(k * x, k * y, k * z);
}

Vector3D Vector3D::div(float k){
	if(k > 0 && k < 0.00001f)
		return Vector3D();
	if(k < 0 && k > -0.00001f)
		return Vector3D();

	k = 1 / k;
	return Vector3D(k * x, k * y, k * z);
}

float Vector3D::dot(const Vector3D &v){
    return x * v.x + y * v.y + z * v.z;
}

Vector3D Vector3D::cross(const Vector3D &v){
    float px = y * v.z - v.y * z;
    float py = -x * v.z + v.x * z;
    float pz = x * v.y - v.x * y;
    return Vector3D(px, py, pz);
}

void Vector3D::normalize(){
    float len = getLength();
    x /= len;
    y /= len;
    z /= len;
}

Vector3D& Vector3D::operator=(const Vector3D &v){
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

float distanceBetweenVector3D(const Vector3D &va, const Vector3D &vb){
    float dx = va.x - vb.x;
    float dy = va.y - vb.y;
    float dz = va.z - va.z;

    return (float)sqrt(dx * dx + dy * dy + dz * dz);
}
