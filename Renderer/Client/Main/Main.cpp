#include "pch.h"
#include "Main.h"
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "Engine/Renderer.h"

#include "NormalDemo.h"
#include "LoadMaterialDemo.h"

shared_ptr<EngineClientFlowManager> g_FlowManager;

void CreateConsole()
{
	// Show Console for debug
	AllocConsole();
	FILE* fp;

	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);

	std::ios::sync_with_stdio();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CreateConsole();

	g_FlowManager = make_shared<EngineClientFlowManager>();

	RenderDesc desc;
	desc.appName = L"Renderer";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = GWinSizeX;
	desc.height = GWinSizeY;
	desc.clearColor = Color(1.f, 1.f, 1.f, 1.f);
	desc.app = make_shared<LoadMaterialDemo>();

	RENDERER->Run(desc);

	g_FlowManager.reset();

	return 0;
}