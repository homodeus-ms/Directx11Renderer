#pragma once

class ShadowMap;

class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	void BeginPlay(HWND hwnd);

	void RenderMSAABegin();
	void RenderBegin();
	void RenderEnd();

	shared_ptr<ShadowMap> GetShadowMap() const { return _shadowMap; }
	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }
	ComPtr<ID3D11RenderTargetView> GetBackBufferRTV() { return _renderTargetView; }
	ComPtr<ID3D11ShaderResourceView> GetResolvedSRV();
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return _UAV; }
	
	void ClearDepth();
	void ClearStencil();
	void CreateResourcesForSubWindows(HWND subHwnd, int width, int height);
	void SubWindowRenderBegin();
	void SubWindowRenderEnd();

private:
	void CreateDeviceAndSwapChain();
	void CreateDeviceAndSwapChain(const DXGI_SWAP_CHAIN_DESC& desc);
	void CreateRTVAndSRV();
	void CreateUAV();
	void CreateDepthStencilView();
	void SetViewport();

	void TestCS();

	void CreateSubSwapChain(HWND subHwnd, int width, int height);
	void CreateSubRTVAndDSV(int32 width, int32 height);
	void SetSubViewport(int width, int height);

	

	HWND _hwnd{};

	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr;
	

	// MSAA + float format for HDRI
	ComPtr<ID3D11Texture2D> _floatMSAATexture = nullptr;
	ComPtr<ID3D11RenderTargetView> _floatMSAARTV = nullptr;
	ComPtr<ID3D11Texture2D> _resolvedTexture = nullptr;
	ComPtr<ID3D11ShaderResourceView> _resolvedSRV = nullptr;
	bool _bUseMSAA = true;
	uint32 _numQualityLevels = 0;

	ComPtr<ID3D11Texture2D> _depthStencilTexture;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;

	D3D11_VIEWPORT _viewport{};
	D3D11_VIEWPORT _viewportFull{};
	shared_ptr<ShadowMap> _shadowMap;

	// For CS
	ComPtr<ID3D11UnorderedAccessView> _UAV = nullptr;


	// For Sub UI Window
	ComPtr<IDXGISwapChain> _subSwapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> _subRTV = nullptr;
	ComPtr<ID3D11DepthStencilView> _subDSV = nullptr;
	ComPtr<ID3D11Texture2D> _subDSVTexture = nullptr;
	D3D11_VIEWPORT _subVP{};
};

