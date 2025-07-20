#include "pch.h"
#include "ShadowMap.h"
#include "Actor/Actor.h"
#include "Managers/ShaderParameterManager.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Buffer/VertexData.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/PixelShader.h"

ShadowMap::ShadowMap()
{
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::Construct()
{
	CreateShadowTexture();
	CreateShadowDSV();
	CreateShadowSRV();
	SetShadowViewport();
	CreateShadowMapResources();
}

void ShadowMap::CreateShadowMap(vector<shared_ptr<Actor>>& actors)
{
	CONTEXT->OMSetRenderTargets(0, nullptr, _shadowDSV.Get());
	CONTEXT->ClearDepthStencilView(_shadowDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	CONTEXT->RSSetViewports(1, &_shadowViewport);
}

void ShadowMap::CreateShadowTexture()
{
	D3D11_TEXTURE2D_DESC desc;
	::ZeroMemory(&desc, sizeof(desc));

	desc.Width = SHADOW_MAP_SIZE;
	desc.Height = SHADOW_MAP_SIZE;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_TYPELESS; // 깊이와 SRV 둘 다 사용하려면 TYPELESS
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _shadowTexture.GetAddressOf());
	check(hr);
}

void ShadowMap::CreateShadowDSV()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	HRESULT hr = DEVICE->CreateDepthStencilView(_shadowTexture.Get(), &desc, _shadowDSV.GetAddressOf());
	check(hr);
}

void ShadowMap::CreateShadowSRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	::ZeroMemory(&desc, sizeof(desc));
	desc.Format = DXGI_FORMAT_R32_FLOAT; // Shader에서 사용할 format
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.MipLevels = 1;

	HRESULT hr = DEVICE->CreateShaderResourceView(_shadowTexture.Get(), &desc, _shadowSRV.GetAddressOf());
	check(hr);
}

void ShadowMap::SetShadowViewport()
{
	_shadowViewport.TopLeftX = 0.f;
	_shadowViewport.TopLeftY = 0.f;
	_shadowViewport.Width = static_cast<float>(SHADOW_MAP_SIZE);
	_shadowViewport.Height = static_cast<float>(SHADOW_MAP_SIZE);
	_shadowViewport.MinDepth = 0.0f;
	_shadowViewport.MaxDepth = 1.0f;
}

void ShadowMap::CreateShadowMapResources()
{
	_shaderInfo = make_shared<ShaderInfo>(SHADER_NAME);

	_resources.vertexShader = make_shared<VertexShader>();
	_resources.vertexShader->Create(_shaderInfo->_shaderPath, _shaderInfo->_vsEntryName, _shaderInfo->_vsVersion);

	_resources.pixelShader = make_shared<PixelShader>();
	_resources.pixelShader->Create(_shaderInfo->_shaderPath, _shaderInfo->_psEntryName, _shaderInfo->_psVersion);

	const vector<D3D11_INPUT_ELEMENT_DESC>& desc = VertexData::descs;
	_resources.inputLayout = make_shared<InputLayout>();
	_resources.inputLayout->Create(desc, _resources.vertexShader->GetBlob());

	// Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 1000;
		desc.SlopeScaledDepthBias = 1.0f;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = false;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = DEVICE->CreateRasterizerState(&desc, _resources.rasterizerState.GetAddressOf());
		check(hr);
	}

	// Depth Stencil State
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = TRUE;							// 깊이 테스트 활성화
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  // 깊이값 기록 허용
		desc.DepthFunc = D3D11_COMPARISON_LESS;			// 더 가까운 픽셀만 통과
		desc.StencilEnable = FALSE;						// 그림자맵에서는 Stencil 불필요

		ID3D11DepthStencilState* shadowDepthState = nullptr;
		HRESULT hr = DEVICE->CreateDepthStencilState(&desc, _resources.DepthStencilState.GetAddressOf());
		check(hr);
	}
}
