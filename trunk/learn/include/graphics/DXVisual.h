#pragma once
#include <string>
#include <d3d11_1.h>
#include <graphics/InputLayout.h>
#include <graphics/VertexBuffer.h>
#include <graphics/IndexBuffer.h>
#include <graphics/ConstBuffer.h>
#include <graphics/VertexShader.h>
#include <graphics/PixelShader.h>
#include <graphics/SamplerState.h>

#include <graphics/ShaderResourceView.h>

class DXVisual{
public:
	DXVisual();
	~DXVisual();

	//��ȡvisual�Ѿ��е�constbuffer����
	inline int GetConstBufferNum();
	//��ȡvisual layoutԪ�صĸ���
	inline int GetLayoutNum();

	//����shader������
	inline void PreInitShader(wchar_t* vsName, wchar_t* psName);

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
	bool Init(ID3D11Device* device, char** vertices, int* vertexNum, int* indices, int indexNum);
	//resView �ⲿ����������ͼ
	void Draw(ID3D11DeviceContext* context, ID3D11ShaderResourceView* resView = nullptr);

public:
	inline void UpdateVSConstBuffer(ID3D11DeviceContext* context, int index, const void* data);
	inline void UpdatePSConstBuffer(ID3D11DeviceContext* context, int index, const void* data);
private:
	InputLayout _layout;


	VertexBuffer _vertexBuffer[4];
	IndexBuffer _indexBuffer;

	//����vs��uniform����
	ConstBuffer _vsConstBuffer[4];
	int _vsConstByteWidth[4];
	int _vsConstBufferNum = 0;

	//����ps��uniform����
	ConstBuffer _psConstBuffer[4];
	int _psConstByteWidth[4];
	int _psConstBufferNum = 0;

	VertexShader _vs;
	PixelShader _ps;

	SamplerState _samplerState;
	ShaderResourceView _resView;
};

inline void DXVisual::PreInitShader(wchar_t* vsName, wchar_t* psName){
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