#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include <FBXReader.h>

#include <Mesh.h>
#include "T03ADSSphere.h"

using namespace DirectX;

T03ADSSphere::T03ADSSphere(){}

T03ADSSphere::~T03ADSSphere(){}

bool T03ADSSphere::loadContent(){
	const wchar_t* path =
		L"E:/learn/dx_learn/trunk/learn/T03ADSSphere/assets/seafloor.dds";


	char* sInputFile = "assets/simple_scene.fbx";
	FBXReader reader;
	reader.read(sInputFile, &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;

	_scene.camera = new Camera(0, -1.0f, -2.0f, 0, 0, 0);
	_scene.camera->setProperty(1.0f, 45.0f, 1.0f, 100.0f, _width, _height);


	_scene.lightList[0] = new Light();
	_scene.lightList[0]->type = Light::TYPE_DIRECTION;
	_scene.lightList[0]->ambientColor = Color{ 0.8f, 0.8f, 0.0f, 1.0f };
	_scene.lightList[0]->diffuseColor = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[0]->specularColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	_scene.lightList[0]->dir = Vector3D(1.0f, 0.0f, 0.0f);
	
	_scene.lightList[1] = new Light();
	_scene.lightList[1]->type = Light::TYPE_POINT;
	_scene.lightList[1]->ambientColor = Color{ 0.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[1]->diffuseColor = Color{ 0.0f, 0.0f, 1.0f, 1.0f };
	_scene.lightList[1]->specularColor = Color{ 0.0f, 0.0f, 0.0f, 1.0f };
	_scene.lightList[1]->pos = Vector3D(0.8f, 0.0f, 0.0f);
	_scene.lightList[1]->range = 200.0f;
	_scene.lightList[1]->kc = 0.0f;
	_scene.lightList[1]->kl = 1.0f;
	_scene.lightList[1]->kq = 0.0f;

	_scene.lightNum = 2;

	/*准备顶点缓冲数据*/
	Mesh* mesh = _scene.getMesh(0);
	Vertex *vertices = new Vertex[mesh->indexNum];
	mesh->getVertexList(vertices);

	/*准备shader数据*/
	CreateShaderInfo vs;
	vs.fileName = L"shader/Phong.fx";
	vs.entryPoint = "VS";
	vs.shaderModel = "vs_4_0";
	CreateShaderInfo ps;
	ps.fileName = L"shader/Phong.fx";
	ps.entryPoint = "PS";
	ps.shaderModel = "ps_4_0";

	/*创建 layout*/
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int numElements = ARRAYSIZE(layout);

	createDevice();
	createDXInput();
	createShader(vs, ps, layout, numElements);
	createVertexBuffer(vertices, mesh->indexNum);
	createConstBuffer(&_constBuff, sizeof(ConstantBuffer));
	createConstBuffer(&_phongBuff, sizeof(PhongBuffer));
	createTexture(path);
	
	delete(vertices);

	camera_to_perspective = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	return true;
}

void T03ADSSphere::unloadContent(){
	BaseApp::unloadContent();
}

void T03ADSSphere::update(){
	UpdatePosByKeyboard(_scene.camera, 0.001f);

	/*根据相机重新计算各个矩阵*/
	world_to_camera = _scene.camera->getWorldToCameraMatrix();

	ConstantBuffer cb;
	cb.view = world_to_camera.transpose();
	cb.perspective = camera_to_perspective.transpose();
	_context->UpdateSubresource(_constBuff, 0, nullptr, &cb, 0, 0);

	Light* light = _scene.lightList[0];
	Color ac = light->ambientColor;
	Color dc = light->diffuseColor;
	Color sc = light->specularColor;
	Vector3D d = light->dir;

	PhongBuffer pb;
	pb.eyeWorldPos = Float4{ 0.0f, 0.0f, 0.0f, 1.0f };

	DirectionLight dl = DirectionLight{
		Float4A{ ac.r, ac.g, ac.b, ac.a },
		Float4A{ dc.r, dc.g, dc.b, dc.a },
		Float4A{ sc.r, sc.g, sc.b, sc.a },
		Float4{ d.x, d.y, d.z, 1.0f } };
	pb.directionLight = dl;

	light = _scene.lightList[1];
	ac = light->ambientColor;
	dc = light->diffuseColor;
	sc = light->specularColor;
	d = light->pos;
	PointLight pl = PointLight{
		Float4A{ ac.r, ac.g, ac.b, ac.a },
		Float4A{ dc.r, dc.g, dc.b, dc.a },
		Float4A{ sc.r, sc.g, sc.b, sc.a },
		Float4{ d.x, d.y, d.z, 1.0f },
		light->range,
		Float3{ light->kc, light->kl, light->kq } };
	pb.pointLight = pl;

	_context->UpdateSubresource(_phongBuff, 0, nullptr, &pb, 0, 0);
}

void T03ADSSphere::render(){
	if(_context == NULL)return;
	_context->ClearRenderTargetView(_backBuffTarget, Colors::MidnightBlue);
	

	_context->VSSetShader(_vs, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constBuff);
	_context->PSSetShader(_ps, nullptr, 0);
	_context->PSSetConstantBuffers(1, 1, &_phongBuff);
	_context->PSSetShaderResources(0, 1, &_resView);
	_context->PSSetSamplers(0, 1, &_sampleState);

	Mesh *m = _scene.getMesh(0);
	_context->Draw(m->indexNum, 0);

	_chain->Present(0, 0);
}