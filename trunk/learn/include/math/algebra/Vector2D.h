#pragma once

class Vector2D{
public:
    float x;
    float y;
    
    Vector2D();
    Vector2D(float px, float py);
    
    /* 模长度 */
    float getLength();
    float getLengthSQ();
    
    /* 矢量相加 */
    Vector2D add(const Vector2D& v);
    /* 矢量相减 */
    Vector2D sub(const Vector2D& v);
    /* 矢量于标量相乘 */
    Vector2D mul(float k);
    /* 点乘 */
    float dot(Vector2D* v);
    /* 归一化 */
    void normalize();
	/*投影*/
	Vector2D project(Vector2D* v);
};

float distanceBetweenVector2D(const Vector2D* va, const Vector2D* vb);
