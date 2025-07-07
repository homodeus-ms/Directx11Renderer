#include "pch.h"
#include "Main.h"
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "Managers/Renderer.h"
#include "TriangleDemo.h"

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

	RenderDesc desc;
	desc.appName = L"Renderer";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = 800;
	desc.height = 600;
	desc.clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
	desc.app = make_shared<TriangleDemo>();

	RENDERER->Run(desc);

	return 0;
}