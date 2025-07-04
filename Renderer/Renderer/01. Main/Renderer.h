#pragma once

class Renderer
{
public:
	Renderer();
	~Renderer();

	void BeginPlay(HWND hWnd);
	void Tick();
	void Render();

private:
	HWND _hWnd{};
};
