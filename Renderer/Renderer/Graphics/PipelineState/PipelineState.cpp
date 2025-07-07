#include "pch.h"
#include "PipelineState.h"

PipelineState::PipelineState()
{
}

PipelineState::~PipelineState()
{
}

shared_ptr<PipelineState> PipelineState::GetDefaultState()
{
	shared_ptr<PipelineState> state = make_shared<PipelineState>();

	// Topology
	state->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// Rasterizer
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = false;
		desc.DepthClipEnable = TRUE;

		HRESULT hr = DEVICE->CreateRasterizerState(&desc, state->_rsState.GetAddressOf());
		check(hr);
	}

	// Blend
	{
		D3D11_RENDER_TARGET_BLEND_DESC blendDesc =
		{
			true,
			D3D11_BLEND_SRC_ALPHA,
			D3D11_BLEND_INV_SRC_ALPHA,
			D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ONE,
			D3D11_BLEND_ZERO,
			D3D11_BLEND_OP_ADD,
			D3D11_COLOR_WRITE_ENABLE_ALL
		};

		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0] = blendDesc;

		HRESULT hr = DEVICE->CreateBlendState(&desc, state->_blendState.GetAddressOf());
		check(hr);
	}

	// Sampler
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = 1;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.MaxAnisotropy = 16;
		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = FLT_MIN;
		desc.MipLODBias = 0.0f;

		HRESULT hr = DEVICE->CreateSamplerState(&desc, state->_samplerState.GetAddressOf());
		check(hr);
	}

	// DepthStencil
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
		desc.StencilEnable = FALSE;

		HRESULT hr = DEVICE->CreateDepthStencilState(&desc, state->_dsState.GetAddressOf());
		check(hr);
	}

	return state;
}
