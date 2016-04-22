#include <math.h>
#include <math/algebra/Vector2D.h>

Vector2D::Vector2D(){
    x = 0;
    y = 0;
}

Vector2D::Vector2D(float px, float py){
    x = px;
    y = py;
}

float Vector2D::getLength(){
    return sqrt(x * x + y * y);
}

float Vector2D::getLengthSQ(){
    return x * x + y * y;
}

Vector2D Vector2D::add(const Vector2D& v){
    return Vector2D(x + v.x, y + v.y);
}

Vector2D Vector2D::sub(const Vector2D& v){
    return Vector2D(x - v.x, y - v.y);
}

Vector2D Vector2D::mul(float k){
    return Vector2D(k * x, k * y);
}

float Vector2D::dot(Vector2D* v){
    return x * v->x + y * v->y;
}

Vector2D Vector2D::project(Vector2D* v){
	float k = dot(v);
	k /= v->getLengthSQ();
	return v->mul(k);
}

void Vector2D::normalize(){
    float len = getLength();
    x /= len;
    y /= len;
}

float distanceBetweenVector2D(const Vector2D* va, const Vector2D* vb){
    float dx = va->x - vb->x;
    float dy = va->y - vb->y;

    return sqrt(dx * dx + dy * dy);
}
