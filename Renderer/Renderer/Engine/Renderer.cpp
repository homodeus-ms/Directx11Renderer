#include "pch.h"
#include "Renderer.h"
#include "Engine/IExecute.h"
#include "Managers/SceneManager.h"
#include "Managers/ShaderParameterManager.h"    // TEMP

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Renderer::~Renderer()
{
	_nameManager.reset();
}

WPARAM Renderer::Run(RenderDesc& desc)
{
	_desc = desc;
	assert(_desc.app != nullptr);

	// 1) 윈도우 창 정보 등록
	MyRegisterClass();

	// 2) 윈도우 창 생성
	if (!InitInstance(SW_SHOWNORMAL))
		return FALSE;

	GRAPHICS->BeginPlay(_desc.hWnd);
	TIME_MANAGER->BeginPlay();
	INPUT_MANAGER->BeginPlay(_desc.hWnd);
	GUI->BeginPlay();
	RESOURCE_MANAGER->BeginPlay();
	SHADER_PARAM_MANAGER->BeginPlay();

	// NameManager : static Singleton으로 했을 때 종료 순서가 보장이 안됨 -> shared_ptr로 관리
	_nameManager = NameManager::CreateNameManager();
	
	// Keep Order
	// app Construct -> Scene Construct -> Scene BeginPly -> app BeginPlay
	_desc.app->Construct();
	SCENE_MANAGER->BeginPlay();
	_desc.app->BeginPlay();


	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			Tick();
		}
	}

	return msg.wParam;
}

void Renderer::Tick()
{
	TIME_MANAGER->Tick();
	INPUT_MANAGER->Tick();

	//GRAPHICS->RenderBegin();
	GUI->Tick();

	SCENE_MANAGER->Tick();
	if (_desc.app)
	{
		_desc.app->Tick();
		_desc.app->Render();
	}
	SCENE_MANAGER->Render();

	GUI->Render();
	
	GRAPHICS->RenderEnd();
}



ATOM Renderer::MyRegisterClass()
{
	WNDCLASSEXW wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _desc.hInstance;
	wcex.hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _desc.appName.c_str();
	wcex.hIconSm = wcex.hIcon;

	return RegisterClassExW(&wcex);
}

BOOL Renderer::InitInstance(int cmdShow)
{
	RECT windowRect = { 0, 0, static_cast<LONG>(_desc.width), static_cast<LONG>(_desc.height) };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	_desc.hWnd = CreateWindowW(_desc.appName.c_str(), _desc.appName.c_str(), WS_OVERLAPPEDWINDOW,
		GWinStartX, GWinStartY, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, _desc.hInstance, nullptr);

	if (!_desc.hWnd)
		return FALSE;

	::ShowWindow(_desc.hWnd, cmdShow);
	::UpdateWindow(_desc.hWnd);

	return TRUE;
}



LRESULT Renderer::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return ::DefWindowProc(handle, message, wParam, lParam);
	}

	return 0;
}
