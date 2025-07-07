#pragma once

class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	void BeginPlay(HWND hwnd);

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

	HWND _hwnd{};

	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr;
	D3D11_VIEWPORT _viewport{};
};

