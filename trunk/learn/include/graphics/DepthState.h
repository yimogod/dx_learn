#pragma once
#include <d3d11_1.h>

class DepthState{
public:
	DepthState();
	~DepthState();

	bool CreateDepthView(ID3D11Device* device, int width, int height);
	bool CreateDepthState(ID3D11Device* device);

	void SetDepthState(ID3D11DeviceContext* context);

	inline ID3D11DepthStencilView* GetDepthStencilView();
private:
	int _width;
	int _height;

	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11DepthStencilState* _depthStencilState;
};

inline ID3D11DepthStencilView* DepthState::GetDepthStencilView(){
	return _depthStencilView;
}