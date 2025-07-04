#include "pch.h"
#include "Renderer.h"

Renderer::Renderer()
{
	LOG(Log, "Renderer");
}

Renderer::~Renderer()
{
	
}

void Renderer::BeginPlay(HWND hWnd)
{
	_hWnd = hWnd;
	LOG(Warning, "BeginPlay");
}

void Renderer::Tick()
{
}

void Renderer::Render()
{
}
