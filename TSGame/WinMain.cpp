#include <Windows.h>
#include <memory>
#include <string>
#include <stdexcept>
#include "TSGame.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool registerMyClass(HINSTANCE hInstance);
bool createWindow(HINSTANCE hInstance, int nCmdShow, HWND* hWnd, int width, int height, bool fullScreen);

const TCHAR className[] = TEXT("MyWindow");
const TCHAR titleName[] = TEXT("TitleText");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg = {};
	HWND hWnd = NULL;

	if (!registerMyClass(hInstance))
		return 0;
	if (!createWindow(hInstance, nCmdShow, &hWnd, 640, 480, false))
		return 0;

	try {
		auto game = std::make_unique<TSGame>(hWnd, hInstance);

		while (true) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else {
				game->run();//mainloop
			}
		}
	} catch (const std::runtime_error& error) {
		MessageBox(NULL, error.what(), "Runtime error", MB_OK | MB_ICONERROR);
	} catch (...) {
		MessageBox(NULL, TEXT("Unknown error"), "Error", MB_OK | MB_ICONERROR);
	}

	return msg.wParam;
}

bool registerMyClass(HINSTANCE hInstance) {
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className;
	wc.hIconSm = NULL;

	if (RegisterClassEx(&wc) == 0)
		return false;

	return true;
}

bool createWindow(HINSTANCE hInstance, int nCmdShow, HWND* hWnd, int width, int height, bool fullScreen) {
	DWORD style;

	if (fullScreen)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;

	*hWnd = CreateWindow(className, titleName, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, (HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL);

	if (!*hWnd)
		return false;

	if (!fullScreen)
	{
		RECT clientRect;
		GetClientRect(*hWnd, &clientRect);
		MoveWindow(*hWnd,
			GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2,
			GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
			width + (width - clientRect.right),
			height + (height - clientRect.bottom),
			TRUE);
	}

	ShowWindow(*hWnd, nCmdShow);
	UpdateWindow(*hWnd);

	return true;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
