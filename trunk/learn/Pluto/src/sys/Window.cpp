#include <sys/Window.h>
#include <string>

Window::Window(){}

Window::~Window(){}

bool Window::Init(HINSTANCE const &ins, HWND const &hwnd){
	BaseApp::Init(ins, hwnd);
	_dxEngine.InitDevice(hwnd, _width, _height);

	_camera.setPos(0, 0, -2.0f);
	_camera.setFrustum(1.0f, 45.0f, 1.0f, 100.0f);
	_camera.setAspect(_width, _height);

	return LoadContent();
}

void Window::InitVisual(Mesh* mesh, wchar_t* vsName){
	InitVisual(mesh, nullptr, 0, vsName);
}

void Window::InitVisual(Mesh* mesh, char* vertAddOn, int vertAddOnNum, wchar_t* vsName){
	DXVisual& visual = mesh->visual;

	/*准备shader数据*/
	visual.PreInitShader(vsName, vsName);

	/*创建 layout*/
	if(visual.GetLayoutNum() == 0)
		visual.PreAddDefaultLayout();

	//创建buffer需要的变量
	if(visual.GetConstBufferNum() == 0)
		visual.PreSetVSConstBufferSize(sizeof(MVPConstBuffer));

	//初始化visual
	//多个Vertex数组
	char** vertices = nullptr;
	int* vertexNum = nullptr;
	int vertListNum = 1;
	if(vertAddOnNum == 0){
		vertListNum = 1;
	}else{
		vertListNum = 2;
	}
	 
	vertices = new char*[vertListNum];
	vertexNum = new int[vertListNum];

	Vertex *vert = new Vertex[mesh->vertexNum];
	mesh->GetVertexList(vert);
	vertices[0] = (char*)vert;
	vertexNum[0] = mesh->vertexNum;

	if(vertListNum == 2){
		vertices[1] = vertAddOn;
		vertexNum[1] = vertAddOnNum;
	}


	int* indices = new int[mesh->indexNum];
	mesh->GetIndexList(indices);

	_dxEngine.InitVisual(visual, vertices, vertexNum, indices, mesh->indexNum);

	delete(vertices);
	delete(indices);
}

void Window::InitVisual(Mesh* mesh, wchar_t* vsName, const char* texturePath){
	AddTexture(mesh, texturePath);
	InitVisual(mesh, vsName);
}

void Window::Update(){
	UpdateByKey(0.002f);
	UpdateByLMouse(0.003f);
	UpdateByRMouse(0.003f);
	UpdateConstBuff();

	_lastMouseX = GetMouseX();
	_lastMouseY = GetMouseY();
}

void Window::UpdateConstBuff(){
	MVPConstBuffer cb;
	cb.view = _camera.GetWorldToCameraMatrix().transpose();
	cb.perspective = _camera.GetCameraToProjMatrix().transpose();
	for(int i = 0; i < _scene.meshNum; i++){
		Mesh* mesh = _scene.GetMesh(i);
		cb.model = mesh->localToWorldMatrix().transpose();
		_dxEngine.UpdateVSSubResource(mesh->visual, 0, &cb);
	}
}

void Window::UpdateByLMouse(float value){
	if(!isLMouseDown())return;
	if(_currMesh == nullptr)return;

	int dx = _lastMouseX - GetMouseX();
	int dy = _lastMouseY - GetMouseY();
	if(abs(dx) > abs(dy)){
		_currMesh->rotateY(dx * value);
	}else{
		_currMesh->rotateX(dy * value);
	}
}

void Window::UpdateByRMouse(float value){
	if(!isRMouseDown())return;

	int dx = _lastMouseX - GetMouseX();
	int dy = _lastMouseY - GetMouseY();
	_camera.rotateY(dx * value);
	_camera.pitchRotate(dy * value);
}

void Window::UpdateByKey(float value){
	if(isKeyDown(65)){//A
		_camera.strafe(-value);
	}
	if(isKeyDown(68)){//D
		_camera.strafe(value);
	}
	if(isKeyDown(87)){//W
		_camera.walk(value);
	}
	if(isKeyDown(83)){//S
		_camera.walk(-value);
	}

	if(isKeyDown(37)){//left
		_currMesh->Move(-value, 0, 0);
	}
	if(isKeyDown(39)){//right
		_currMesh->Move(value, 0, 0);
	}
	if(isKeyDown(38)){//front
		_currMesh->Move(0, 0, value);
	}
	if(isKeyDown(40)){//back
		_currMesh->Move(0, 0, -value);
	}
}

void Window::AddTexture(Mesh* mesh, const char* texturePath){
	DXVisual &visual = mesh->visual;
	std::wstring path = GetFullPathW(texturePath);
	visual.PreAddTexture(path);
}

void Window::Destroy(){
	UnloadContent();
}