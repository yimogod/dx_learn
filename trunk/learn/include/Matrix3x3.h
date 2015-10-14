#pragma once
/* �����Ⱦ���, 0123�ǵ�һ�� */
class Matrix3x3{
public:
	float m00, m01, m02;
	float m10, m11, m12;
	float m20, m21, m22;

	Matrix3x3();
	Matrix3x3(float v00, float v01, float v02,
		float v10, float v11, float v12,
		float v20, float v21, float v22);
	~Matrix3x3();

	/*�����ҳ�*/
	Matrix3x3 mul(Matrix3x3 &mb);
	/*ת��*/
	Matrix3x3 transpose();
	/*��¡*/
	Matrix3x3 clone();
	/*����ʽ*/
	float det();
};