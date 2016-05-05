#include <sys/Window.h>
#include <util/CUtil.h>

using namespace plu;

Window::Window(){
}

Window::~Window(){
}

bool Window::init(HINSTANCE ins, HWND hwnd){
	_ins = ins;
	_hwnd = hwnd;

	RECT rect;
	GetClientRect(_hwnd, &rect);
	_width = rect.right - rect.left;
	_height = rect.bottom - rect.top;

	return loadContent();
}

bool Window::loadContent(){
	return true;
}

void Window::unloadContent(){
	
}

void Window::update(){

}

bool Window::OnMouseClick(App::MouseButton button, App::MouseState state,
	int x, int y, unsigned int modifiers){
	return false;
}

bool Window::OnMouseMotion(App::MouseButton button, int x, int y,
	unsigned int modifiers){
	return false;
}

void Window::destroy(){
	unloadContent();
}

string Window::getAppRoot(){
	return _env.getVariable("PLU_ROOT");
}

string Window::getFullPath(char* path){
	return _env.getVariable("PLU_ROOT") + path;
}

wstring Window::getFullPathW(char* path){
	string pstr = _env.getVariable("PLU_ROOT") + path;
	wstring wst;
	str_2_wstr(pstr, wst);
	return wst;
}
