#pragma once

class Vector3D{
public:
    float x;
    float y;
    float z;
    
    Vector3D();
    Vector3D(float px, float py, float pz);
    
    /* 模长度 */
    float getLength();
    float getLengthSQ();
    
    /* 矢量相加 */
    Vector3D add(const Vector3D &v);
    /* 矢量相减 */
    Vector3D sub(const Vector3D &v);
    /* 矢量于标量相乘 */
    Vector3D mul(float k);

    /* 点乘 */
    float dot(const Vector3D &v);
    /* 叉乘 */
    Vector3D cross(const Vector3D &v);
    /* 归一化 */
    void normalize();
};

float distanceBetweenVector3D(const Vector3D &va, const Vector3D &vb);
