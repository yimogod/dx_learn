#pragma once
#include "Matrix3x3.h"

/* �����Ⱦ���, 0123�ǵ�һ�� */
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

	/*�����ҳ�*/
	Matrix4x4 mul(Matrix4x4 &mb);
	Matrix4x4 mul(float f);
	/*ת��*/
	Matrix4x4 transpose();

	/*��¡*/
	Matrix4x4 clone();

	/*����ʽ~~ע��ʹ�������ʽ������!*/
	Matrix3x3 minor(int row, int col);
	/*�з��Ŵ�������ʽ*/
	float cMinor(int row, int col);

	/*��׼�������~~Ҳ�ʹ�������ʽ�����ת�þ���*/
	Matrix4x4 adj();

	/*����ʽ, ������ʽ������*/
	float det();

	/*�����~~�������ʽ����(ѡ��;���)��, ��ת�þ�����������*/
	Matrix4x4 reverse();

	/*v�ǻ���, ������������ϵ*/
	Matrix4x4 rotateX(float v);
	Matrix4x4 rotateY(float v);
	Matrix4x4 rotateZ(float v);
};