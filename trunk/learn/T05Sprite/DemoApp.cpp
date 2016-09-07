#include <graphics/GeoCreater.h>
#include <graphics/Shader.h>
#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp(){}

DemoApp::~DemoApp(){}

bool DemoApp::LoadContent(){
	_currMesh = new Mesh();
	_currMesh->setWorldPos(0, 0, 0.0f);
	GeoCreater::CreateSprite(*_currMesh);

	/*准备顶点缓冲数据*/
	Vertex* vertices = new Vertex[_currMesh->indexNum];
	_currMesh->getVertexList(vertices);

	/*准备shader数据*/
	Shader vs(L"shader/sprite.fx", "VS", "vs_4_0");
	Shader ps(L"shader/sprite.fx", "PS", "ps_4_0");

	/*创建 layout*/
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	int numElements = ARRAYSIZE(layout);
	_dxEngine.CreateShader(vs, ps, layout, numElements);
	_dxEngine.CreateVertexBuffer(vertices, _currMesh->indexNum, 40 * 4);
	_dxEngine.CreateConstBuffer(sizeof(ConstantBuffer));
	_dxEngine.CreateTexture(GetFullPathW("assets/t_01.dds").c_str());

	delete(vertices);
	return true;
}

void DemoApp::UnloadContent(){
}

void DemoApp::Update(){
	Window::Update();

	ConstantBuffer cb;
	cb.model = _currMesh->localToWorldMatrix().transpose();
	cb.view = _camera.getWorldToCameraMatrix().transpose();
	cb.perspective = _camera.getCameraToProjMatrix().transpose();
	_dxEngine.GetContext()->UpdateSubresource(_dxEngine.GetConstBuff(), 0, nullptr, &cb, 0, 0);
}

void DemoApp::Render(){
	if(!_dxEngine.GetReady())return;

	_dxEngine.ClearBuffers();
	_dxEngine.BindVertexBuff();

	_dxEngine.VSSetShader();
	_dxEngine.VSSetConstantBuffers(0, 1);
	_dxEngine.PSSetShader();
	_dxEngine.PSSetShaderResources(0);
	_dxEngine.PSSetSamplers(0, 1);

	_dxEngine.GetContext()->Draw(_currMesh->indexNum, 0);
	_dxEngine.GetChain()->Present(0, 0);
}