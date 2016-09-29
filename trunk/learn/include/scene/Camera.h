#pragma once

#include <iostream>
#include <math/algebra/Matrix4x4.h>
#include <graphics/Plane3D.h>

//默认方向是(0,0,1)
class Camera{
public:
    Vector3D position;
	
	/* base on angle radians(弧度), 基于世界坐标系逆时针, */
	/*rotate y, 180~-180*/
	float heading;
	/* rotate x, 90~-90 */
	float pitch;
	/* roatet z, 180~-180 */
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
    int viewportWidth;
    int viewportHeight;
    
public:
    Camera();
	//Camera(const Camera &c);//复制构造函数
    ~Camera();
    
	void SetPos(float px, float py, float pz);
	void SetPos(const Vector3D &pos);
	//rotate y, x, z, 基于自身坐标, 单位角度
	void SetEulerAngle(float pheading, float ppitch, float pbank);
	void SetFrustum(float viewDis, float pfov, float nz, float fz);
    void SetAspect(int pviewportWidth, int pviewportHeight);

	Matrix4x4 GetWorldToCameraMatrix();
	Matrix4x4 GetCameraToProjMatrix();
	Matrix4x4 GetWorldToProjMatrix();

	/*镜头在right方向横移, d为增量*/
	void Strafe(float d);
	/*镜头在direction(heading方向)前进or后退, d为增量*/
	void Walk(float d);
	/*镜头绕自身up轴旋转, d为增量弧度*/
	void HeadingRotate(float radian);
	/*镜头绕世界坐标系Y轴旋转, d为增量弧度*/
	void RotateY(float radian);
	/*镜头绕自身right轴旋转, d为增量弧度*/
	void PitchRotate(float radian);
};
