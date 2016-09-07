#include <sys/Window.h>

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

void Window::Update(){
	if(!isLMouseDown() || !isRMouseDown()){
		_lastMouseX = GetMouseX();
		_lastMouseY = GetMouseY();
	}

	UpdateByKey(0.002f);
	UpdateByLMouse(0.001f);
	UpdateByRMouse(0.001f);
}

void Window::UpdateByLMouse(float value){
	if(!isLMouseDown())return;
	if(_currMesh == nullptr)return;

	int dx = _lastMouseX - GetMouseX();
	int dy = _lastMouseY - GetMouseY();
	if(abs(dx) > abs(dy)){
		_currMesh->rotateY(-2.0f * dx * value);
	}else{
		_currMesh->rotateX(-2.0f * dy * value);
	}
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
}

void Window::UpdateByRMouse(float value){
	if(!isRMouseDown())return;

	int dx = _lastMouseX - GetMouseX();
	int dy = _lastMouseY - GetMouseY();
	_camera.rotateY(-2.0f * dx * value);
	_camera.pitchRotate(-2.0f * dy * value);
	
}

void Window::Destroy(){
	UnloadContent();
}