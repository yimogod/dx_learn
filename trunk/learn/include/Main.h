#pragma once
#include <windows.h>
#include <BaseApp.h>
#include <sys/Window.h>
#include <sys/App.h>

using namespace plu;

struct MainInfo{
	Window* app;
	LPCWSTR title;
	LPCWSTR icon;
	int width;
	int height;
};

/*全局变量声明*/
HINSTANCE g_hInst = nullptr;
HWND g_hWnd = nullptr;
Window* _window;

/*函数声明*/
void Extract(LPARAM lParam, int& x, int& y);
void Extract(WPARAM wParam, int& x, int& y);

HRESULT InitWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, int, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int MainBody(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow, MainInfo info);

/*函数定义*/
int MainBody(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow, MainInfo info){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HRESULT hr = S_OK;
	/*初始化窗口*/
	hr = InitWindow(hInstance, nCmdShow, info.title, info.icon, info.width, info.height);
	if(FAILED(hr))return NULL;

	_window = info.app;

	/*初始化d3d设备*/
	hr = _window->init(hInstance, g_hWnd);

	if(FAILED(hr)){
		_window->destroy();
		return NULL;
	}

	MSG msg = { 0 };
	while(WM_QUIT != msg.message){
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		_window->update();
	}

	_window->destroy();

	return (int)msg.wParam;
}

/*注册并创建窗口*/
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow, LPCWSTR title, LPCWSTR icon, int width, int height){
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, icon);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, icon);
	if(!RegisterClassEx(&wcex))return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"TutorialWindowClass", title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, hInstance, nullptr);
	if(!g_hWnd)return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT ps;
	HDC hdc;

	switch(message){
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		// Get the virtual key code.
		int key = (int)wParam;

		// Quit the application when the 'escape' key is pressed.
		if(key == VK_ESCAPE){
			PostQuitMessage(0);
			return 0;
		}

		_window->OnKeyDown(key);
		break;

	case WM_KEYUP:
		// Get the virtual key code.
		int key = (int)wParam;
		_window->OnKeyUp(key);
		break;

	case WM_LBUTTONDOWN:
		// Get the modifier flags.
		unsigned int modifiers = (unsigned int)wParam;

		// Get the cursor position in client coordinates.
		int x, y;
		Extract(lParam, x, y);
		_window->OnMouseClick(App::MOUSE_LEFT, App::MOUSE_DOWN,	x, y, modifiers);
		break;
	
	case WM_LBUTTONUP:
		unsigned int modifiers = (unsigned int)wParam;

		int x, y;
		Extract(lParam, x, y);
		_window->OnMouseClick(App::MOUSE_LEFT, App::MOUSE_UP, x, y, modifiers);
		break;
	
	case WM_RBUTTONDOWN:
		unsigned int modifiers = (unsigned int)wParam;

		int x, y;
		Extract(lParam, x, y);
		_window->OnMouseClick(App::MOUSE_RIGHT, App::MOUSE_DOWN, x, y, modifiers);
		break;
	
	case WM_RBUTTONUP:
		unsigned int modifiers = (unsigned int)wParam;

		int x, y;
		Extract(lParam, x, y);

		_window->OnMouseClick(App::MOUSE_RIGHT, App::MOUSE_UP, x, y, modifiers);
		break;
	
	case WM_MOUSEMOVE:
		unsigned int modifiers = (unsigned int)wParam;

		int x, y;
		Extract(lParam, x, y);

		App::MouseButton button;

		if(wParam & MK_LBUTTON)button = App::MOUSE_LEFT;
		else if(wParam & MK_RBUTTON)button = App::MOUSE_RIGHT;
		else button = App::MOUSE_NONE;

		_window->OnMouseMotion(button, x, y, modifiers);
		break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Extract(LPARAM lParam, int& x, int& y){
	x = (int)(short)(lParam & 0xFFFF);
	y = (int)(short)((lParam & 0xFFFF0000) >> 16);
}

void Extract(WPARAM wParam, int& x, int& y){
	x = (int)(short)(wParam & 0xFFFF);
	y = (int)(short)((wParam & 0xFFFF0000) >> 16);
}