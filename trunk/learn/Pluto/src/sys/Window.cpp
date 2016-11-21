#include <sys/Window.h>
#include <string>

Window::Window(){}

Window::~Window(){}

bool Window::Init(HINSTANCE const &ins, HWND const &hwnd){
	BaseApp::Init(ins, hwnd);
	_dxEngine.InitDevice(hwnd, _width, _height);

	_camera.SetPos(0, 0, -2.0f);
	_camera.SetFrustum(1.0f, 45.0f, 1.0f, 100.0f);
	_camera.SetAspect(_width, _height);

	return LoadContent();
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
		_currMesh->RotateY(dx * value);
	}else{
		_currMesh->RotateX(dy * value);
	}
}

void Window::UpdateByRMouse(float value){
	if(!isRMouseDown())return;

	int dx = _lastMouseX - GetMouseX();
	int dy = _lastMouseY - GetMouseY();
	_camera.RotateY(dx * value);
	_camera.PitchRotate(dy * value);
}

void Window::UpdateByKey(float value){
	if(isKeyDown(65)){//A
		_camera.Strafe(-value);
	}
	if(isKeyDown(68)){//D
		_camera.Strafe(value);
	}
	if(isKeyDown(87)){//W
		_camera.Walk(value);
	}
	if(isKeyDown(83)){//S
		_camera.Walk(-value);
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

void Window::Destroy(){
	UnloadContent();
}