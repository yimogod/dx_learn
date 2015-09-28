#include "DemoApp1.h"

DemoApp1::DemoApp1(){}

DemoApp1::~DemoApp1(){}

bool DemoApp1::loadContent(){
	return true;
}

void DemoApp1::unloadContent(){

}

void DemoApp1::update(){}

void DemoApp1::render(){
	if(_context == NULL)return;

	float color[4] = {0.0f, 0.0f, 1.0f, 1.0f};

	_context->ClearRenderTargetView(_backBuffTarget, color);
	_chain->Present(0, 0);
}