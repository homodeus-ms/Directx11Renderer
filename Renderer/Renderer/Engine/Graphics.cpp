#include "pch.h"
#include "Graphics.h"
#include "Graphics/RenderPass/ShadowMap.h"

void Graphics::BeginPlay(HWND hwnd)
{
	_hwnd = hwnd;

	CreateDeviceAndSwapChain();
	CreateRTVAndSRV();
	CreateCopyTexture();
	CreateDepthStencilView();
	SetViewport();

	_shadowMap = make_shared<ShadowMap>();
	_shadowMap->Construct();
}

void Graphics::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), (float*)(&RENDERER->GetGameDesc().clearColor));
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Graphics::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	check(hr);
}


ComPtr<ID3D11ShaderResourceView> Graphics::GetBackBufferSRV()
{
	// BackBuffer에 그려진 것을 CopyTexture에 한 번 복사 후 CopyTexture의 SRV를 리턴

	ComPtr<ID3D11Texture2D> backBuffer;
	_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	_deviceContext->CopyResource(_copyTexture.Get(), backBuffer.Get());
	
	//_deviceContext->ResolveSubresource(_copyTexture.Get(), 0, backBuffer.Get(), 0, DXGI_FORMAT_R8G8B8A8_UNORM);
	
	return _copyTextureSRV;
}

void Graphics::CreateDeviceAndSwapChain()
{
	UINT deviceFlags = 0;

#if defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = static_cast<UINT>(RENDERER->GetGameDesc().width);
		desc.BufferDesc.Height = static_cast<UINT>(RENDERER->GetGameDesc().height);
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // | DXGI_USAGE_SHADER_INPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		deviceFlags,
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

void Graphics::CreateCopyTexture()
{
	ComPtr<ID3D11Texture2D> backBuffer;
	_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));

	D3D11_TEXTURE2D_DESC desc;
	backBuffer->GetDesc(&desc);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = 0;

	HRESULT hr = _device->CreateTexture2D(&desc, nullptr, _copyTexture.GetAddressOf());
	check(hr);
	hr = _device->CreateShaderResourceView(_copyTexture.Get(), nullptr, _copyTextureSRV.GetAddressOf());
	check(hr);
}

void Graphics::CreateRTVAndSRV()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	check(hr);

	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	check(hr);

	/*hr = _device->CreateShaderResourceView(backBuffer.Get(), nullptr, _shaderResourceView.GetAddressOf());
	check(hr);*/
}

void Graphics::CreateDepthStencilView()
{
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = static_cast<uint32>(RENDERER->GetGameDesc().width);
		desc.Height = static_cast<uint32>(RENDERER->GetGameDesc().height);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _depthStencilTexture.GetAddressOf());
		check(hr);
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = DEVICE->CreateDepthStencilView(_depthStencilTexture.Get(), &desc, _depthStencilView.GetAddressOf());
		check(hr);
	}
}

void Graphics::SetViewport()
{
	//_viewport.TopLeftX = GViewportStartX;
	//_viewport.TopLeftY = GViewportStartY;
	_viewport.TopLeftX = GWinSizeX == VIEW_X ? 0 : GViewportStartX;
	_viewport.TopLeftY = 0;
	_viewport.Width = static_cast<float>(VIEW_X);
	_viewport.Height = static_cast<float>(VIEW_Y);;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}
