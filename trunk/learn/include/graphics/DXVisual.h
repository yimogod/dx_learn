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

	//设置shader的名称
	inline void PreInitShader(wchar_t* vsName, wchar_t* psName);

	//设置layout
	inline void PreAddDefaultLayout();

	//读取图片, 设置resourceview
	inline void PreAddTexture(const std::wstring &path);

	//设置创建buffer需要的各种数据
	void PreSetVSConstBufferSize(int byteWidth);//设置constbuffer对象的byte
	void PreSetPSConstBufferSize(int byteWidth);//设置constbuffer对象的byte

	bool Init(ID3D11Device* device, void* vertices, int vertexNum, int* indices, int indexNum);
	void Draw(ID3D11DeviceContext* context);

public:
	inline void UpdateVSConstBuffer(ID3D11DeviceContext* context, int index, const void* data);
	inline void UpdatePSConstBuffer(ID3D11DeviceContext* context, int index, const void* data);
private:
	InputLayout _layout;

	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	ConstBuffer _vsConstBuffer[4];
	int _vsConstByteWidth[4];
	int _vsConstBufferNum = 0;
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

inline void DXVisual::UpdateVSConstBuffer(ID3D11DeviceContext* context, int index, const void* data){
	_vsConstBuffer[index].UpdateConstBuff(context, data);
}
inline void DXVisual::UpdatePSConstBuffer(ID3D11DeviceContext* context, int index, const void* data){
	_psConstBuffer[index].UpdateConstBuff(context, data);
}

inline void DXVisual::PreAddTexture(const std::wstring &path){
	return _resView.AddTexturePath(path);
}