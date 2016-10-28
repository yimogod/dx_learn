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

	//获取visual已经有的constbuffer数量
	inline int GetConstBufferNum();
	//获取visual layout元素的个数
	inline int GetLayoutNum();

	//设置shader的名称
	inline void PreInitShader(wchar_t* vsName, wchar_t* psName);

	//设置layout
	inline void PreAddDefaultLayout();
	inline void PreAddDefaultLayoutInstance();

	//读取图片, 设置resourceview
	inline void PreAddTexture(const std::wstring &path);
	inline void PreAddSampleState(const D3D11_TEXTURE_ADDRESS_MODE adress);

	//设置创建buffer需要的各种数据
	void PreSetVSConstBufferSize(int byteWidth);//设置constbuffer对象的byte
	void PreSetPSConstBufferSize(int byteWidth);//设置constbuffer对象的byte

	//vertices是个二维数组
	//vertexNum是个一维数组
	bool Init(ID3D11Device* device, char** vertices, int* vertexNum, int* indices, int indexNum);
	//resView 外部传进来的贴图
	void Draw(ID3D11DeviceContext* context, ID3D11ShaderResourceView* resView = nullptr);

	//动态改变shader, 这涉及到更改很多东西
	//几乎意味着重建整个Visual, layout, vertbuffer, pixelbuffer, constbuffer全部需要重建
	void UpdateShader(wchar_t* vsName, wchar_t* psName);
	void Reset();
public:
	inline void UpdateVSConstBuffer(ID3D11DeviceContext* context, int index, const void* data);
	inline void UpdatePSConstBuffer(ID3D11DeviceContext* context, int index, const void* data);
private:
	//顶点布局
	InputLayout _layout;
	
	//顶点和索引数据, 最多占用4个slot
	VertexBuffer _vertexBuffer[4];
	IndexBuffer _indexBuffer;

	//用于vs的uniform数据
	//我们约定一个shader中最多4个vertex const buffer
	ConstBuffer _vsConstBuffer[4];
	int _vsConstByteWidth[4];
	int _vsConstBufferNum = 0;

	//用于ps的uniform数据
	//我们约定一个shader中最多4个pixel const buffer
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