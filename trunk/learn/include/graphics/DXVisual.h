#pragma once
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

	//����shader������
	inline void PreInitShader(wchar_t* vsName, wchar_t* psName);

	//����layout
	inline void PreAddDefaultLayout();

	//��ȡͼƬ, ����resourceview
	inline void PreAddTexture(const wchar_t* path);

	//���ô���buffer��Ҫ�ĸ�������
	inline void PreSetConstBufferSize(int byteWidth);//����constbuffer�����byte

	bool Init(ID3D11Device* device, void* vertices, int vertexNum, int* indices, int indexNum);
	void Draw(ID3D11DeviceContext* context);

public:
	inline void UpdateConstBuffer(ID3D11DeviceContext* context, int index, const void* data);
private:
	InputLayout _layout;

	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	ConstBuffer _constBuffer[4];
	int _constByteWidth[4];
	int _constBufferNum = 0;

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

inline void DXVisual::PreSetConstBufferSize(int byteWidth){
	_constByteWidth[_constBufferNum] = byteWidth;
	_constBufferNum++;
}

inline void DXVisual::PreAddTexture(const wchar_t* path){
	return _resView.AddTexturePath(path);
}

inline void DXVisual::UpdateConstBuffer(ID3D11DeviceContext* context, int index, const void* data){
	_constBuffer[index].UpdateConstBuff(context, data);
}