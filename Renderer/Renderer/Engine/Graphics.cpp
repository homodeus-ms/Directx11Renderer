#include "pch.h"
#include "Graphics.h"
#include "Graphics/RenderPass/ShadowMap.h"
#include "Graphics/RenderPass/CommonRenderResource.h"
#include "Graphics/RenderPass/PSO.h"

void Graphics::BeginPlay(HWND hwnd)
{
	_hwnd = hwnd;

	CreateDeviceAndSwapChain();
	CreateRTVAndSRV();
	CreateDepthStencilView();
	SetViewport();

	GET_SINGLE(CommonRenderResource)->Initialize();

	_shadowMap = make_shared<ShadowMap>();
	_shadowMap->Construct();
}

void Graphics::RenderMSAABegin()
{
	_deviceContext->OMSetRenderTargets(1, _floatMSAARTV.GetAddressOf(), _depthStencilView.Get());
	_deviceContext->ClearRenderTargetView(_floatMSAARTV.Get(), (float*)(&RENDERER->GetGameDesc().clearColor));
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Graphics::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
	_deviceContext->ClearRenderTargetView(_floatMSAARTV.Get(), (float*)(&RENDERER->GetGameDesc().clearColor));
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Graphics::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	check(hr);
}

void Graphics::SubWindowRenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _subRTV.GetAddressOf(), _subDSV.Get());
	_deviceContext->ClearRenderTargetView(_subRTV.Get(), (float*)(&RENDERER->GetGameDesc().clearColor));
	_deviceContext->ClearDepthStencilView(_subDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	_deviceContext->RSSetViewports(1, &_subVP);
}

void Graphics::SubWindowRenderEnd()
{
	HRESULT hr = _subSwapChain->Present(1, 0);
	check(hr);
}

ComPtr<ID3D11ShaderResourceView> Graphics::GetResolvedSRV()
{
	_deviceContext->ResolveSubresource(_resolvedTexture.Get(), 0, _floatMSAATexture.Get(), 0, DXGI_FORMAT_R16G16B16A16_FLOAT);

	return _resolvedSRV;
}

void Graphics::ClearDepth()
{
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0.f);
}

void Graphics::ClearStencil()
{
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_STENCIL, 1.f, 0.f);
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



void Graphics::CreateRTVAndSRV()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	check(hr);
	// 최종 출력용 RTV
	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	check(hr);

	// MSAA RTV
	hr = _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT, 4, &_numQualityLevels);
	check(hr);

	D3D11_TEXTURE2D_DESC desc;
	backBuffer->GetDesc(&desc);
	desc.MipLevels = desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = 0;
	if (_bUseMSAA && _numQualityLevels) 
	{
		desc.SampleDesc.Count = 4;
		desc.SampleDesc.Quality = _numQualityLevels - 1;
	}
	else {
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
	}

	hr = _device->CreateTexture2D(&desc, NULL, _floatMSAATexture.GetAddressOf());
	check(hr);

	hr = _device->CreateRenderTargetView(_floatMSAATexture.Get(), NULL, _floatMSAARTV.GetAddressOf());
	check(hr);

	// TEMP : for check
	D3D11_RENDER_TARGET_VIEW_DESC forCheck;
	_floatMSAARTV->GetDesc(&forCheck);

	// MSAA -> Resoulve Texture
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	hr = _device->CreateTexture2D(&desc, NULL, _resolvedTexture.GetAddressOf());
	check(hr);

	hr = _device->CreateShaderResourceView(_resolvedTexture.Get(), NULL, _resolvedSRV.GetAddressOf());
	check(hr);

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

		if (_numQualityLevels > 0) 
		{
			desc.SampleDesc.Count = 4;
			desc.SampleDesc.Quality = _numQualityLevels - 1;
		}
		else {
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
		}
		
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _depthStencilTexture.GetAddressOf());
		check(hr);
	}

	HRESULT hr = _device->CreateDepthStencilView(
		_depthStencilTexture.Get(), 0, _depthStencilView.GetAddressOf());

	//{
	//	D3D11_DEPTH_STENCIL_VIEW_DESC desc;
	//	ZeroMemory(&desc, sizeof(desc));
	//	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//	desc.Texture2D.MipSlice = 0;
	//
	//	HRESULT hr = DEVICE->CreateDepthStencilView(_depthStencilTexture.Get(), &desc, _depthStencilView.GetAddressOf());
	//	check(hr);
	//}
}

void Graphics::SetViewport()
{
	_viewport.TopLeftX = GWinSizeX == VIEW_X ? 0 : GViewportStartX;
	_viewport.TopLeftY = 0;
	_viewport.Width = static_cast<float>(VIEW_X);
	_viewport.Height = static_cast<float>(VIEW_Y);;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}

void Graphics::CreateResourcesForSubWindows(HWND subHwnd, int width, int height)
{
	CreateSubSwapChain(subHwnd, width, height);
	CreateSubRTVAndDSV(width, height);
	SetSubViewport(width, height);
}

void Graphics::CreateSubSwapChain(HWND subHwnd, int width, int height)
{
	ComPtr<IDXGIDevice> dxgiDevice;
	_device.As(&dxgiDevice);

	ComPtr<IDXGIAdapter> adapter;
	dxgiDevice->GetAdapter(&adapter);

	ComPtr<IDXGIFactory> factory;
	adapter->GetParent(__uuidof(IDXGIFactory), &factory);


	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = subHwnd;
	desc.Windowed = TRUE;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr = factory->CreateSwapChain(_device.Get(), &desc, _subSwapChain.GetAddressOf());
	check(hr);
}

void Graphics::CreateSubRTVAndDSV(int32 width, int32 height)
{
	ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = _subSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	check(hr);
	
	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _subRTV.GetAddressOf());
	check(hr);

	// Texture
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	hr = _device->CreateTexture2D(&depthDesc, nullptr, _subDSVTexture.GetAddressOf());
	check(hr);

	// DSV
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = _device->CreateDepthStencilView(_subDSVTexture.Get(), &dsvDesc, _subDSV.GetAddressOf());
	check(hr);
}

void Graphics::SetSubViewport(int width, int height)
{
	_subVP.TopLeftX = 0.0f;
	_subVP.TopLeftY = 0.0f;
	_subVP.Width = static_cast<float>(width);
	_subVP.Height = static_cast<float>(height);
	_subVP.MinDepth = 0.0f;
	_subVP.MaxDepth = 1.0f;
}
