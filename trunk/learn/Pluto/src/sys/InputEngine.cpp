#include <sys/InputEngine.h>
InputEngine::InputEngine(){
}
bool InputEngine::isKeyDown(char keycode){
	char a = 0; //_keyboardBuff[keycode];
	return (a & 0x80) != 0;
}

bool InputEngine::isLMouseDown(){
	char a = 0;
	return (a & 0x80) != 0;
}

bool InputEngine::isRMouseDown(){
	char a = 0;
	return (a & 0x80) != 0;
}


/*根据相机姿态调整, pitch, ylow, head*/
/*
void InputEngine::UpdatePosByRMouse(Camera* camera, float value){
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

void InputEngine::UpdatePosByLMouse(Mesh* mesh, float value){
	_isLMouseDown = isLMouseDown();

	if(_isLMouseDown){
		float nx = 0;
		float ny = 0;

		float dx = nx - _mouseX;
		float dy = ny - _mouseY;
		if(abs(dx) > abs(dy)){
			mesh->rotateY(-2.0f * dx * value);
		}
		else{
			mesh->rotateX(-2.0f * dy * value);
		}

		_mouseX = nx;
		_mouseY = ny;
	}
	else if(!_isRMouseDown){
		_mouseX = 0;
		_mouseY = 0;
	}
}*/