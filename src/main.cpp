#pragma once
#include "core/App.hpp"
#include <string>
#include <filesystem>
#include <Windows.h>
#include <tchar.h>
using namespace std;


//윈도우 프로시저: 윈도우즈 시스템이 WM왔다고 호출할 수 있도록, 프로그램 시작 전에 미리 등록되어야 하는 함수
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	//1. app* 포인터를 꺼냄
	App* pApp = reinterpret_cast<App*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	//2. 꺼낸 포인터로 App객체의 함수를 호출 (NULL체크 필요)
	if (pApp) 
	{
		//키보드, 마우스 등 App로직 호출
		//pApp->handleWindowsMessage(hWnd, msg, wParam, lParam);
	}


	switch (msg) 
	{
	case WM_CREATE:
	{
		//1. lParam을 CREATESTRUCT*로 캐스팅
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);

		//2. CREATESTRUCT안에 숨겨져 있던 app*를 꺼냄
		App* pAppLocal = reinterpret_cast<App*>(pCreate->lpCreateParams);

		//3. 이 App* 포인터를 윈도우의 GWLP_USERDATA공간에 저장
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pAppLocal));
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) 
{
	/* 배포시 경로를 맞추기 위한 설정, 개발 동안은 주석처리
	//실행파일 경로를 솔루션 폴더로 설정
	namespace fs = filesystem;
	wchar_t path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, path, MAX_PATH);

	//폴더 경로만 남김
	fs::path exePath = path;
	fs::path exeDir = exePath.parent_path();
	SetCurrentDirectory(exeDir.c_str());
	*/

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, hInstance,
					nullptr, nullptr, nullptr, nullptr, _T("MyAppClass"), nullptr };
	RegisterClassEx(&wc);

	RECT screenRect;
	GetClientRect(GetDesktopWindow(), &screenRect);
	int width = 1280;
	int height = 800;
	int posX = (screenRect.right - width) * 0.5;
	int posY = (screenRect.bottom - height) * 0.5;

	App app;
	HWND hwnd = CreateWindow(wc.lpszClassName, _T("JIINY's 3D Tools"),
				WS_OVERLAPPEDWINDOW, posX, posY, width, height,
				nullptr, nullptr, hInstance, &app);

	if (!app.initialize(hwnd)) 
	{
		app.shutdown();
		UnregisterClass(wc.lpszClassName, hInstance);
		return 1;
	}

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
	app.run();
	app.shutdown();

	UnregisterClass(wc.lpszClassName, hInstance);
	return 0;
}
