#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dinput.h>
#include <FBXReader.h>

#include <Mesh.h>
#include "DemoApp1.h"

using namespace DirectX;

DemoApp1::DemoApp1(){}

DemoApp1::~DemoApp1(){}

bool DemoApp1::loadContent(){
	const wchar_t* path =
		L"E:/learn/dx_learn/trunk/learn/T01TextureCube/assets/seafloor.dds";


	char* sInputFile = "assets/simple_scene.fbx";
	FBXReader reader;
	reader.read(sInputFile, &_scene);
	_scene.renderType = Scene::RENDER_TYPE_FRAME;

	_scene.camera = new Camera(0, -1.0f, -2.0f, 0, 0, 0);
	_scene.camera->setProperty(1.0f, 45.0f, 1.0f, 100.0f, _width, _height);

	_scene.lightList[0] = new Light();
	_scene.lightList[0]->type = Light::TYPE_AMBIENT;
	_scene.lightList[0]->ambientColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	_scene.lightNum = 1;

	/*准备顶点缓冲数据*/
	Mesh* mesh = _scene.getMesh(0);
	Vertex *vertices = new Vertex[mesh->indexNum];
	mesh->getVertexList(vertices);

	/*准备shader数据*/
	CreateShaderInfo vs;
	vs.fileName = L"shader/Triangle.fx";
	vs.entryPoint = "VS";
	vs.shaderModel = "vs_4_0";
	CreateShaderInfo ps;
	ps.fileName = L"shader/Triangle.fx";
	ps.entryPoint = "PS";
	ps.shaderModel = "ps_4_0";

	createDevice();
	createDXInput();
	createShader(vs, ps);
	createVertexBuffer(vertices, mesh->indexNum);
	//createIndexBuffer(mesh->indexList, mesh->indexNum);
	createConstBuffer();
	createTexture(path);
	
	delete(vertices);

	return true;
}

void DemoApp1::unloadContent(){
	BaseApp::unloadContent();
}

void DemoApp1::update(){
	acquireInput();

	if(isKeyDown(DIK_A)){
		_scene.camera->position.x -= 0.001f;
	}else if(isKeyDown(DIK_D)){
		_scene.camera->position.x += 0.001f;
	}else if(isKeyDown(DIK_W)){
		_scene.camera->position.y += 0.001f;
	}else if(isKeyDown(DIK_S)){
		_scene.camera->position.y -= 0.001f;
	}else if(isKeyDown(DIK_Q)){
		_scene.camera->position.z += 0.001f;
	}else if(isKeyDown(DIK_E)){
		_scene.camera->position.z -= 0.001f;
	}else if(isKeyDown(DIK_J)){
		_scene.camera->rotateY -= 0.001f;
	}else if(isKeyDown(DIK_L)){
		_scene.camera->rotateY += 0.001f;
	}else if(isKeyDown(DIK_I)){
		_scene.camera->rotateX -= 0.001f;
	}else if(isKeyDown(DIK_K)){
		_scene.camera->rotateX += 0.001f;
	}else if(isKeyDown(DIK_U)){
		_scene.camera->rotateZ -= 0.001f;
	}else if(isKeyDown(DIK_O)){
		_scene.camera->rotateZ += 0.001f;
	}
}

void DemoApp1::render(){
	if(_context == NULL)return;
	_context->ClearRenderTargetView(_backBuffTarget, Colors::MidnightBlue);

	/*根据相机重新计算各个矩阵*/
	world_to_camera = _scene.camera->getWorldToCameraMatrix();

	//float aspect = _scene.camera->aspect;
	camera_to_perspective = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 1.0f,
							0.0f, 0.0f, 0.0f, 0.0f);
							
	ConstantBuffer cb;
	cb.view = world_to_camera.transpose();
	cb.perspective = camera_to_perspective.transpose();
	_context->UpdateSubresource(_constBuff, 0, nullptr, &cb, 0, 0);
	

	_context->VSSetShader(_vs, nullptr, 0);
	_context->VSSetConstantBuffers(0, 1, &_constBuff);
	_context->PSSetShader(_ps, nullptr, 0);
	_context->PSSetShaderResources(0, 1, &_resView);
	_context->PSSetSamplers(0, 1, &_sampleState);

	Mesh *m = _scene.getMesh(0);
	_context->Draw(m->indexNum, 0);
	//_context->DrawIndexed(m->indexNum, 0, 0);

	_chain->Present(0, 0);
}