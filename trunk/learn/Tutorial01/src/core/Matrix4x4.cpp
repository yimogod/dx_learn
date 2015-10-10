#include "Matrix4x4.h"

/*构造标准矩阵*/
Matrix4x4::Matrix4x4(){
	m00 = m11 = m22 = m33 = 1.0f;
	m01 = m02 = m03 = 0;
	m10 = m12 = m13 = 0;
	m20 = m21 = m23 = 0;
	m30 = m31 = m32 = 0;
}

Matrix4x4::Matrix4x4(float v00, float v01, float v02, float v03,
	float v10, float v11, float v12, float v13,
	float v20, float v21, float v22, float v23,
	float v30, float v31, float v32, float v33){

	m00 = v00; m01 = v01; m02 = v02; m03 = v03;
	m10 = v10; m11 = v11; m12 = v12; m13 = v13;
	m20 = v20; m21 = v21; m22 = v22; m23 = v23;
	m30 = v30; m31 = v31; m32 = v32; m33 = v33;
}

Matrix4x4::~Matrix4x4(){
}

Matrix4x4 Matrix4x4::mul(Matrix4x4 &mb){
	Matrix4x4 m;
	/*第一行*/
	m.m00 = m00 * mb.m00 + m01 * mb.m10 + m02 * mb.m20 + m03 * mb.m30;
	m.m01 = m00 * mb.m01 + m01 * mb.m11 + m02 * mb.m21 + m03 * mb.m31;
	m.m02 = m00 * mb.m02 + m01 * mb.m12 + m02 * mb.m22 + m03 * mb.m32;
	m.m03 = m00 * mb.m03 + m01 * mb.m13 + m02 * mb.m23 + m03 * mb.m33;

	/*第二行*/
	m.m10 = m10 * mb.m00 + m11 * mb.m10 + m12 * mb.m20 + m13 * mb.m30;
	m.m11 = m10 * mb.m01 + m11 * mb.m11 + m12 * mb.m21 + m13 * mb.m31;
	m.m12 = m10 * mb.m02 + m11 * mb.m12 + m12 * mb.m22 + m13 * mb.m32;
	m.m13 = m10 * mb.m03 + m11 * mb.m13 + m12 * mb.m23 + m13 * mb.m33;

	/*第三行*/
	m.m20 = m20 * mb.m00 + m21 * mb.m10 + m22 * mb.m20 + m23 * mb.m30;
	m.m21 = m20 * mb.m01 + m21 * mb.m11 + m22 * mb.m21 + m23 * mb.m31;
	m.m22 = m20 * mb.m02 + m21 * mb.m12 + m22 * mb.m22 + m23 * mb.m32;
	m.m23 = m20 * mb.m03 + m21 * mb.m13 + m22 * mb.m23 + m23 * mb.m33;

	/*第四行*/
	m.m30 = m30 * mb.m00 + m31 * mb.m10 + m32 * mb.m20 + m33 * mb.m30;
	m.m31 = m30 * mb.m01 + m31 * mb.m11 + m32 * mb.m21 + m33 * mb.m31;
	m.m32 = m30 * mb.m02 + m31 * mb.m12 + m32 * mb.m22 + m33 * mb.m32;
	m.m33 = m30 * mb.m03 + m31 * mb.m13 + m32 * mb.m23 + m33 * mb.m33;

	return m;
}

Matrix4x4 Matrix4x4::transpose(){
	Matrix4x4 m = clone();

	Matrix4x4 n(m.m00, m.m10, m.m20, m.m30,
		m.m01, m.m11, m.m21, m.m31, 
		m.m02, m.m12, m.m22, m.m32, 
		m.m03, m.m13, m.m23, m.m33);

	return n;
}

Matrix4x4 Matrix4x4::clone(){
	Matrix4x4 n(m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33);

	return n;
}