#pragma once
#include <d3d11_1.h>

class DXRenderTexture{
public:
	DXRenderTexture();
	~DXRenderTexture();

	bool CreateRenderTargetView(ID3D11Device* deivce, int width, int height);
	void ClearRTT(ID3D11DeviceContext* context, ID3D11DepthStencilView* depthStencilView);
	inline void UseRTT(ID3D11DeviceContext* context, ID3D11DepthStencilView* depthStencilView);
private:
	//ÓÃÓÚRTTµÄrender target
	ID3D11Texture2D* _renderTargetBuffer;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11ShaderResourceView* _renderTargetResView;
};

inline void DXRenderTexture::UseRTT(ID3D11DeviceContext* context, ID3D11DepthStencilView* depthStencilView){
	context->OMSetRenderTargets(1, &_renderTargetView, depthStencilView);
}