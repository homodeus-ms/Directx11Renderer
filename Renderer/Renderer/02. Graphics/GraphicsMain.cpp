#include "pch.h"
#include "GraphicsMain.h"

GraphicsMain::GraphicsMain(HWND hWnd)
{
	_hWnd = hWnd;

	CreateSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

GraphicsMain::~GraphicsMain()
{
}

void GraphicsMain::RenderBegin()
{
	
}

void GraphicsMain::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);    // param1 : V-sync true
	check(hr);
}

void GraphicsMain::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BufferDesc.Width = GWinSizeX;
	desc.BufferDesc.Height = GWinSizeY;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate = { 60, 1 };
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;    // 1: Double Buffering
	desc.OutputWindow = _hWnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	check(hr);
}

void GraphicsMain::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	check(hr);

	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	check(hr);
	
}

void GraphicsMain::SetViewport()
{
}
