#pragma once
#include "BaseApp.h"
class DemoApp1 : public BaseApp{
public:
	DemoApp1();
	virtual ~DemoApp1();

	bool loadContent();
	void unloadContent();

	void update();
	void render();
};