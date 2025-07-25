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
	Color clearColor = Color(1.f, 1.f, 1.f, 1.f);

};

class Renderer
{
	DECLARE_SINGLE(Renderer);

public:
	~Renderer();
	WPARAM Run(RenderDesc& desc);

	RenderDesc& GetGameDesc() { return _desc; }
	shared_ptr<class NameManager> GetNameManager() { return _nameManager; }

private:
	ATOM MyRegisterClass();
	BOOL InitInstance(int cmdShow);

	void Tick();

	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:
	shared_ptr<class NameManager> _nameManager = nullptr;
	RenderDesc _desc;
};

