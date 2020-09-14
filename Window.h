#pragma once

#include <Windows.h>

class Window;

void* temporalPtr;

class Window {
private:
	HINSTANCE mainHInst = nullptr;
	HWND mainHWnd = nullptr;
	HDC mainHDC = nullptr;
	WCHAR* mainWndTitle;

	unsigned int WndWidth = 640;
	unsigned int WndHeight = 480;
	int WndPosX = 50;
	int WndPosY = 60;

	long static __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	WORD RegistClass();
	WORD RegistClass(WNDCLASSEXW WCEx);
	BOOL InitInstance(DWORD style = WS_OVERLAPPEDWINDOW);
public:
	Window(HWND hWnd = nullptr);	/* 初始化窗口 */

	BOOL init(DWORD style = WS_OVERLAPPEDWINDOW);	/* 初始化窗口 */
	BOOL init(WCHAR* wndTitle, unsigned width, unsigned height, DWORD style = WS_OVERLAPPEDWINDOW);
	BOOL init(WCHAR* wndTitle, unsigned width, unsigned height, WNDCLASSEXW WCEx, DWORD style = WS_OVERLAPPEDWINDOW);

	BOOL show(int nCmdShow = SW_SHOW);	/* 显示已初始化的窗口 */
	BOOL show(int posX, int posY, int nCmdShow = SW_SHOW);

	BOOL hide();	/* 隐藏窗口 */

	BOOL close();	/* 关闭窗口 */

	HINSTANCE	GetHInstance();		/* 获得窗口实例 */
	HWND		GetHWnd();			/* 获得窗口句柄 */
	HDC			GetHDC();			/* 获得绘图句柄 */
};

WORD Window::RegistClass() {
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = this->WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = this->mainHInst;
	wcex.hIcon = LoadIconW(this->mainHInst, nullptr/*MAKEINTRESOURCE(IDI_SPEEDYE)*/);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = this->mainWndTitle;
	wcex.lpszClassName = this->mainWndTitle;
	wcex.hIconSm = LoadIconW(wcex.hInstance, nullptr/*MAKEINTRESOURCE(IDI_SMALL)*/);

	return RegisterClassExW(&wcex);
}

WORD Window::RegistClass(WNDCLASSEXW WCEx) {
	return RegisterClassExW(&WCEx);
}

long __stdcall Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		temporalPtr = (void*)hdc;
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

BOOL Window::InitInstance(DWORD style) {
	this->mainHWnd = CreateWindowW(
		this->mainWndTitle,
		this->mainWndTitle,
		style,
		this->WndPosX,
		this->WndPosY,
		this->WndWidth,
		this->WndHeight,
		this->mainHWnd,
		nullptr,
		this->mainHInst,
		nullptr
	);

	if (!this->mainHWnd) return FALSE;

	return TRUE;
}

Window::Window(HWND hWnd) {
	this->mainHWnd = hWnd;
}

BOOL Window::init(DWORD style) {
	this->mainWndTitle = (WCHAR*)L"MyWindow";

	RegistClass();
	return InitInstance(style);
}

BOOL Window::init(WCHAR* wndTitle, unsigned width, unsigned height, DWORD style) {
	this->mainWndTitle = wndTitle;
	this->WndWidth = width;
	this->WndHeight = height;

	RegistClass();
	return InitInstance(style);
}

BOOL Window::init(WCHAR* wndTitle, unsigned width, unsigned height, WNDCLASSEXW WCEx, DWORD style) {
	this->mainWndTitle = wndTitle;
	this->WndWidth = width;
	this->WndHeight = height;

	RegistClass(WCEx);
	return InitInstance(style);
}

BOOL Window::show(int nCmdShow) {
	ShowWindow(this->mainHWnd, nCmdShow);
	UpdateWindow(this->mainHWnd);

	HACCEL hAccelTable = LoadAcceleratorsW(this->mainHInst, nullptr);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return TRUE;
}

BOOL Window::show(int posX, int posY, int nCmdShow) {
	this->WndPosX = posX;
	this->WndPosY = posY;

	ShowWindow(this->mainHWnd, nCmdShow);
	UpdateWindow(this->mainHWnd);

	HACCEL hAccelTable = LoadAcceleratorsW(this->mainHInst, nullptr);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return TRUE;
}

HINSTANCE Window::GetHInstance() {
	return this->mainHInst;
}

HWND Window::GetHWnd() {
	return this->mainHWnd;
}

HDC Window::GetHDC() {
	this->mainHDC = (HDC)temporalPtr;
	return this->mainHDC;
}