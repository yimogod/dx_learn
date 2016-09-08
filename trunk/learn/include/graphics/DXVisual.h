#pragma once
#include <d3d11_1.h>
#include <graphics/InputLayout.h>
#include <graphics/VertexBuffer.h>
#include <graphics/IndexBuffer.h>
#include <graphics/ConstBuffer.h>
#include <graphics/VertexShader.h>
#include <graphics/PixelShader.h>
#include <graphics/SamplerState.h>

class DXVisual{
public:
	DXVisual();
	~DXVisual();

	//����shader������
	inline void PreInitShader(wchar_t* vsName, wchar_t* psName);

	//����layout
	inline void PreAddDefaultLayout();
	inline void PreAddLayoutPostion();
	inline void PreAddLayoutNormal();
	inline void PreAddLayoutTangent();
	inline void PreAddLayoutColor();
	inline void PreAddLayoutTexcoord();

	//���ô���buffer��Ҫ�ĸ�������
	inline void PreSetConstBufferSize(int byteWidth);//����constbuffer�����byte

	bool Init(ID3D11Device* device, void* vertices, int vertexNum);
	void Draw(ID3D11DeviceContext* context);

public: //��ȡdx obj���
	inline ID3D11Buffer* GetDXConstBuffer() const;

private:
	InputLayout _layout;

	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	ConstBuffer _constBuffer;

	VertexShader _vs;
	PixelShader _ps;

	SamplerState _samplerState;

	int _vertexNum = 0;
	int _constByteWidth = 0;
};

inline void DXVisual::PreInitShader(wchar_t* vsName, wchar_t* psName){
	_vs.SetFileName(vsName);
	_ps.SetFileName(psName);
}

inline void DXVisual::PreAddDefaultLayout(){
	_layout.AddPosition();
	_layout.AddNormal();
	_layout.AddTangent();
	_layout.AddColor();
	_layout.AddTexCoord();
}

inline void DXVisual::PreAddLayoutPostion(){
	_layout.AddPosition();
}

inline void DXVisual::PreAddLayoutNormal(){

}

inline void DXVisual::PreAddLayoutTangent(){

}

inline void DXVisual::PreAddLayoutColor(){
	_layout.AddColor();
}

inline void DXVisual::PreAddLayoutTexcoord(){
	_layout.AddTexCoord();
}

inline void DXVisual::PreSetConstBufferSize(int byteWidth){
	_constByteWidth = byteWidth;
}

inline ID3D11Buffer* DXVisual::GetDXConstBuffer() const{
	return _constBuffer.GetDXObj();
}