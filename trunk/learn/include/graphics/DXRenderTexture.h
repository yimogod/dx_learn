#pragma once
#include <d3d11_1.h>
#include <graphics/DepthState.h>

class DXRenderTexture{
public:
	DXRenderTexture();
	~DXRenderTexture();

	bool CreateRenderTargetView(ID3D11Device* deivce, int width, int height);
	void ClearRTT(ID3D11DeviceContext* context);
	inline void UseRTT(ID3D11DeviceContext* context);
	inline ID3D11ShaderResourceView* GetRTTResView();

private:
	//ÓÃÓÚRTTµÄrender target
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