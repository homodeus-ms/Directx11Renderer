#include "pch.h"
#include "PipelineState.h"
#include "SamplerState.h"

PipelineState* PipelineState::s_defaultStates = nullptr;
PipelineState* PipelineState::s_shadowStates = nullptr;
PipelineState* PipelineState::s_filterStates = nullptr;
PipelineState* PipelineState::s_drawStencilStates = nullptr;
PipelineState* PipelineState::s_usingStencilStates = nullptr;

bool PipelineState::s_bDescsSet = false;

D3D11_RASTERIZER_DESC PipelineState::DEFAULT_RASTERIZER_DESC;
D3D11_RASTERIZER_DESC PipelineState::CCW_RASTERIZER_DESC;
D3D11_SAMPLER_DESC PipelineState::DEFAULT_SAMPLER_DESC;
D3D11_SAMPLER_DESC PipelineState::CLAMP_SAMPLER_DESC;
D3D11_SAMPLER_DESC PipelineState::COMPARISION_SAMPLER_DESC;

D3D11_RENDER_TARGET_BLEND_DESC PipelineState::DEFAULT_RENDER_TARGET_BLEND_DESC;
D3D11_BLEND_DESC PipelineState::DEFAULT_BLEND_DESC;
D3D11_BLEND_DESC PipelineState::MIRROR_BLEND_DESC;

D3D11_DEPTH_STENCIL_DESC PipelineState::DEFAULT_DEPTH_STENCIL_DESC;
D3D11_DEPTH_STENCIL_DESC PipelineState::DRAW_STENCIL_DESC;
D3D11_DEPTH_STENCIL_DESC PipelineState::USING_STENCIL_DESC;

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
	SAFE_DELETE(s_drawStencilStates);
	SAFE_DELETE(s_usingStencilStates);
}

PipelineState* PipelineState::GetDefaultStates()
{
	if (s_defaultStates)
		return s_defaultStates;

	CreateDescs();

	s_defaultStates = new PipelineState();
	
	HRESULT hr = DEVICE->CreateRasterizerState(&DEFAULT_RASTERIZER_DESC, s_defaultStates->_rsState.GetAddressOf());
	check(hr);

	D3D11_RASTERIZER_DESC wireframeDesc = DEFAULT_RASTERIZER_DESC;
	wireframeDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	hr = DEVICE->CreateRasterizerState(&wireframeDesc, s_defaultStates->_wireframeState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateSamplerState(&DEFAULT_SAMPLER_DESC, s_defaultStates->_samplerState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateSamplerState(&CLAMP_SAMPLER_DESC, s_defaultStates->_clampSamplerState.GetAddressOf());
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

	hr = DEVICE->CreateSamplerState(&COMPARISION_SAMPLER_DESC, s_shadowStates->_samplerState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateDepthStencilState(&SHADOW_DEPTH_STENCIL_DESC, s_shadowStates->_dsState.GetAddressOf());
	check(hr);


	return s_shadowStates;
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

PipelineState* PipelineState::GetDrawStencilStates()
{
	if (s_drawStencilStates)
		return s_drawStencilStates;

	CreateDescs();

	s_drawStencilStates = new PipelineState();

	HRESULT hr = DEVICE->CreateRasterizerState(&DEFAULT_RASTERIZER_DESC, s_drawStencilStates->_rsState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateSamplerState(&DEFAULT_SAMPLER_DESC, s_drawStencilStates->_samplerState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateSamplerState(&CLAMP_SAMPLER_DESC, s_drawStencilStates->_clampSamplerState.GetAddressOf());
	check(hr);

	D3D11_BLEND_DESC desc;
	::memcpy(&desc, &DEFAULT_BLEND_DESC, sizeof(desc));
	desc.RenderTarget[0].BlendEnable = false;
	desc.RenderTarget[0].RenderTargetWriteMask = 0;
	
	hr = DEVICE->CreateBlendState(&desc, s_drawStencilStates->_blendState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateDepthStencilState(&DRAW_STENCIL_DESC, s_drawStencilStates->_dsState.GetAddressOf());
	check(hr);

	return s_drawStencilStates;
}

PipelineState* PipelineState::GetUsingStencilStates()
{
	if (s_usingStencilStates)
		return s_usingStencilStates;

	CreateDescs();

	s_usingStencilStates = new PipelineState();
	
	HRESULT hr = DEVICE->CreateRasterizerState(&CCW_RASTERIZER_DESC, s_usingStencilStates->_rsState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateSamplerState(&DEFAULT_SAMPLER_DESC, s_usingStencilStates->_samplerState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateSamplerState(&CLAMP_SAMPLER_DESC, s_usingStencilStates->_clampSamplerState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateBlendState(&MIRROR_BLEND_DESC, s_usingStencilStates->_blendState.GetAddressOf());
	check(hr);

	hr = DEVICE->CreateDepthStencilState(&USING_STENCIL_DESC, s_usingStencilStates->_dsState.GetAddressOf());
	check(hr);

	return s_usingStencilStates;
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
		DEFAULT_RASTERIZER_DESC.DepthClipEnable = true;
		DEFAULT_RASTERIZER_DESC.MultisampleEnable = true;
	}
	// CounterClockWise RS
	::ZeroMemory(&CCW_RASTERIZER_DESC, sizeof(CCW_RASTERIZER_DESC));
	{
		CCW_RASTERIZER_DESC.FillMode = D3D11_FILL_SOLID;
		CCW_RASTERIZER_DESC.CullMode = D3D11_CULL_BACK;
		CCW_RASTERIZER_DESC.FrontCounterClockwise = true;
		CCW_RASTERIZER_DESC.DepthClipEnable = true;
		CCW_RASTERIZER_DESC.MultisampleEnable = true;
		CCW_RASTERIZER_DESC.DepthBias = -1;                // 살짝 당겨줌
		CCW_RASTERIZER_DESC.SlopeScaledDepthBias = -0.1f;  // 기울기에 따라 오프셋
	}

	// Default Sampler Desc
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

	// Clamp Smapler Desc
	::memcpy(&CLAMP_SAMPLER_DESC, &DEFAULT_SAMPLER_DESC, sizeof(DEFAULT_SAMPLER_DESC));
	{
		CLAMP_SAMPLER_DESC.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		CLAMP_SAMPLER_DESC.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		CLAMP_SAMPLER_DESC.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	}

	// Comparision Sampler
	::memcpy(&COMPARISION_SAMPLER_DESC, &DEFAULT_SAMPLER_DESC, sizeof(DEFAULT_SAMPLER_DESC));
	{
		COMPARISION_SAMPLER_DESC.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		COMPARISION_SAMPLER_DESC.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		COMPARISION_SAMPLER_DESC.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		COMPARISION_SAMPLER_DESC.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		//COMPARISION_SAMPLER_DESC.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		COMPARISION_SAMPLER_DESC.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	}

	// Blend Desc
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

	// Mirror Blend Desc
	ZeroMemory(&MIRROR_BLEND_DESC, sizeof(MIRROR_BLEND_DESC));
	{
		MIRROR_BLEND_DESC.AlphaToCoverageEnable = true; // MSAA
		MIRROR_BLEND_DESC.IndependentBlendEnable = false;
		// 개별 RenderTarget에 대해서 설정 (최대 8개)
		MIRROR_BLEND_DESC.RenderTarget[0].BlendEnable = true;
		MIRROR_BLEND_DESC.RenderTarget[0].SrcBlend = D3D11_BLEND_BLEND_FACTOR;
		MIRROR_BLEND_DESC.RenderTarget[0].DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
		MIRROR_BLEND_DESC.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		MIRROR_BLEND_DESC.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		MIRROR_BLEND_DESC.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		MIRROR_BLEND_DESC.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		// 필요하면 RGBA 각각에 대해서도 조절 가능
		MIRROR_BLEND_DESC.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}


	// DepthStencil Desc
	::ZeroMemory(&DEFAULT_DEPTH_STENCIL_DESC, sizeof(DEFAULT_DEPTH_STENCIL_DESC));
	{
		DEFAULT_DEPTH_STENCIL_DESC.DepthEnable = TRUE;
		DEFAULT_DEPTH_STENCIL_DESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DEFAULT_DEPTH_STENCIL_DESC.DepthFunc = D3D11_COMPARISON_LESS;
		DEFAULT_DEPTH_STENCIL_DESC.StencilEnable = FALSE;
	}

	// Draw Stencil Desc
	::ZeroMemory(&DRAW_STENCIL_DESC, sizeof(DRAW_STENCIL_DESC));
	{
		DRAW_STENCIL_DESC.DepthEnable = TRUE;
		DRAW_STENCIL_DESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		DRAW_STENCIL_DESC.DepthFunc = D3D11_COMPARISON_LESS;

		DRAW_STENCIL_DESC.StencilEnable = true;
		DRAW_STENCIL_DESC.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		DRAW_STENCIL_DESC.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		DRAW_STENCIL_DESC.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DRAW_STENCIL_DESC.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		DRAW_STENCIL_DESC.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		DRAW_STENCIL_DESC.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		DRAW_STENCIL_DESC.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DRAW_STENCIL_DESC.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		DRAW_STENCIL_DESC.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		DRAW_STENCIL_DESC.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}

	// Using Stencil Desc
	::ZeroMemory(&USING_STENCIL_DESC, sizeof(USING_STENCIL_DESC));
	{
		USING_STENCIL_DESC.DepthEnable = TRUE;
		USING_STENCIL_DESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		USING_STENCIL_DESC.DepthFunc = D3D11_COMPARISON_LESS;

		USING_STENCIL_DESC.StencilEnable = true;
		USING_STENCIL_DESC.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		USING_STENCIL_DESC.StencilWriteMask = 0x00;
		
		USING_STENCIL_DESC.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		USING_STENCIL_DESC.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		USING_STENCIL_DESC.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		USING_STENCIL_DESC.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;  // (= Draw When Equal with StencilRef)
	
		USING_STENCIL_DESC.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		USING_STENCIL_DESC.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		USING_STENCIL_DESC.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		USING_STENCIL_DESC.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
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


