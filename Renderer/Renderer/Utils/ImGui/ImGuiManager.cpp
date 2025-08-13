#include "pch.h"
#include "ImGuiManager.h"

void ImGuiManager::BeginPlay()
{
	_mainHwnd = RENDERER->GetGameDesc().hWnd;

	IMGUI_CHECKVERSION();
	_mainContext = ImGui::CreateContext();
	ImGui::SetCurrentContext(_mainContext);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(RENDERER->GetGameDesc().hWnd);
	ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

	CreateSubContext();
}

// NewFrame() → UI 코드 호출 → Render()
void ImGuiManager::Tick()
{
	HWND focused = GetForegroundWindow();
	if (focused == _mainHwnd) 
	{
		ImGui::SetCurrentContext(_mainContext);
	}
	else if (focused == _subHwnd) 
	{
		ImGui::SetCurrentContext(_subContext);
	}

	//ImGui::SetCurrentContext(_mainContext);
	if (ImGui::GetCurrentContext() == _mainContext)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		_onTick.Broadcast();
	}

	if (ImGui::GetCurrentContext() == _subContext)
	{
		if (_bShowSubWindow)
		{
			ImGui::SetCurrentContext(_subContext);
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			_onSubWindowTick.Broadcast();

			// TEST
			//ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Once);
			//ImGui::SetNextWindowSize({ 500, 250 }, ImGuiCond_Once);
			//ImGui::Begin("SubWindow", nullptr);
			//ImGui::Text("Test Sub UI, Is it Work??\n Is It?\n");
			//ImGui::End();
		}
	}
}

void ImGuiManager::Render()
{
	if (ImGui::GetCurrentContext() == _mainContext)
	{
		ImGui::SetCurrentContext(_mainContext);
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		GRAPHICS->RenderEnd();
	}

	if (ImGui::GetCurrentContext() == _subContext)
	{
		if (_bShowSubWindow)
		{
			GRAPHICS->SubWindowRenderBegin();

			ImGui::SetCurrentContext(_subContext);
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			GRAPHICS->SubWindowRenderEnd();
		}
	}
	
}

void ImGuiManager::CreateSubContext()
{
	assert(_subHwnd == NULL && _subContext == nullptr);

	_subHwnd = RENDERER->CreateSubWindow(500, 400);
	GRAPHICS->CreateResourcesForSubWindows(_subHwnd, 500, 400);

	_subContext = ImGui::CreateContext();
	ImGui::SetCurrentContext(_subContext);

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(_subHwnd);
	ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

	HideSubWindow();

	ImGui::SetCurrentContext(_mainContext);
}

void ImGuiManager::ShowSubWindow()
{
	_bShowSubWindow = true;
	::ShowWindow(_subHwnd, SW_SHOW);
}

void ImGuiManager::HideSubWindow()
{
	_bShowSubWindow = false;
	::ShowWindow(_subHwnd, SW_HIDE);
}

void ImGuiManager::QuitSubContext()
{
	assert(_subContext != nullptr && _subHwnd != NULL);

	ImGui::SetCurrentContext(_subContext);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext(_subContext);
	_subContext = nullptr;

	DestroyWindow(_subHwnd);
	_subHwnd = NULL;

	ImGui::SetCurrentContext(_mainContext);
}

void ImGuiManager::BroadcastSubWindowHidden()
{
	_onSubWindowHidden.Broadcast();
}




