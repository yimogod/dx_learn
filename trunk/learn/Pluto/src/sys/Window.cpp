#include <sys/Window.h>

Window::Window(){}

Window::~Window(){}

void Window::Update(){
	if(!isLMouseDown() || !isRMouseDown()){
		_lastMouseX = GetMouseX();
		_lastMouseY = GetMouseY();
	}

	UpdateByKey(0.1f);
	UpdateByLMouse(0.1f);
	UpdateByRMouse(0.1f);
}

void Window::UpdateByLMouse(float value){
	if(!isLMouseDown())return;
	if(_crrMesh == nullptr)return;

	int dx = _lastMouseX - GetMouseX();
	int dy = _lastMouseY - GetMouseY();
	if(abs(dx) > abs(dy)){
		_crrMesh->rotateY(-2.0f * dx * value);
	}
	else{
		_crrMesh->rotateX(-2.0f * dy * value);
	}
}

void Window::UpdateByKey(float value){
	if(isKeyDown(97)){
		_camera.strafe(-value);
	}
	if(isKeyDown(100)){
		_camera.strafe(value);
	}
	if(isKeyDown(119)){
		_camera.walk(value);
	}
	if(isKeyDown(115)){
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