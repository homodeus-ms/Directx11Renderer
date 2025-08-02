#include "pch.h"
#include "PipelineState.h"
#include "SamplerState.h"

PipelineState* PipelineState::s_defaultStates = nullptr;
PipelineState* PipelineState::s_shadowStates = nullptr;
PipelineState* PipelineState::s_filterStates = nullptr;

bool PipelineState::s_bDescsSet = false;

D3D11_RASTERIZER_DESC PipelineState::DEFAULT_RASTERIZER_DESC;
D3D11_SAMPLER_DESC PipelineState::DEFAULT_SAMPLER_DESC;
D3D11_RENDER_TARGET_BLEND_DESC PipelineState::DEFAULT_RENDER_TARGET_BLEND_DESC;
D3D11_BLEND_DESC PipelineState::DEFAULT_BLEND_DESC;
D3D11_DEPTH_STENCIL_DESC PipelineState::DEFAULT_DEPTH_STENCIL_DESC;

D3D11_RASTERIZER_DESC PipelineState::SHADOW_RASTERIZER_DESC;
D3D11_DEPTH_STENCIL_DESC PipelineState::SHADOW_DEPTH_STENCIL_DESC;

D3D11_RASTERIZER_DESC PipelineState::FILTER_RASTERIZER_DESC;
D3D11_SAMPLER_DESC PipelineState::FILTER_SAMPLER_DESC;

PipelineState::PipelineState()
{
	
}

PipelineState::~PipelineState()
{
	SAFE_DELETE(s_defaultStates);
	SAFE_DELETE(s_shadowStates);
	SAFE_DELETE(s_filterStates);
}

PipelineState* PipelineState::GetDefaultStates()
{
	if (s_defaultStates)
		return s_defaultStates;

	CreateDescs();

	s_defaultStates = new PipelineState();
	
	HRESULT hr = DEVICE->CreateRasterizerState(&DEFAULT_RASTERIZER_DESC, s_defaultStates->_rsState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateSamplerState(&DEFAULT_SAMPLER_DESC, s_defaultStates->_samplerState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateBlendState(&DEFAULT_BLEND_DESC, s_defaultStates->_blendState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateDepthStencilState(&DEFAULT_DEPTH_STENCIL_DESC, s_defaultStates->_dsState.GetAddressOf());
	check(hr);

	return s_defaultStates;
}

PipelineState* PipelineState::GetShadowStates()
{
	if (s_shadowStates)
		return s_shadowStates;

	CreateDescs();

	s_shadowStates = new PipelineState();

	HRESULT hr = DEVICE->CreateRasterizerState(&SHADOW_RASTERIZER_DESC, s_shadowStates->_rsState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateSamplerState(&DEFAULT_SAMPLER_DESC, s_shadowStates->_samplerState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateDepthStencilState(&SHADOW_DEPTH_STENCIL_DESC, s_shadowStates->_dsState.GetAddressOf());
	check(hr);


	return nullptr;
}

PipelineState* PipelineState::GetFilterStates()
{
	if (s_filterStates)
		return s_filterStates;

	CreateDescs();

	s_filterStates = new PipelineState();

	HRESULT hr = DEVICE->CreateRasterizerState(&FILTER_RASTERIZER_DESC, s_filterStates->_rsState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateSamplerState(&FILTER_SAMPLER_DESC, s_filterStates->_samplerState.GetAddressOf());
	check(hr);

	return s_filterStates;
}

void PipelineState::CreateDescs()
{
	if (s_bDescsSet)
		return;

	s_bDescsSet = true;

	//              //
	// Default Desc //
	//              //
	
	// Raterizer Desc
	::ZeroMemory(&DEFAULT_RASTERIZER_DESC, sizeof(DEFAULT_RASTERIZER_DESC));
	{
		DEFAULT_RASTERIZER_DESC.FillMode = D3D11_FILL_SOLID;
		DEFAULT_RASTERIZER_DESC.CullMode = D3D11_CULL_BACK;
		DEFAULT_RASTERIZER_DESC.FrontCounterClockwise = false;
		DEFAULT_RASTERIZER_DESC.DepthClipEnable = TRUE;
	}

	// Sampler Desc
	::ZeroMemory(&DEFAULT_SAMPLER_DESC, sizeof(DEFAULT_SAMPLER_DESC));
	{
		DEFAULT_SAMPLER_DESC.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		DEFAULT_SAMPLER_DESC.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		DEFAULT_SAMPLER_DESC.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		DEFAULT_SAMPLER_DESC.BorderColor[0] = 100.f;
		DEFAULT_SAMPLER_DESC.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		DEFAULT_SAMPLER_DESC.MaxAnisotropy = 16;
		DEFAULT_SAMPLER_DESC.MinLOD = 0.0f;
		DEFAULT_SAMPLER_DESC.MaxLOD = D3D11_FLOAT32_MAX;
		DEFAULT_SAMPLER_DESC.MipLODBias = 0.0f;
		DEFAULT_SAMPLER_DESC.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	}

	// Render Target Blend Desc
	::ZeroMemory(&DEFAULT_RENDER_TARGET_BLEND_DESC, sizeof(DEFAULT_RENDER_TARGET_BLEND_DESC));
	{
		DEFAULT_RENDER_TARGET_BLEND_DESC.BlendEnable = true;
		DEFAULT_RENDER_TARGET_BLEND_DESC.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		DEFAULT_RENDER_TARGET_BLEND_DESC.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		DEFAULT_RENDER_TARGET_BLEND_DESC.BlendOp = D3D11_BLEND_OP_ADD;
		DEFAULT_RENDER_TARGET_BLEND_DESC.SrcBlendAlpha = D3D11_BLEND_ONE;
		DEFAULT_RENDER_TARGET_BLEND_DESC.DestBlendAlpha = D3D11_BLEND_ZERO;
		DEFAULT_RENDER_TARGET_BLEND_DESC.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		DEFAULT_RENDER_TARGET_BLEND_DESC.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// Blend Desc
	::ZeroMemory(&DEFAULT_BLEND_DESC, sizeof(DEFAULT_BLEND_DESC));
	{
		DEFAULT_BLEND_DESC.AlphaToCoverageEnable = false;
		DEFAULT_BLEND_DESC.IndependentBlendEnable = false;
		DEFAULT_BLEND_DESC.RenderTarget[0] = DEFAULT_RENDER_TARGET_BLEND_DESC;
	}

	// DepthStencil Desc
	::ZeroMemory(&DEFAULT_DEPTH_STENCIL_DESC, sizeof(DEFAULT_DEPTH_STENCIL_DESC));
	{
		DEFAULT_DEPTH_STENCIL_DESC.DepthEnable = TRUE;
		DEFAULT_DEPTH_STENCIL_DESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DEFAULT_DEPTH_STENCIL_DESC.DepthFunc = D3D11_COMPARISON_LESS;
		DEFAULT_DEPTH_STENCIL_DESC.StencilEnable = FALSE;
	}


	//              //
	// Shadow Desc  //
	//              //

	::ZeroMemory(&SHADOW_RASTERIZER_DESC, sizeof(SHADOW_RASTERIZER_DESC));
	{
		SHADOW_RASTERIZER_DESC.FillMode = D3D11_FILL_SOLID;
		SHADOW_RASTERIZER_DESC.CullMode = D3D11_CULL_BACK;
		SHADOW_RASTERIZER_DESC.FrontCounterClockwise = false;
		SHADOW_RASTERIZER_DESC.DepthBias = 100;
		SHADOW_RASTERIZER_DESC.SlopeScaledDepthBias = 3.f;
		SHADOW_RASTERIZER_DESC.DepthBiasClamp = 0.0f;
		SHADOW_RASTERIZER_DESC.DepthClipEnable = true;
		SHADOW_RASTERIZER_DESC.ScissorEnable = false;
		SHADOW_RASTERIZER_DESC.MultisampleEnable = false;
		SHADOW_RASTERIZER_DESC.AntialiasedLineEnable = false;
	}

	::ZeroMemory(&SHADOW_DEPTH_STENCIL_DESC, sizeof(SHADOW_DEPTH_STENCIL_DESC));
	{
		SHADOW_DEPTH_STENCIL_DESC.DepthEnable = TRUE;
		SHADOW_DEPTH_STENCIL_DESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		SHADOW_DEPTH_STENCIL_DESC.DepthFunc = D3D11_COMPARISON_LESS;
		SHADOW_DEPTH_STENCIL_DESC.StencilEnable = FALSE;
	}

	//              //
	// Filter Desc  //
	//              //

	
	ZeroMemory(&FILTER_RASTERIZER_DESC, sizeof(FILTER_RASTERIZER_DESC));
	{
		FILTER_RASTERIZER_DESC.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		FILTER_RASTERIZER_DESC.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		FILTER_RASTERIZER_DESC.FrontCounterClockwise = false;
		FILTER_RASTERIZER_DESC.DepthClipEnable = false;
	}

	ZeroMemory(&FILTER_SAMPLER_DESC, sizeof(FILTER_SAMPLER_DESC));
	{
		FILTER_SAMPLER_DESC.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		FILTER_SAMPLER_DESC.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		FILTER_SAMPLER_DESC.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		FILTER_SAMPLER_DESC.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		FILTER_SAMPLER_DESC.ComparisonFunc = D3D11_COMPARISON_NEVER;
		FILTER_SAMPLER_DESC.MinLOD = 0;
		FILTER_SAMPLER_DESC.MaxLOD = D3D11_FLOAT32_MAX;
	}
}


