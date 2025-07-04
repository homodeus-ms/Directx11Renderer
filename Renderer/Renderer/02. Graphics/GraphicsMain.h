#pragma once

class GraphicsMain
{
public:
	GraphicsMain(HWND hWnd);
	~GraphicsMain();

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	void CreateSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

private:
	HWND _hWnd{};

	ComPtr<ID3D11Device> _device{};
	ComPtr<ID3D11DeviceContext> _deviceContext{};
	ComPtr<IDXGISwapChain> _swapChain{};
	
	ComPtr<ID3D11RenderTargetView> _renderTargetView{};

	D3D11_VIEWPORT _viewport{};
	const float _clearColor[4] = { 0.5f, };
};

