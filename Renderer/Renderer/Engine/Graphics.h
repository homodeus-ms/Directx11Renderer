#pragma once

class ShadowMap;

class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	void BeginPlay(HWND hwnd);

	void RenderBegin();
	void RenderEnd();

	shared_ptr<ShadowMap> GetShadowMap() const { return _shadowMap; }
	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void SetViewport();

	HWND _hwnd{};

	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr;

	ComPtr<ID3D11Texture2D> _depthStencilTexture;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;

	D3D11_VIEWPORT _viewport{};

	shared_ptr<ShadowMap> _shadowMap;
};

