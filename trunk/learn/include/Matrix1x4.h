#pragma once
#include <Matrix4x4.h>

/* �����Ⱦ���, 1��4�� */
class Matrix1x4{
public:
	float m00, m01, m02, m03;

	Matrix1x4();
	Matrix1x4(float v00, float v01, float v02, float v03);
	~Matrix1x4();

	/*�����ҳ�--�Լ��ڳ˺����*/
	Matrix1x4 mul(Matrix4x4 &mb);
};