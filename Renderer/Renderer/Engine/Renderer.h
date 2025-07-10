#pragma once

class IExecute;

struct RenderDesc
{
	shared_ptr<IExecute> app = nullptr;
	wstring appName = L"Renderer";
	HINSTANCE hInstance{};
	HWND hWnd{};
	float width = GWinSizeX;
	float height = GWinSizeY;
	bool vsync = false;
	bool windowed = true;
	Color clearColor = Color(0.f, 0.f, 0.f, 0.f);
};

class Renderer
{
	DECLARE_SINGLE(Renderer);

public:
	WPARAM Run(RenderDesc& desc);

	RenderDesc& GetGameDesc() { return _desc; }

private:
	ATOM MyRegisterClass();
	BOOL InitInstance(int cmdShow);

	void Tick();

	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:
	RenderDesc _desc;
};

