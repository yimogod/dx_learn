#pragma once
#include <string>
#include <windows.h>

#include <sys/SysEnv.h>
#include <sys/App.h>

using namespace std;

namespace plu{
	class Window{
	public:
		Window();
		~Window();

		bool init(HINSTANCE ins, HWND hwnd);
		virtual void update();
		virtual void destroy();

		virtual void OnKeyDown(int key);
		virtual void OnKeyUp(int key);

		virtual bool OnMouseClick(App::MouseButton button, App::MouseState state,
			int x, int y, unsigned int modifiers);

		virtual bool OnMouseMotion(App::MouseButton button, int x, int y,
			unsigned int modifiers);

	protected:
		virtual bool loadContent();
		virtual void unloadContent();

		string getAppRoot();
		string getFullPath(char* path);
		wstring getFullPathW(char* path);

	protected:
		HINSTANCE _ins;
		HWND _hwnd;
		SysEnv _env;

		int _width, _height;
	};
}
