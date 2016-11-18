#pragma once
#include <d3d11_1.h>
#include <graphics/DXDepthState.h>

//RTT, 讲渲染结果存储到一个texture中
class DXRenderTexture{
public:
	DXRenderTexture();
	~DXRenderTexture();

	bool CreateRenderTargetView(ID3D11Device* deivce, int width, int height);
	void ClearRTT(ID3D11DeviceContext* context);
	inline void UseRTT(ID3D11DeviceContext* context);
	inline ID3D11ShaderResourceView* GetRTTResView();

private:
	//用于RTT的render target
	ID3D11Texture2D* _renderTargetBuffer;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11ShaderResourceView* _renderTargetResView;

	DepthState _depthState;
};

inline void DXRenderTexture::UseRTT(ID3D11DeviceContext* context){
	context->OMSetRenderTargets(1, &_renderTargetView, _depthState.GetDepthStencilView());
}

inline ID3D11ShaderResourceView* DXRenderTexture::GetRTTResView(){
	return _renderTargetResView;
}