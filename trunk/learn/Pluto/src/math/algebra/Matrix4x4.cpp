#include <math.h>
#include <math/algebra/Matrix4x4.h>
#include <math/algebra/Matrix3x3.h>

/*构造标准矩阵*/
Matrix4x4::Matrix4x4(){
	m00 = m11 = m22 = m33 = 1.0f;
	m01 = m02 = m03 = 0.0f;
	m10 = m12 = m13 = 0.0f;
	m20 = m21 = m23 = 0.0f;
	m30 = m31 = m32 = 0.0f;
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

Matrix4x4 Matrix4x4::mul(float f){
	Matrix4x4 n(f * m00, f * m01, f * m02, f * m03,
		f * m10, f * m11, f * m12, f * m13,
		f * m20, f * m21, f * m22, f * m23,
		f * m30, f * m31, f * m32, f * m33);

	return n;
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

Matrix3x3 Matrix4x4::minor(int row, int col){
	if(row == 0 && col == 0){
		return Matrix3x3(m11, m12, m13,
			m21, m22, m23,
			m31, m32, m33);
	}

	if(row == 0 && col == 1){
		return Matrix3x3(m10, m12, m13,
			m20, m22, m23,
			m30, m32, m33);
	}

	if(row == 0 && col == 2){
		return Matrix3x3(m10, m11, m13,
			m20, m21, m23,
			m30, m31, m33);
	}

	if(row == 0 && col == 3){
		return Matrix3x3(m10, m11, m12,
			m20, m21, m22,
			m30, m31, m32);
	}

	//-----------
	if(row == 1 && col == 0){
		return Matrix3x3(m01, m02, m03,
			m21, m22, m23,
			m31, m32, m33);
	}

	if(row == 1 && col == 1){
		return Matrix3x3(m00, m02, m03,
			m20, m22, m23,
			m30, m32, m33);
	}

	if(row == 1 && col == 2){
		return Matrix3x3(m00, m01, m03,
			m20, m21, m23,
			m30, m31, m33);
	}

	if(row == 1 && col == 3){
		return Matrix3x3(m00, m01, m02,
			m20, m21, m22,
			m30, m31, m32);
	}

	//-------------------------
	if(row == 2 && col == 0){
		return Matrix3x3(m01, m02, m03,
			m11, m12, m13,
			m31, m32, m33);
	}

	if(row == 2 && col == 1){
		return Matrix3x3(m00, m02, m03,
			m10, m12, m13,
			m30, m32, m33);
	}

	if(row == 2 && col == 2){
		return Matrix3x3(m00, m01, m03,
			m10, m11, m13,
			m30, m31, m33);
	}

	if(row == 2 && col == 3){
		return Matrix3x3(m00, m01, m02,
			m10, m11, m12,
			m30, m31, m32);
	}

	//----------------------------
	if(row == 3 && col == 0){
		return Matrix3x3(m01, m02, m03,
			m11, m12, m13,
			m21, m22, m23);
	}

	if(row == 3 && col == 1){
		return Matrix3x3(m00, m02, m03,
			m10, m12, m13,
			m20, m22, m23);
	}

	if(row == 3 && col == 2){
		return Matrix3x3(m00, m01, m03,
			m10, m11, m13,
			m20, m21, m23);
	}

	if(row == 3 && col == 3){
		return Matrix3x3(m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22);
	}
}

float Matrix4x4::cMinor(int row, int col){
	int tag = row + col;
	if(tag % 2 == 0)tag = 1;
	else tag = -1;

	Matrix3x3 m33 = minor(row, col);
	float d = m33.det();
	return tag * d;
}

Matrix4x4 Matrix4x4::adj(){
	float m00 = cMinor(0, 0);
	float m01 = cMinor(0, 1);
	float m02 = cMinor(0, 2);
	float m03 = cMinor(0, 3);

	float m10 = cMinor(1, 0);
	float m11 = cMinor(1, 1);
	float m12 = cMinor(1, 2);
	float m13 = cMinor(1, 3);

	float m20 = cMinor(2, 0);
	float m21 = cMinor(2, 1);
	float m22 = cMinor(2, 2);
	float m23 = cMinor(2, 3);

	float m30 = cMinor(3, 0);
	float m31 = cMinor(3, 1);
	float m32 = cMinor(3, 2);
	float m33 = cMinor(3, 3);

	Matrix4x4 m(m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33);

	return m.transpose();
}

float Matrix4x4::det(){
	float d = m00 * minor(0, 0).det();
	d -= m01 * minor(0, 1).det();
	d += m02 * minor(0, 2).det();
	d -= m03 * minor(0, 3).det();

	return d;
}

Matrix4x4 Matrix4x4::reverse(){
	Matrix4x4 m = adj();
	float n = 1.0f / det();
	return m.mul(n);
}

Matrix4x4 Matrix4x4::rotateX(float v){
	float c = cosf(v);
	float s = sinf(v);
	Matrix4x4 m(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, c, s, 0.0f,
		0.0f, -s, c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return mul(m);
}

Matrix4x4 Matrix4x4::rotateY(float v){
	float c = cosf(v);
	float s = sinf(v);
	Matrix4x4 m(c, 0.0f, -s, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		s, 0.0f, c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return mul(m);
}

Matrix4x4 Matrix4x4::rotateZ(float v){
	float c = cosf(v);
	float s = sinf(v);
	Matrix4x4 m(c, s, 0.0f, 0.0f,
		-s, c, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return mul(m);
}