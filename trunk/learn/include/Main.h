#pragma once
#include <windows.h>
#include <BaseApp.h>

struct MainInfo{
	BaseApp* app;
	LPCWSTR title;
	LPCWSTR icon;
	int width;
	int height;
};

HINSTANCE g_hInst = nullptr;
HWND g_hWnd = nullptr;
BaseApp* g_app;


HRESULT InitWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, int, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Extract(LPARAM lParam, int& x, int& y);

int MainBody(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow, MainInfo info);


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

	g_app = info.app;

	/*初始化d3d设备*/
	hr = g_app->Init(hInstance, g_hWnd);

	if(FAILED(hr)){
		g_app->Destroy();
		return NULL;
	}

	MSG msg = { 0 };
	while(WM_QUIT != msg.message){
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		g_app->Update();
		g_app->Render();
	}

	g_app->Destroy();

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
	if(!RegisterClassEx(&wcex))
		return E_FAIL;

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
	{
		int key = (int)wParam;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(hWnd, &point);
		int x = (int)point.x;
		int y = (int)point.y;

		g_app->OnKeyDown(key, x, y);
		break;
	}
	case WM_KEYUP:
	{
		int key = (int)wParam;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(hWnd, &point);
		int x = (int)point.x;
		int y = (int)point.y;

		g_app->OnKeyUp(key, x, y);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x, y;
		Extract(lParam, x, y);
		g_app->OnMouseDown(x, y);
		break;
	}
	case WM_LBUTTONUP:
	{
		int x, y;
		Extract(lParam, x, y);
		g_app->OnMouseDown(x, y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		int x, y;
		Extract(lParam, x, y);
		g_app->OnRMouseDown(x, y);
		break;
	}
	case WM_RBUTTONUP:
	{
		int x, y;
		Extract(lParam, x, y);
		g_app->OnRMouseDown(x, y);
		break;
	}
	case WM_MOUSEMOVE:
	{
		int x, y;
		Extract(lParam, x, y);
		g_app->OnMove(x, y);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Extract(LPARAM lParam, int& x, int& y){
	x = (int)(short)(lParam & 0xFFFF);
	y = (int)(short)((lParam & 0xFFFF0000) >> 16);
}