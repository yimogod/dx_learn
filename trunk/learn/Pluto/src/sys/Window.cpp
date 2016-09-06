#include <sys/Window.h>

Window::Window(){}

Window::~Window(){}

void Window::Update(){


	UpdateByLMouse(0.1f);

	_lastMouseX = GetMouseX();
	_lastMouseY = GetMouseY();
}

void Window::UpdateByLMouse(float value){
	if(!isLMouseDown())return;
	if(_crrMesh == nullptr)return;

	float dx = _lastMouseX - GetMouseX();
	float dy = _lastMouseY - GetMouseY();
	if(abs(dx) > abs(dy)){
		_crrMesh->rotateY(-2.0f * dx * value);
	}
	else{
		_crrMesh->rotateX(-2.0f * dy * value);
	}
}

void Window::UpdateByRMouse(float value){
	if(isKeyDown(DIK_A)){
		camera->strafe(-value);
	}
	if(isKeyDown(DIK_D)){
		camera->strafe(value);
	}
	if(isKeyDown(DIK_W)){
		camera->walk(value);
	}
	if(isKeyDown(DIK_S)){
		camera->walk(-value);
	}

	_isRMouseDown = isRMouseDown();

	if(_isRMouseDown){
		float nx = 0;//(float)_mouseState.lAxisX;
		float ny = 0;//(float)_mouseState.lAxisY;
		float dx = nx - _mouseX;
		float dy = ny - _mouseY;

		camera->rotateY(-2.0f * dx * value);
		camera->pitchRotate(-2.0f * dy * value);

		_mouseX = nx;
		_mouseY = ny;
	}
	else if(!_isLMouseDown){
		_mouseX = 0;
		_mouseY = 0;
	}
}