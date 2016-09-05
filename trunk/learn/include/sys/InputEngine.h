#pragma once
class InputEngine{
public:
	InputEngine();

protected:
	bool isKeyDown(char keycode);
	bool isLMouseDown();
	bool isRMouseDown();
	//void UpdatePosByRMouse(Camera* camera, float value);
	//void UpdatePosByLMouse(Mesh* mesh, float value);

	float _mouseX, _mouseY;
	bool _isRMouseDown = false;
	bool _isLMouseDown = false;
};
