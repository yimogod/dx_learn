#pragma once
#include <math/algebra/Matrix4x4.h>

/* 行优先矩阵, 1行4列 */
class Matrix1x4{
public:
	float m00, m01, m02, m03;

	Matrix1x4();
	Matrix1x4(float v00, float v01, float v02, float v03);
	~Matrix1x4();

	/*矩阵右乘--自己在乘号左边*/
	Matrix1x4 mul(Matrix4x4 &mb);
};