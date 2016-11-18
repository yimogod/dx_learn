#include <graphics/DXDepthState.h>

DepthState::DepthState(){
}


DepthState::~DepthState(){
}

bool DepthState::CreateDepthView(ID3D11Device* device, int width, int height){
	_width = width;
	_height = height;

	//声明深度模板<数据>描述
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = _width;
	td.Height = _height;
	td.MipLevels = 1; //深度缓存不需要mip level
	td.ArraySize = 1; //texture array中texture的个数, 如果深度缓存, 1个
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.Usage = D3D11_USAGE_DEFAULT;

	td.SampleDesc.Count = 1;//这边只能设置为1, 否则出错
	td.SampleDesc.Quality = 0;

	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	/*创建深度/模板缓存*/
	HRESULT hr = device->CreateTexture2D(&td, nullptr, &_depthStencilBuffer);
	if(FAILED(hr))return false;

	//深度缓存描述
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(_depthStencilBuffer, &dsvd, &_depthStencilView);
	if(FAILED(hr))return false;
	return true;
}

bool DepthState::CreateDepthState(ID3D11Device* device, bool enable){
	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));
	dsd.DepthEnable = enable;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

	dsd.StencilEnable = false;
	dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace = dsd.FrontFace;

	HRESULT hr = device->CreateDepthStencilState(&dsd, &_depthStencilState);
	if(FAILED(hr))return false;
	return true;
}

void DepthState::SetDepthState(ID3D11DeviceContext* context){
	context->OMSetDepthStencilState(_depthStencilState, 0);
}