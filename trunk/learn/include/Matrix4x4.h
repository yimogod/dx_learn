#pragma once
#include "Matrix3x3.h"

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

	/*矩阵右乘*/
	Matrix4x4 mul(Matrix4x4 &mb);
	Matrix4x4 mul(float f);
	/*转置*/
	Matrix4x4 transpose();

	/*克隆*/
	Matrix4x4 clone();

	/*余子式~~注意和代数余子式的区别!*/
	Matrix3x3 minor(int row, int col);
	/*有符号代数余子式*/
	float cMinor(int row, int col);

	/*标准伴随矩阵~~也就代数余子式矩阵的转置矩阵*/
	Matrix4x4 adj();

	/*行列式, 用余子式来计算*/
	float det();

	/*逆矩阵~~如果矩阵式正交(选择和镜像)的, 则转置矩阵就是逆矩阵*/
	Matrix4x4 reverse();

	/*v是弧度, 基于世界坐标系*/
	Matrix4x4 rotateX(float v);
	Matrix4x4 rotateY(float v);
	Matrix4x4 rotateZ(float v);
};