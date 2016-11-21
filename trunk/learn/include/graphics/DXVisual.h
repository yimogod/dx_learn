#pragma once
#include <string>
#include <d3d11_1.h>
#include <graphics/DXInputLayout.h>
#include <graphics/DXVertexBuffer.h>
#include <graphics/DXIndexBuffer.h>
#include <graphics/DXConstBuffer.h>
#include <graphics/DXVertexShader.h>
#include <graphics/DXPixelShader.h>
#include <graphics/DXSamplerState.h>

#include <graphics/DXShaderResourceView.h>

//visual��������Ⱦһ����������Ҫ����������
//�������㲼��, ��������, ��������, shader, �Լ�shader��Ҫ��vs/ps const buffer
//��ͼ�Լ���ͼ�Ĳ���״̬
//�������ݶ����ڱ�ƽ״̬
class DXVisual{
public:
	DXVisual();
	~DXVisual();

	//��ȡvisual�Ѿ��е�constbuffer����
	inline int GetConstBufferNum();
	//��ȡvisual layoutԪ�صĸ���
	inline int GetLayoutNum();

	//����shader������
	inline void PreInitShader(const wchar_t* vsName, const wchar_t* psName);

	//����layout
	inline void PreAddDefaultLayout();
	inline void PreAddDefaultLayoutInstance();

	//��ȡͼƬ, ����resourceview
	inline void PreAddTexture(const std::wstring &path);
	inline void PreAddSampleState(const D3D11_TEXTURE_ADDRESS_MODE adress);

	//���ô���buffer��Ҫ�ĸ�������
	void PreSetVSConstBufferSize(int byteWidth);//����constbuffer�����byte
	void PreSetPSConstBufferSize(int byteWidth);//����constbuffer�����byte

	//vertices�Ǹ���ά����
	//vertexNum�Ǹ�һά����
	bool Init(ID3D11Device* device, char* vertices, int vertexNum, int* indices, int indexNum);
	//resView �ⲿ����������ͼ
	void Draw(ID3D11DeviceContext* context, ID3D11ShaderResourceView* resView = nullptr);

	//��̬�ı�shader, ���漰�����ĺܶණ��
	//������ζ���ؽ�����Visual, layout, vertbuffer, pixelbuffer, constbufferȫ����Ҫ�ؽ�
	void UpdateShader(wchar_t* vsName, wchar_t* psName);
	void Reset();
public:
	inline void UpdateVSConstBuffer(ID3D11DeviceContext* context, int index, const void* data);
	inline void UpdatePSConstBuffer(ID3D11DeviceContext* context, int index, const void* data);
private:
	//���㲼��
	InputLayout _layout;
	
	//�������������, ���ռ��4��slot
	//ÿ��slotֻ��ռ��һ��vertexbuffer. ����16��slot
	VertexBuffer _vertexBuffer[4];
	IndexBuffer _indexBuffer;

	//����vs��uniform����
	//����Լ��һ��shader�����4��vertex const buffer
	ConstBuffer _vsConstBuffer[4];
	int _vsConstByteWidth[4];
	int _vsConstBufferNum = 0;

	//����ps��uniform����
	//����Լ��һ��shader�����4��pixel const buffer
	ConstBuffer _psConstBuffer[4];
	int _psConstByteWidth[4];
	int _psConstBufferNum = 0;

	VertexShader _vs;
	PixelShader _ps;

	//����Ӧ��ÿ��texture��Ӧһ��sampler
	SamplerState _samplerState;
	//visual�õ�����ͼ, ���ж���
	ShaderResourceView _resView;
};

inline void DXVisual::PreInitShader(const wchar_t* vsName, const wchar_t* psName){
	_vs.SetFileName(vsName);
	_ps.SetFileName(psName);
}

inline void DXVisual::PreAddDefaultLayout(){
	_layout.AddPosition();
	_layout.AddColor();
	_layout.AddTexCoord();
	_layout.AddNormal();
	_layout.AddTangent();
}

inline void DXVisual::PreAddDefaultLayoutInstance(){
	_layout.AddInstance();
}

inline void DXVisual::UpdateVSConstBuffer(ID3D11DeviceContext* context, int index, const void* data){
	_vsConstBuffer[index].UpdateConstBuff(context, data);
}
inline void DXVisual::UpdatePSConstBuffer(ID3D11DeviceContext* context, int index, const void* data){
	_psConstBuffer[index].UpdateConstBuff(context, data);
}

inline void DXVisual::PreAddTexture(const std::wstring &path){
	_resView.AddTexturePath(path);
}

inline void DXVisual::PreAddSampleState(const D3D11_TEXTURE_ADDRESS_MODE adress){
	_samplerState.AddAdress(adress);
}

inline int DXVisual::GetConstBufferNum(){
	return _vsConstBufferNum + _psConstBufferNum;
}

inline int DXVisual::GetLayoutNum(){
	return _layout.GetElementNum();
}