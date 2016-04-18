#pragma once

#include <iostream>
#include "Matrix4x4.h"
#include "Plane3D.h"

class Camera{
public:
    Vector3D position;
    /* base on angle radians(弧度), 逆时针, 数据基于世界坐标系 */
    float rotateX;
    float rotateY;
    float rotateZ;
    
	/* rotate y, base on self-coordinate */
	float heading;
	/* rotate x */
	float pitch;
	/* roatet z */
	float bank;

    /* 视距，视点到投影面的距离 */
    float dis = 1.0f;
    
    /* 宽高比 */
    float aspect;
    
    /* 远近裁剪面 */
    float nearClipZ = 1.0f;
    float farClipZ = 1000.0f;
    
    /* 四周裁剪面 */
    /* 正方向朝左 */
    Plane3D rightClipPlane;
    /* 正方向朝右 */
    Plane3D leftClipPlane;
    /* 正方向朝下 */
    Plane3D topClipPlane;
    /* 正方向朝上 */
    Plane3D bottomClipPlane;
    
    /* in degree, field of view, 1-180 */
    float fov = 45.0f;
    
    /* 视口尺寸 */
    float viewportWidth;
    float viewportHeight;
    
public:
    Camera();
    Camera(float px, float py, float pz, float rx, float ry, float rz);
    ~Camera();
    
    void setProperty(float viewDis, float pfov, float nz, float fz,
                     float pviewportWidth, float pviewportHeight);

	Matrix4x4 getWorldToCameraMatrix();
	Matrix4x4 getCameraToProjMatrix();
	Matrix4x4 getWorldToProjMatrix();

	void strafe(float d);
	void walk(float d);
};
