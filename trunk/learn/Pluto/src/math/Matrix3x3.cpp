#include <math/algebra/Matrix3x3.h>

/*构造标准矩阵*/
Matrix3x3::Matrix3x3(){
	m00 = m11 = m22 = 1.0f;
	m01 = m02 = 0;
	m10 = m12 = 0;
	m20 = m21 = 0;
}

Matrix3x3::Matrix3x3(float v00, float v01, float v02,
	float v10, float v11, float v12,
	float v20, float v21, float v22){

	m00 = v00; m01 = v01; m02 = v02;
	m10 = v10; m11 = v11; m12 = v12;
	m20 = v20; m21 = v21; m22 = v22;
}

Matrix3x3::~Matrix3x3(){
}

Matrix3x3 Matrix3x3::mul(Matrix3x3 &mb){
	Matrix3x3 m;
	/*第一行*/
	m.m00 = m00 * mb.m00 + m01 * mb.m10 + m02 * mb.m20;
	m.m01 = m00 * mb.m01 + m01 * mb.m11 + m02 * mb.m21;
	m.m02 = m00 * mb.m02 + m01 * mb.m12 + m02 * mb.m22;

	/*第二行*/
	m.m10 = m10 * mb.m00 + m11 * mb.m10 + m12 * mb.m20;
	m.m11 = m10 * mb.m01 + m11 * mb.m11 + m12 * mb.m21;
	m.m12 = m10 * mb.m02 + m11 * mb.m12 + m12 * mb.m22;

	/*第三行*/
	m.m20 = m20 * mb.m00 + m21 * mb.m10 + m22 * mb.m20;
	m.m21 = m20 * mb.m01 + m21 * mb.m11 + m22 * mb.m21;
	m.m22 = m20 * mb.m02 + m21 * mb.m12 + m22 * mb.m22;

	return m;
}

Matrix3x3 Matrix3x3::transpose(){
	Matrix3x3 m = clone();

	Matrix3x3 n(m.m00, m.m10, m.m20,
		m.m01, m.m11, m.m21,
		m.m02, m.m12, m.m22);

	return n;
}

Matrix3x3 Matrix3x3::clone(){
	Matrix3x3 n(m00, m01, m02,
		m10, m11, m12,
		m20, m21, m22);

	return n;
}

float Matrix3x3::det(){
	return m00*(m11 * m22 - m12*m21) +
		m01 * (m12 * m20 - m10 * m22) +
		m02 * (m10 * m21 - m11 * m20);
}