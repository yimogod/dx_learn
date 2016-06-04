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

HRESULT InitWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, int, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
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

	BaseApp* app = info.app;

	/*初始化d3d设备*/
	hr = app->init(hInstance, g_hWnd);

	if(FAILED(hr)){
		app->destroy();
		return NULL;
	}

	MSG msg = { 0 };
	while(WM_QUIT != msg.message){
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		app->update();
		app->render();
	}

	app->destroy();

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
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}