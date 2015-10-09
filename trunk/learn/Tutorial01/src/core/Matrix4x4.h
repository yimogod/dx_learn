#pragma once
/* 行优先矩阵, 0123是第一行 */
class Matrix4x4{
public:
	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;

	Matrix4x4();
	Matrix4x4(float v00, float v01, float v02, float v03,
		float v10, float v11, float v12, float v13,
		float v20, float v21, float v22, float v23,
		float v30, float v31, float v32, float v33);
	~Matrix4x4();

	Matrix4x4 mul(Matrix4x4 &mb);
};