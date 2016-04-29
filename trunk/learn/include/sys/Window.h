#pragma once
#include <string>
#include <windows.h>

#include <sys/SysEnv.h>

using namespace std;

namespace plu{
	class Window{
	public:
		Window();
		~Window();

		bool init(HINSTANCE ins, HWND hwnd);
		virtual void update();
		virtual void destroy();

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
