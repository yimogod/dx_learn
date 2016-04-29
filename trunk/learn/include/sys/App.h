#pragma once
#include <string>

using namespace std;

namespace plu{
	class App{
	public:
		enum MouseButton{
			MOUSE_NONE,
			MOUSE_LEFT,
			MOUSE_MIDDLE,
			MOUSE_RIGHT
		};

		enum MouseState{
			MOUSE_UP,
			MOUSE_DOWN
		};
	};
}

