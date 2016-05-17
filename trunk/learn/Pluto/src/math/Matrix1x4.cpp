#include <math.h>
#include <math/algebra/Matrix1x4.h>

/*构造标准矩阵*/
Matrix1x4::Matrix1x4(){
	m00 = m01 = m02 = m03 = 1.0f;
}

Matrix1x4::Matrix1x4(float v00, float v01, float v02, float v03){
	m00 = v00; m01 = v01; m02 = v02; m03 = v03;
}

Matrix1x4::~Matrix1x4(){
}

Matrix1x4 Matrix1x4::mul(Matrix4x4 &mb){
	Matrix1x4 m;
	/*第一行*/
	m.m00 = m00 * mb.m00 + m01 * mb.m10 + m02 * mb.m20 + m03 * mb.m30;
	m.m01 = m00 * mb.m01 + m01 * mb.m11 + m02 * mb.m21 + m03 * mb.m31;
	m.m02 = m00 * mb.m02 + m01 * mb.m12 + m02 * mb.m22 + m03 * mb.m32;
	m.m03 = m00 * mb.m03 + m01 * mb.m13 + m02 * mb.m23 + m03 * mb.m33;

	return m;
}
